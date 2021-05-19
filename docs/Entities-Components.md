# Entities & Components

Entities and components make up most of the game play items.  Entities include the player controlled ship, weapons, projectiles, bombs, powerups, and the viruses.  Entities are composed from Components.  Components include attributes like `Age`, `Damage`, `Drag`, `Health`, `Lifetime`, among others.  A single `Entity` may be composed of any number of components.

ECS Note: While this code uses the terms Entity, Component, and System, the architectural design is not that of a true [ECS](https://en.wikipedia.org/wiki/Entity_component_system).  There is some flavor of ECS in the design and code, but it isn't an ECS architecture.  If interested, see some of my other work that does following an ECS architectural design in my [Game Techniques](https://github.com/ProfPorkins/GameTech) repository.

## Components

Components are attributes of entities, having only state and no behaviors.  By that I mean a component doesn't have any logic associated with it, only data.  Components have their state modified either by the entity itself or by a system.  The components in this code are very much like components in an ECS architecture.

The following diagram provides an overview of components in this code.

![Components Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Components.png)

All components are derived from the `Component` class.  The purpose of this class is to provide a common base type that can be referenced by the `Entity` class (described below) for any derived type.  This class has no data or functionality.  Some components have trivial state, such as `Damage`, while others have more complex (but not that complex) state such as `AnimatedSprite`.

The `Damage` component has only a `m_damage` value, which conveys the amount of damage from a weapon's projectile.  For this game, only the `Bullet` entity causes damage, therefore, it is only used by that entity.  The `Bomb` entity doesn't cause damage itself, instead it explodes into a ring of bullets and those carry damage.

The `AnimatedSprite` component represents the state for a, well, animated sprite.  Powerups have an animated sprite appearance, cycling through different images over time for their display.  This component is used to hold the image that has the various animation sub-images, how long to show each sub-image, which sub-image to display, and how long the current image has been displayed.  The state of this component is updated by the `AnimatedSprite` system.

## Entities

Entities are the game play items, with the exception of levels (described here [TODO: add link when it is written]).  Entities are composed of components and possibly having code that modifies their own state based on how the `GameModel` uses it.

ECS Note: Entities in this code are composed components, as ECS describes, but they are not exclusively composed of components and having no logic.  Entities in this code may have other state and logic that modifies their state, which is different from ECS where only systems have logic that modifies state.

The following diagram provides an overview of entities in this code.

![Entities Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Entities.png)

All entities are derived from the `Entity` class; it is simply and only a container for components.  This class provides the support necessary for adding and removing components, along with checking to see if a component exists on an entity, along with obtaining a pointer to a specific component.

The [CTTI](https://github.com/Manu343726/ctti) library is used to obtain an unique type (at compile time) for the various components in the code.  This allows a component to be identified like an integer for use in containers like a hash table; in fact, that is how it is primarily used in the code.  The set of components are stored using an `std::unordered_map`, as shown in the line below...

    std::unordered_map<ctti::unnamed_type_id_t, std::unique_ptr<components::Component>> m_components;

The CTTI generated id is the key, and a unique pointer to the component is the value.

