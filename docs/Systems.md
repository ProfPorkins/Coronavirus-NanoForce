# Systems

Systems are a way to organize the game play code into cohesive units.  There are systems to age viruses, update the health of viruses, move entities, update the state of animated sprites, handle particles, process input, and others.  The following diagram provides an overview of the systems in this code.

![Systems Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Systems.png)

The common tie among the systems is that they all expose an `update` method.  In all cases this method accepts elapsed time, any other parameters may or may not be unique to the system.  There is no inheritance model associated with systems.

## Update

All systems have an `update` method that is invoked during the update stage of the game loop.  The `update` method accepts the elapsed time, and for most systems, a list of entities; the particle and keyboard systems don't receive a list of entities.  During the update, the system iterates over the list of entities and acts on the entities as appropriate.  The following code shows the `update` method from the `Age` system...

    void Age::update(const std::chrono::microseconds elapsedTime, std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Virus>>& entities)
    {
        for (auto&& [id, entity] : entities)
        {
            auto age = entity->getComponent<components::Age>();
            auto size = entity->getComponent<components::Size>();

            age->update(elapsedTime);

            auto sizeUpdate = math::lerp(age->get(), std::chrono::microseconds(0), age->getMaturity(), age->getMinSize(), age->getMaxSize());
            sizeUpdate = std::min(age->getMaxSize(), sizeUpdate); // Cap the max size
            size->set({ sizeUpdate, sizeUpdate });
        }
    }

The code iterates through the list of entities, obtains pointers to the `Age` and `Size` components and modifies their state based on the elapsed time.  This is typical of the other systems.

Consider the following code segment from the `GameModel::update` method...

    void GameModel::update(const std::chrono::microseconds elapsedTime)
    {
        m_sysKeyboard.update(elapsedTime);
        m_sysParticle.update(elapsedTime);

        m_updatePlayer(elapsedTime);

        m_sysLifetime.update(elapsedTime, m_bullets);
        m_sysLifetime.update(elapsedTime, m_bombs);
        m_sysLifetime.update(elapsedTime, m_powerups);

        m_sysMovement.update(*m_level, elapsedTime, m_bullets);
        m_sysMovement.update(*m_level, elapsedTime, m_bombs);
        m_sysMovement.update(*m_level, elapsedTime, m_viruses);

        m_sysAge.update(elapsedTime, m_viruses);
        m_sysBirth.update(elapsedTime, m_viruses);
        m_sysHealth.update(elapsedTime, m_viruses);

        m_sysAnimatedSprite.update(elapsedTime, m_powerups);

        ...other code below this...
    }

Notice the game model tracks different types of entities and passes the appropriate entity list to the system.  For example, both the lifetime and movement systems are called three times, each time with a different list of entities.

I considered having a `notifyEntity` or `registerEntity` method on the systems that is called each time a new entity is added.  Such a method could interrogate the type and/or components of the entity to decide if it relevant and keep a reference (shared pointer) to it.  Then when `update` is called, no entity list needs to be passed in, and the game model doesn't need to track different entity types, reducing its complexity.  In my [Game Techniques](https://github.com/ProfPorkins/GameTech) repository, for the C++ multiplayer examples used that approach and it works well.  For this game, I wasn't trying to use a pure ECS model.  The code complexity of the game model isn't too bad through the use of the systems as they are.  A more complex game (e.g., game model) could benefit from the approach taken with the multiplayer examples.  With that said, as I note on the front page of this documentation, I am considering refactoring this code to be much closer to an ECS architecture; for _fun_ of course.

## Animated Sprites

It is worth noting there is an `AnimatedSprite` _component_, an `AnimatedSprite` _system_, and an `AnimatedSprite` _renderer_.  There isn't an `AnimatedSprite` entity, but all powerups contain an `AnimatedSprite` component.  I like this separation of the state, logic, and rendering.  I'm never going to do it, but if the rendering system were changed, to use something other than the SFML, then only the `AnimatedSprite` renderer needs to be changed, the component and system ("in theory") don't need to be modified.

Some systems update the state of the game play, but not all, like the `AnimatedSprite` system.  During its update, it obtains the `AnimatedSprite` component, works through how much time has elapsed and modifies the component state.  The state of this component is later used by the `AnimatedSprite` renderer to render its current state.

## Input Processing

The only input accepted during game play is from the keyboard and it is, unsurprisingly, the `KeyboardInput` system that handles that.  Keyboard input is something of a publish/subscribe model.  Several methods to register or unregister (e.g., subscribe) from input events are exposed.  During the main game loop, all keyboard events are signaled to the game model, which are then passed to the keyboard system and queued for processing.  When the `update` method is called, all queued input events are processed.

The design of the keyboard input is to make it like the other game systems.  It is contained by the game model, used by the game model, and told when to update by the game model.  The following shows how keyboard inputs are registered in the `GameModel::startPlayer` method...

    auto player = m_player;
    m_sysKeyboard.registerKeyPressedHandler(Configuration::get<std::string>(config::KEYBOARD_UP), [player]() { player->startThrust(); });
    m_sysKeyboard.registerKeyReleasedHandler(Configuration::get<std::string>(config::KEYBOARD_UP), [player]() { player->endThrust(); });
    m_sysKeyboard.registerHandler(Configuration::get<std::string>(config::KEYBOARD_LEFT), true, std::chrono::microseconds(0), [player](std::chrono::microseconds elapsedTime) { player->rotateLeft(elapsedTime); });
    m_sysKeyboard.registerHandler(Configuration::get<std::string>(config::KEYBOARD_RIGHT), true, std::chrono::microseconds(0), [player](std::chrono::microseconds elapsedTime) { player->rotateRight(elapsedTime); });
    //
    // Primary weapon fire
    m_sysKeyboard.registerHandler(
        Configuration::get<std::string>(config::KEYBOARD_PRIMARY_FIRE), true, std::chrono::microseconds(0),
        [this, player]([[maybe_unused]] std::chrono::microseconds elapsedTime) {
            player->getPrimaryWeapon()->fire(
                [this](std::shared_ptr<entities::Entity>& bullet) { this->emitBullet(bullet); },
                [this](std::shared_ptr<entities::Entity>& bomb) { this->emitBomb(bomb); });
        });

    //
    // Secondary weapon fire
    m_sysKeyboard.registerHandler(
        Configuration::get<std::string>(config::KEYBOARD_SECONDARY_FIRE), true, std::chrono::microseconds(0),
        [this, player]([[maybe_unused]] std::chrono::microseconds elapsedTime) {
            player->getSecondaryWeapon()->fire(
                [this](std::shared_ptr<entities::Entity>& bullet) { this->emitBullet(bullet); },
                [this](std::shared_ptr<entities::Entity>& bomb) { this->emitBomb(bomb); });
        });

This design fully separates the behavior of the player controlled ship from the type of input.  In theory, an "AI" player could be developed and it could interface with the player controlled ship using the same methods, it doesn't have to be controlled by keyboard input.

Another design could have the keyboard system external to the game model perhaps as a singleton, similar to the other code singletons.  In this way, keyboard input could be given to it directly from the game loop and the `update` method could also be called in the game loop, being handled independently from the game model.  I'm not documenting the user interface code, but it handles keyboard and mouse input independently from the `KeyboardInput` system.  If the system were refactored to a singleton, in addition to the game model, the user interface code could then use this system.  Possibly, such an approach could reduce overall code complexity.  There is a lot to like about this approach and even as I write about it, I'm considering it very seriously; something else to add to the future revisions list.

At one time I started work towards integrating controller input, but never went very far with it, could be something to add to the future revisions list.
