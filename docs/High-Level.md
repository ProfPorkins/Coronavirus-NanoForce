# High-Level Overview

The high-level overview of the architecture looks like the following diagram.

![High Level Overview](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/High-Level-Overview.png)

The GameModel is the focus of the code, with most everything else in support of it.  The game model is composed of the current level, all entities currently in the game, the systems used to update the entities, the renderers used to, well, render the game entities, and other logic used to provide the gameplay.  At 600+ lines of code, it is the largest code file; I'm not proud it is that large, but it isn't (yet) problematic.

## Entities & Components

## Systems

## Renderers

## Levels
