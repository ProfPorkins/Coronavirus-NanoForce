# Game Model

The `GameModel` is the focus of the code during game play.  It can be thought of the _glue_ that holds everything else together.  All coordination of everything that composes the game play occurs within the `GameModel`.  The game model creates (and manages) the current level, tracks all entities, tells the systems when to update, and invokes the renderers when it is time to draw the scene.  The `GameModel` implementation file has a little under 600 lines of code, I'm not happy it is that long.  Of that, around 250 lines of code are related to the active game play, the rest are associated with initialization and setup.  The bulk of the game play logic is organized throughout the various systems, entities, renderers, and a few other places (e.g., levels).

The following diagram provides an overview of the code associated with the game model.

![GameModel Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/GameModel.png)

## Entities

The game model tracks different sets of entities, these are:

* Player
* Viruses
* Bullets
* Bombs
* Powerups

A few of these entities and how they are handled are described next.

### Player Entity

The player entity is, hopefully, self explanatory.  The player controlled ship needs to be created and initialized at the start of the level.  It also needs to be reinitialized after it is destroyed, and there are still ships available to use.  The core methods associated with the player include...

* `onPlayerDeath()`
* `resetPlayer()`
* `startPlayer()`

The `startPlayer()` method is worth a bit of discussion.  It is in this method where the keyboard controls are registered with the `KeyboardInput` system, allowing for player control of the ship.  The other interesting part of this method is that a lambda function is assigned to a `m_updatePlayer` function, that is part of the `GameModel` class and invoked during the `update` of the game model.  This lambda is where the player entity's `update` methods is invoked, along with the logic to determine if any powerups where picked up, or if any collisions with viruses occurred.

The reason for the `m_updatePlayer` function, and assigning different lambdas to it through the `GameModel` code is to _cleanly_ allow for different update functionality to occur, without having a big ugly if or switch statement based on the game play state.  Depending on the game play state (active game play, or player death), `m_updatePlayer` is assigned a different lambda.  This makes the `update` method of the `GameModel` much simpler, it simply invokes the `m_updatePlayer` function and whatever lambda is assigned to it, executes.

### Virus Entities

The `Virus` entities have several updates that occur on them during the game loop.  This includes updates by several systems, a check to see if they have collided with any _bullets_ (The code uses the term bullets, but they are conceptually antigens that attach to the viruses), check to see if they have collided with the player ship, and then birth and death of viruses.

Four systems act upon virus entities: `Movement`, `Age`, `Birth`, and `Health`.  Movement, well, updates the virus location based on momentum.  Aging of a virus determines its size, and whether or not it has reached maturity and can give birth.  The birth system checks to see if the virus is old enough, and if so, whether or not it gives birth to a brand new bouncing baby virus.  The heath (how many bullets it can take) of a virus increases with age, to some max limit, it is the health system that updates this.  Through these different systems, the logic for a virus is encapsulated.  Rather than having one big fat class or chunk of code with all logic for a virus, it is distributed among these different systems.  Of these systems, `Age`, `Birth`, and `Health` are unique to the virus, but the `Movement` system is shared among several entities, the player, bullets, and bombs; which reduces overall code complexity.

### Powerup Entities

Powerups are a relatively simple entity.  They convey information about a new weapon for the player entity.  They only interact with the player, when the player touches (collides) one.  When this occurs, the type of powerup determines something about either the primary or secondary weapon to the player entity.  During the core game loop, a powerup is updated by the `Lifetime` and `AnimatedSprite` systems.  Some entities have a limited lifetime, when maxed out, they are removed.  The `Lifetime` system tracks this and invokes an `endOfLife` method associated with the entity's `Lifetime` component, along with removing those entities during the system update.

## Systems

As noted in the [Systems](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Systems.md) documentation, the systems help reduce overall code complexity by creating cohesive units of code.  One of the goals for systems is to reduce complexity by sharing of code among entities, having systems that can act on different kinds of entities, using the same logic.  A good example of this is the `Movement` system.  Entities don't have logic for movement, instead, entities are given to the `Movement` system and it acts on them.  The simplification of the code is found by looking at the first part of the `update` method...

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

This shows the different systems being invoked on the various types of entities, it is very simple.  The remainder of this method is checking to see if bullets have collides with viruses, removing bullets that have collides with viruses, telling the level to update, adding newly birthed viruses, and removing any dead viruses.

## Other Game Logic

As noted at the start, the game model is the _glue_ that holds all other game play related code together.  The following are support methods in this direction:

* `emitBullet`
* `emitBomb`
* `emitPowerup`
* `onVirusDeath`
* `onVirusBirth`
* `onPlayerDeath`

The `emitBullet` and `emitBomb` methods are used in a couple lambdas associated with the player keyboard control.  When the `startPlayer` method is called, keyboard handlers are registered for the primary and secondary fire controls.  When invoked, those handlers (lambdas) call the `emit` methods.  These methods simply add the appropriate entity to either the bullet of bomb entity set.

The `emitPowerup` is invoked from a lambda passed to the `Level`.  This method adds the powerup to the powerup collection.  It is the `Level` that contains the logic for when a powerup should be emitted, the `GameModel` only tracks the existence of the powerup.

`onVirusDeath` is called from the `update` method, for when a virus has been defeated.  The main thing this method does is to build the particle effect to indicate the death.  The other, key, item this method does is to check if this is the last virus, and if so, bring up the end of level status message.

`onVirusBirth` is invoked from a lambda that is passed to the `Birth` system.  When called, this method validates the max number of viruses for the level hasn't been hit.  If it hasn't, then the new virus is created and positioned based on the location of its parent.

Finally, `onPlayerDeath` is invoked during the `update` method, but from within the lambda that has been assigned to the `m_playerUpdate` function.  Its core responsibility is to create the particle effect for the player's death, and then take appropriate action based on how many player ships remain.

## Rendering

The `GameModel` knows how to render itself.  The core logic for rendering everything in the game is encapsulated in the various `renderers`, which results in quite simple rendering code in the `GameModel` class itself.  The following is the complete `render` method...

    void GameModel::render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
    {
        renderTarget.clear(sf::Color::Black);
        m_rendererBackground->render(renderTarget);

        m_rendererBullet->render(m_bullets, renderTarget);
        m_rendererBomb->render(m_bombs, renderTarget);
        m_rendererPowerup->render(m_powerups, renderTarget);
        m_rendererSarsCov2->render(m_viruses, renderTarget, elapsedTime);
        m_renderPlayer(renderTarget);
        m_rendererParticleSystem->render(m_sysParticle, renderTarget);
        m_rendererHUD->render(m_remainingNanoBots + 1, m_timePlayed, m_virusesKilled, renderTarget);
        m_rendererStatus->render(renderTarget);
    }

It is simply going through the different `renderers` and having them render the various parts of the game.  

~~One possible improvement, to further simplify this code would be to have the bullet, bomb, powerup, and virus renderers take a collection and have it iterate over the collection, rendering as appropriate.  While not necessary for this game, because performance isn't an issue, this could potentially allow for rendering performance improvements.  It would also be good to perform a small bit of logic that can be removed from the `GameModel`, and push it to the the `renderers`.~~

The above is Done!
