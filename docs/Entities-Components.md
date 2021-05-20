# Entities & Components

Entities and components make up most of the game play items.  Entities include the player controlled ship, weapons, projectiles, bombs, powerups, and the viruses.  Entities are composed from Components.  Components include attributes like `Age`, `Damage`, `Drag`, `Health`, `Lifetime`, among others.  A single `Entity` may be composed of any number of components.

ECS Note: While this code uses the terms Entity, Component, and System, the architectural design is not that of a true [ECS](https://en.wikipedia.org/wiki/Entity_component_system).  There is some flavor of ECS in the design and code, but it isn't an ECS architecture.  If interested, see some of my other work that does following an ECS architectural design in my [Game Techniques](https://github.com/ProfPorkins/GameTech) repository.

## Components

Components are attributes of entities, having only state and no behaviors.  By that I mean a component doesn't have any logic associated with it, only data.  Components have their state modified either by the entity itself or by a system.  The components in this code are very much like components in an ECS architecture.

The following diagram provides an overview of the components in this code.

![Components Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Components.png)

All components are derived from the `Component` class.  The purpose of this class is to provide a common base type that can be referenced by the `Entity` class (described below) for any derived type.  This class has no data or functionality.  Some components have trivial state, such as `Damage`, while others have more complex (but not that complex) state such as `AnimatedSprite`.

The `Damage` component has only a `m_damage` value, which conveys the amount of damage from a weapon's projectile.  For this game, only the `Bullet` entity causes damage, therefore, it is only used by that entity.  The `Bomb` entity doesn't cause damage itself, instead it explodes into a ring of bullets and those carry damage.

The `AnimatedSprite` component represents the state for a, well, animated sprite.  Powerups have an animated sprite appearance, cycling through different images over time for their display.  This component is used to hold the image that has the various animation sub-images, how long to show each sub-image, which sub-image to display, and how long the current image has been displayed.  The state of this component is updated by the `AnimatedSprite` system.

## Entities

Entities are the game play items, with the exception of levels (described here [TODO: add link when it is written]).  Entities are composed of components and possibly having code that modifies their own state based on how the `GameModel` uses it.

ECS Note: Entities in this code are composed of components, as ECS describes, but they are not only composed of components.  Entities may have other state and logic that modifies their state, which is different from ECS where only systems have logic that modifies state.

The following diagram provides an overview of then entities in this code.

![Entities Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Entities.png)

All entities are derived from the `Entity` class; it is simply and only a container for components.  This class provides the support necessary for adding and removing components, along with checking to see if a component exists on an entity, along with obtaining a pointer to a specific component.

### `Entity` Class Notes

The [CTTI](https://github.com/Manu343726/ctti) library is used to obtain a unique type (at compile time) for the various components in the code.  This allows a component to be identified like an integer for use in containers such as a hash table; in fact, that is how it is primarily used in the code.  The set of components are stored using an `std::unordered_map`, as shown in the line below...

    std::unordered_map<ctti::unnamed_type_id_t, std::unique_ptr<components::Component>> m_components;

The CTTI generated id is the key, and a unique pointer to the component is the value.

Note the use of templated methods inside the non-templated `Entity` class.  This allows the compiler to specialize the methods for each type.

Also note `getComponent` returns a raw pointer to the component.  This is done because any _use_ of a component has nothing to say about ownership or lifetime, ownership and lifetime are exclusively controlled by the `Entity`.  This is why the components are stored as `std::unique_ptr`, rather than `std::shared_ptr`.

The class is derived from `std::enable_shared_from_this<Entity>` because the `Player` entity needs to obtain a shared pointer to itself in order to set the parent when a weapon is attached.

### Adding Components

It is the responsibility for an entity to add its own components, this is done in the constructor.  The following is the `Player` entity's constructor...

    Player::Player(Specification spec) :
        m_thrustRate(spec.thrustRate),
        m_rotateRate(spec.rotateRate),
        m_maxSpeed(spec.maxSpeed)
    {
        this->addComponent(std::make_unique<components::Position>(math::Point2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Orientation>(0.0f));
        this->addComponent(std::make_unique<components::Size>(math::Dimension2f(spec.size, spec.size)));
        this->addComponent(std::make_unique<components::Momentum>(math::Vector2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Drag>(spec.dragRate));

        //
        // This is a unique one, due to the way it works, handing it directly here, rather than a
        // fire-and-forget using the SoundBuffer.
        m_thrust.setBuffer(*Content::get<sf::SoundBuffer>(content::KEY_AUDIO_THRUST));
    }

As noted above, in addition to systems that can act upon entities to modify the state of their components, an entity can have additional logic.  The `Player` entity, for example, exposes the following methods that can be used by the game model...

* `getPrimaryWeapon`
* `getSecondaryWeapon`
* `applyPowerup`
* `rotateLeft`
* `rotateRight`
* `startThrust`
* `endThrust`

The keyboard system uses the rotate, thrust, and weapon methods to modify state in response to user input.  The game model uses the `applyPowerup` method when it detects the ship has touched a powerup.  The movement system takes care of updating the momentum and position of the entity, using the `Position`, `Orientation`, `Momentum`, and `Drag` components.

### Creating `Entity` Instances

There are two different ways instances of entities are created.  The first is through the typical creation through its constructor, used by most entities.  However, the `Player` instance is created using a static `create` method available.  The following is the `create` method for the `Player` class...

    std::shared_ptr<Player> Player::create()
    {
        Specification spec;
        spec.thrustRate = Configuration::get<double>(config::PLAYER_THRUST_RATE);
        spec.dragRate = Configuration::get<double>(config::PLAYER_DRAG_RATE) * misc::PER_MS_TO_US;
        spec.rotateRate = Configuration::get<float>(config::PLAYER_ROTATE_RATE);
        spec.maxSpeed = Configuration::get<float>(config::PLAYER_MAX_SPEED);
        spec.size = Configuration::get<float>(config::PLAYER_SIZE);

        auto player = std::make_shared<Player>(spec);

        player->attachPrimaryWeapon(std::make_shared<entities::WeaponGun>(config::ENTITY_WEAPON_BASIC_GUN));
        player->attachSecondaryWeapon(std::make_shared<entities::WeaponEmpty>());

        return player;
    }

Why did I do it this way for the `Player` class?  The `attachPrimaryWeapon` and `attachSecondaryWeapon` methods call `shared_from_this()`, which fails when called from a constructor.  When inside a constructor, the shared pointer isn't available.  Because of this, I decided to provide a static `create` method for this entity.
