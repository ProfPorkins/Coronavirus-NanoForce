# Entities & Components

Entities and components make up most of the game play items.  Entities include the player controlled ship, weapons, projectiles, bombs, powerups, and the viruses.  Entities are composed from Components.  Components include attributes like `Age`, `Damage`, `Drag`, `Health`, `Lifetime`, among others.  A single `Entity` may be composed of any number of components.

ECS Note: While this code uses the terms Entity, Component, and System, the architectural design is not that of a true [ECS](https://en.wikipedia.org/wiki/Entity_component_system).  There is some flavor of ECS in the design and code, but it isn't an ECS architecture.  If interested, see some of my other work that does following an ECS architectural design in my [Game Techniques](https://github.com/ProfPorkins/GameTech) repository.

## Components

Components are attributes of entities, having only state and no behaviors.  By that I mean a component doesn't have any logic associated with it, only data.  Components have their state modified either by the entity itself or by a system.  The components in this code are very much like components in an ECS architecture.

The following diagram provides an overview of the components.

![Components Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Components.png)

All components are derived from the `Component` class.  The purpose of this class is to provide a common base type that can be referenced by the `Entity` class (described below) for any derived type.  This class has no data or functionality.  Some components have trivial state, such as `Damage`, while others have more complex (but not that complex) state such as `AnimatedSprite`.

The `Damage` component has only a `m_damage` value, which conveys the amount of damage from a weapon's projectile.  For this game, only the `Bullet` entity causes damage, therefore, it is only used by that entity.  The `Bomb` entity doesn't cause damage itself, instead it explodes into a ring of bullets and those carry damage.

The `AnimatedSprite` component represents the state for a, well, animated sprite.  Powerups have an animated sprite appearance, cycling through different images over time for their display.  This component is used to hold the image that has the various animation sub-images, how long to show each sub-image, which sub-image to display, and how long the current image has been displayed.  The state of this component is updated by the `AnimatedSprite` system.

## Entities

![Entities Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Entities.png)
