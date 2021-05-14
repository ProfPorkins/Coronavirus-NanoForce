# High-Level Overview

The high-level overview of the architecture looks like the following diagram.

![High Level Overview](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/High-Level-Overview.png)

The GameModel is the focus of the code, with most everything else in support of it.

## Game Design

Fundamentally, it is a variation on [Asteroids](https://en.wikipedia.org/wiki/Asteroids_%28video_game%29).  Highlighting a few of the differences:  The gameplay arena has borders, specifically it is played in petri dishes; y