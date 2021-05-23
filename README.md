# Coronavirus - Nano Force

This repository contains the source code for the _Coronavirus - Nano Force_ game.  The source doesn't include the Steam specifics like the leader boards, achievements, and Steam overlay, but it is otherwise the complete source code.  The reason for making it publicly available is to provide an example of a simple, but complete game.

Currently (5/7/2021) there isn't much writeup, but it is coming.  Over the next few weeks I plan to provide some detail on the overall design and discussion of the implementation.

## Game Design

Fundamentally, it is a variation on [Asteroids](https://en.wikipedia.org/wiki/Asteroids_%28video_game%29).  Highlighting a few of the differences:  The gameplay arena has borders, specifically it is played in petri dishes; yes, it is meant to be silly.  A single shot does not destroy or split a virus, instead it requires multiple shots to surround and destroy virus.  Viruses can spawn new viruses during the game.  There are four different weapons, three are obtained by collecting powerups.  There are no equivalents to the UFOs.

I freely admit I'm not a game designer and this game is not anything particularly amazing.  My purpose in writing the game is for my enjoyment, the writing of the game is _my_ fun.  An additional goal is to write a simple game and make the source code available.  By simple I mean a complete game, with some complexity; I don't mean something trivial like tic-tac-toe.

## Developer Setup and Build Instructions

The code is written in C++, uses the [SFML](https://www.sfml-dev.org/), [CTTI](https://github.com/Manu343726/ctti), and [RapidJson](https://github.com/Tencent/rapidjson) libraries.  It builds and runs on Windows using MSVC, Linux using g++, and macOS using llvm.  Additionally, the project utilizes [CMake](https://cmake.org/) to generate platform specific build systems.

Instructions for Windows and Linux are found at [this page](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Developer-Instructions.md).

## Code Design

Links to documentation on various aspects of the architecture and code are found below.  Throughout the documentation you will find not only the description of the design and code, but also my commentary on why it is that why and how else I might like to do it.  Mostly I'm attempting to describe the code as it is, but I also want to communicate compromises made (typically to time) other possibilities for how it might be different/better.

* [High Level Architectural Overview](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/High-Level.md)
* [Startup and application/game loop](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Startup-GameLoop.md)
* [Entities & Components](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Entities-Components.md)
* [Systems](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Systems.md)
* [Renderers](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Renderers.md)
* [Levels](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Levels.md)
* [Game Model](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/GameModel.md)

A few answers to questions some might have:

* Does it work? Yes
* Could the design and implementation be improved? Yes
* Does it use your favorite architectural design? No
* Does it use your favorite C++ features? No
* Does it use your favorite third-party library? No
* Why didn't you use Unity, Unreal, or another game engine? I enjoy writing the lower level systems myself, that is part of the _fun_ for me.  Those are incredible tools, just not what I'm looking to do for this kind of hobby project.
* Why didn't you do XYZ this other way in CMake? My CMake skills are only so so

## Possible Future Revisions

The following are ideas I have about revisions to the implementation I may (or may not) make in the future.

* Refactor the keyboard and mouse input as described at the end of the [Systems](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Systems.md) documentation.
* Refactor the level management code to be full data driver as suggested in the introduction to the [Levels](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Levels.md) documentation.
* Refactor the code to be closer to an ECS architecture.
* Once C++20 is well supported the three compilers used to build it (MSVC, g++, llvm), take advantage of the new language capabilities where/if it makes sense.  In particular, I'm thinking of refactoring to use modules to give myself experience with that capability.
* Add support for localization.  It would be an interesting exercise to revise the code to handle multiple languages.
* [Gyruss](https://en.wikipedia.org/wiki/Gyruss) was a favorite game of mine back in the day.  Have considered a new version of the game design that follows that style of gameplay.
