# High-Level Overview

The high-level overview of the architecture looks like the following diagram.

![High Level Overview](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/High-Level-Overview.png)

The GameModel is the focus of the code, with most everything else in support of it.  The game model is composed of the current level, all entities currently in the game, the systems used to update the entities, the renderers used to, well, render the game entities, and other logic used to provide the gameplay.  At 600+ lines of code, it is the largest code file; I'm not proud it is that large, but it isn't (yet) problematic.

The GameModel is primarily supported by the following capabilities

* **Entities & Components** : These make up most of the game play items.  Entities include the player controlled ship, weapons, projectiles, bombs, powerups, and the viruses.  Entities are composed from Components.  Components include attributes like `Age`, `Damage`, `Drag`, `Health`, `Lifetime`, among others.  A single `Entity` may be composed of any number of components.
* **Systems** : These are used to update aspects of the gameplay, primarily acting upon entities.  For example, powerups have an animated appearance, it is the `AnimatedSprite` system that updates the state of the appearance.
* **Renderers** : As the name indicates, there are used to draw something.  Referring to the `AnimatedSprite` example described above.  In addition to having an `AnimatedSprite` system, there is an `AnimatedSprite` renderer that takes the current state of an `AnimatedSprite` `Entity` and draws it.
* **Levels** : These are the gameplay arenas.  The level describes things like the background image, size, music, max viruses, etc.

## Additional Support

There are three additional pieces to the overall architecture.  The are services to support loading and retrieval of assets, playing of sound and music, and loading and maintenance of game configuration settings.  Each of these are described below.

All three of these services are implemented as Singletons.  I know it is popular to shame the use of Singletons, but the technique works well for these.  For each of these, there should only ever be the possibility of one of them existing, and each of these needs to be available to many parts of the game code.  I did consider making something of a service architecture instead of using Singletons, but that was going to be more work and more complexity for no additional benefit that I could see.

## Configuration (class `Configuration`)

The very first step, when the game is started, is to read the configuration.  There are actually two possible configuration files to read.  The first is the _settings_ configuration (`client.settings.json`), the second is the _developer_ configuration (`client.developer.json`).  The settings include items like the resolution, full screen or windowed, keyboard controls, and whether or not to play background music.  The developer configuration details which fonts to use, attributes for the entities, and attributes for each of the levels.

The configuration is stored in a JSON format.  This is a format I happen to like, and given that the number of settings is small, the file size is not of any concern.

**Why two configuration files?** The `client.settings.json` file stores settings that the user can change.  The `client.developer.json` file stores settings the user shouldn't change.  If the `client.developer.json` file exists, the settings from that file are used.  If the file doesn't exist, a hard-coded string is used instead.  During development, a developer can have the `client.developer.json` file as part of the build and change the settings as desired.  Then, when ready to ship the game, take the JSON string and hard-code it into the executable.

I acknowledge this leaves the game susceptible to exe hacking, someone changing the settings to make the game easier and end up at the top of leader boards.  I had started down a path to hash the hard-coded settings and compare a run-time computed hash with the stored hash.  Unfortunately, that doesn't work because both the hard-coded string and the hard-coded hash to compare with can be hacked.  In the end, don't have a verification to ensure the integrity of this data.  If someone wants to be at the top of the leader boards bad enough they are willing to hack the exe, well, that is that.

### Template Specialization

Two methods are exposed for management of the configuration, `set` and `get`; doing what you expect.  An interesting note about their implementation is the use of template specialization.  Different data types can be stored in the configuration; `bool`, `std::string`, `std::chrono::milliseconds`, and multiple integer types.  The reason for using template specialization is because RapidJson has differently named methods for obtaining the different data types.

The declaration for the `get` method is:

    template <typename T>
    static T get(const std::vector<std::string>& path);

The specializations for obtaining a `bool` and `std::string` are:

    template <>
    bool Configuration::get(const std::vector<std::string>& path)
    {
        return findValue(instance().m_domFull, path)->value.GetBool();
    }

    template <>
    std::string Configuration::get(const std::vector<std::string>& path)
    {
        return findValue(instance().m_domFull, path)->value.GetString();
    }

The use of these methods looks like:

    Configuration::get<std::string>(config::KEYBOARD_PRIMARY_FIRE)
    Configuration::get<bool>(config::GRAPHICS_FULL_SCREEN)

## Asset Management (class `Content`)

Assets (images, sounds, fonts, etc) are managed by the `Content` Singleton.  Several methods are exposed that allow for requesting an asset is loaded, obtaining a pointer to an asset, checking to see if a particular asset exists or if there are any pending loading tasks.  Because the memory footprint of this game is so small, no capability to unload assets is provided.  Once something is loaded into memory, it is there for the duration of the process.

At initialization, this class creates a worker thread that is used to load assets from disk into memory.  The motivation for using a worker thread is to keep the main thread free from being paused when assets are loaded.  The worker thread waits for an event to occur, such as a request to load an asset.  Once an event is signaled, the worker pulls pulls the next available task from a queue and calls the appropriate code to load that asset type.  If the loading was successful, and an `onComplete` function is defined for the task, the `onComplete` function is invoked.  If the loading wasn't successful, and an `onError` function is defined for the task, the `onError` function is invoked.  In this way, code that requests an asset to be loaded can be notified on either success or failure and take appropriate action.

### Template Specialization

Similar to the `Configuration` class described above, template specialization is used by the two methods that initiate loading of an asset and retrieving a pointer to an asset already in memory.  Two methods for loading and retrieval of assets have specializations, their declarations are shown below:

    template <typename T>
    static void load(std::string key, std::string filename, std::function<void(std::string)> onComplete = nullptr, std::function<void(std::string)> onError = nullptr);

    template <typename T>
    static std::shared_ptr<T> get(std::string key);

The specialization for the `sf::Font` type for each is shown next:

    template <>
    void Content::load<sf::Font>(std::string key, std::string filename, std::function<void(std::string)> onComplete, std::function<void(std::string)> onError)
    {
        instance().m_tasks.enqueue({ Task::Type::Font, key, filename, onComplete, onError });
        instance().m_eventTasks.notify_one();
    }

    template <>
    std::shared_ptr<sf::Font> Content::get(std::string key)
    {
        return instance().m_fonts[key];
    }

## Sound Effect Player (class `SoundPlayer`)

The `SoundPlayer` is used to play sound effects, but not the background music.  The class is something just barely more than a lightweight interface to the SFML audio capabilities.  I wanted to create something that allowed the game code to be simple, but the overall code framework still have a certain level of management of what is going on with audio.

At initialization, this class creates a worker thread that is used to watch a queue of audio requests and play.  As sound tasks are added to the queue, the worker thread immediately dequeues them and used the SFML to play them.  When an audio request is made, the `Task` for that request includes a content path to the sound asset.  When the worker thread dequeues the `Task`, a pointer to the audio asset is obtained from the `Content` object and assigned to an SFML object and played.

Another important bit of setup is performed during initialization of this object.  A queue of `sf::Sound` (shared) pointers is created.  The purpose of this queue is to keep pointers to sound tasks (private class `Task`) alive while the sound is being played.  It is necessary to keep a pointer to `sf::Sound` alive in order for the object to say in memory while being played through the SFML.  The size of this queue is set to 100.  Therefore, only up to 100 active sounds can be playing at the same time.  If a new audio request is made, the oldest of the 100 will stop because it is replaced by the newest one.  For this game, never, ever (famous last words) will there be anything close to 100 active sounds being played.

A single method is exposed for playing either a sound (effect) or music, `play`.  This method takes a `std::string`, which is a content path to the audio object, and a volume parameter.  When called, the method creates a task and adds to the queue to be played as soon as possible.

### Background Music

Because the background music is trivial, the game code handles it directly.  The following code is used during the startup of a level to play background music:

    if (Configuration::get<bool>(config::PLAY_BACKGROUND_MUSIC) && Content::has<sf::Music>(m_level->getBackgroundMusicKey()))
    {
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->stop();
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->setVolume(15);
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->setLoop(true);
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->play();
    }
