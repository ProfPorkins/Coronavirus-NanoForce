# Entities & Components

Entities and components make up most of the game play items.  Entities include the player controlled ship, weapons, projectiles, bombs, powerups, and the viruses.  Entities are composed from Components.  Components include attributes like `Age`, `Damage`, `Drag`, `Health`, `Lifetime`, among others.  A single `Entity` may be composed of any number of components.

ECS Note: While this code uses the terms Entity, Component, and System, the architectural design is not that of a true [ECS](https://en.wikipedia.org/wiki/Entity_component_system).  There is some flavor of ECS in the design and code, but it isn't an ECS architecture.  If interested, see some of my other work that does following an ECS architectural design in my Game Techniques repository [right here](https://github.com/ProfPorkins/GameTech).

## Components

![Components Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Components.png)

## Entities

![Entities Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Entities.png)
