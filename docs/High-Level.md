# High-Level Overview

The high-level overview of the architecture looks like the following diagram.

![High Level Overview](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/High-Level-Overview.png)

The GameModel is the focus of the code, with most everything else in support of it.  The game model is composed of the current level, all entities currently in the game, the systems used to update the entities, the renderers used to, well, render the game entities, and other logic used to provide the gameplay.  At 600+ lines of code, it is the largest code file; I'm not proud it is that large, but it isn't (yet) problematic.

The GameModel is primarily supported by the following capabilities

* Entities & Components : These make up most of the game play items.  Entities include the player controlled ship, weapons, projectiles, bombs, powerups, and the viruses.  Entities are composed from Components.  Components include attributes like `Age`, `Damage`, `Drag`, `Health`, `Lifetime`, among others.  A single `Entity` may be composed of any number of components.
* Systems : These are used to update aspects of the gameplay, primarily acting upon entities.  For example, powerups have an animated appearance, it is the `AnimatedSprite` system that updates the state of the appearance.
* Renderers : As the name indicates, there are used to draw something.  Referring to the `AnimatedSprite` example described above.  In addition to having an `AnimatedSprite` system, there is an `AnimatedSprite` renderer that takes the current state of an `AnimatedSprite` `Entity` and draws it.
* Levels : These are the gameplay arenas.  The level describes things like the background image, size, music, max viruses, etc.

## Additional Support

There are three additional pieces to the overall architecture.  The are services to support loading and retrieval of assets, playing of sound and music, and loading and maintenance of game configuration settings.  Each of these are described below.

All three of these services are implemented as Singletons.  I know it is popular to shame the use of Singletons, but the technique works well for these.  For each of these, there should only ever be the possibility of one of them existing, and each of these needs to be available to many parts of the game code.  I did consider making something of a service architecture instead of using Singletons, but that was going to be more work and more complexity for a benefit not necessary for this code.

## Configuration (class `Configuration`)

The very first step, when the game is started, is to read the configuration.  There are actually two possible configuration files to read.  The first is the _settings_ configuration (`client.settings.json`), the second is the _developer_ configuration (`client.developer.json`).  The settings include items like the resolution, full screen or windowed, keyboard controls, and whether or not to play background music.  The developer configuration details which fonts to use, attributes for the entities, and attributes for each of the levels.

The configuration is stored in a JSON format.  This is a format I happen to like, and given that the number of settings is small, the file size is not of any concern.

**Why two configuration files?** The `client.settings.json` file stores settings that the user can change.  The `client.developer.json` file stores settings the user shouldn't change.  If the `client.developer.json` file exists, the settings from that file are used.  If the file doesn't exist, a hard-coded string is used instead.  During development, a developer can have the `client.developer.json` file as part of the build and change the settings as desired.  Then, when ready to ship the game, take the JSON string and hard-code it into the executable.

I acknowledge this leaves the game susceptible to exe hacking, someone changing the settings to make the game easier and end up at the top of leader boards.  I had started down a path to hash the hard-coded settings and compare a run-time computed hash with the stored hash.  Unfortunately, that doesn't work because both the hard-coded string and the hard-coded hash to compare with can be hacked.  In the end, don't have a verification to ensure the integrity of this data.  If someone wants to be at the top of the leader boards bad enough they are willing to hack the exe, well, that is that.

## Asset Management (class `Content`)

## Sound & Music Player (class `SoundPlayer`)
