# Coronavirus - Nano Force

This repository contains the source code for the [_Coronavirus - Nano Force_](https://store.steampowered.com/app/1341980/Coronavirus__Nano_Force/) game.  The source doesn't include the Steam specifics like the leader boards, achievements, and Steam overlay, but it is otherwise the complete source code.  In fact, while working on the documentation, I've made revisions to the code, that aren't in the release available on Steam.  None of the changes affect the game play, they are improvements to the implementation.  The reason for making it publicly available is to provide an example of a simple, but complete game.

* 5/23/2021 : The core documentation/writeup is complete.  There are other parts of the code and design that can be discussed (e.g., Particle System) and will likely be done as I have time and interest.
* 6/15/2021 : The keyboard and mouse refactors are complete, along with as much of the ECS refactoring that I have time to do for now.  I will update the documentation to reflect the changes as a result of these code refactors.  Unlikely I'll do more work on code.  A future game effort I intend to work on will be a full ECS design from the start, because I happen to like that model, along with incorporating the other items I have listed as possibilities for this code; C++ 20, localization, better data driven design.  The code for that effort will eventually be made public, but it could be a long time, as in years, before that game will be in any form to show.  I'll try to spin out core portions of the code for public availability as I make progress.

## Game Design

Fundamentally, it is a variation on [Asteroids](https://en.wikipedia.org/wiki/Asteroids_%28video_game%29).  Highlighting a few of the differences:  The gameplay arena has borders, specifically it is played in petri dishes; yes, it is meant to be silly.  A single shot does not destroy or split a virus, instead it requires multiple shots to surround and destroy a virus.  Viruses can spawn new viruses during the game.  There are four different weapons, three are obtained by collecting powerups.  There are no equivalents to the UFOs.

I freely admit I'm not a game designer and this game is not anything particularly amazing.  My purpose in writing the game is for my enjoyment, the writing of the game is _my_ fun.  An additional goal is to write a simple game and make the source code available.  By simple I mean a complete game, with some complexity; I don't mean something trivial like tic-tac-toe.

## Developer Setup and Build Instructions

The code is written in C++, uses the [SFML](https://www.sfml-dev.org/), [CTTI](https://github.com/Manu343726/ctti), and [RapidJson](https://github.com/Tencent/rapidjson) libraries.  It builds and runs on Windows using MSVC, Linux using g++, and macOS using llvm.  Additionally, the project utilizes [CMake](https://cmake.org/) to generate platform specific build systems.

Instructions for Windows and Linux are found at [this page](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Developer-Instructions.md).  My access to a macOS based computer is limited, therefore, I don't have a writeup for it yet.  But fundamentally, it is similar to the Linux instructions.

## Code Design

Links to documentation on various aspects of the architecture and code are found below.  Throughout the documentation you will find not only the description of the design and code, but also my commentary on why it is that why and how else I might like to do it.  Mostly I'm attempting to describe the code as it is, but I also want to communicate compromises made (typically to time) and other possibilities for how it might be different/better.

A non-goal for this code is to create a game _engine_.  The goal is to create a game, have fun doing so, and hopefully provide a reasonably well-written example; I recognize the result is not free from critiques for improvement, but see the answer to the first question below for context.  Some aspects of the code are agnostic to the game play and reusable in other games, but that is more an artifact of the architectural and code design rather than an effort to write a game engine.  Reusability within the game code itself is the first goal, reusability in other game projects is a secondary goal.  If I were to write another game, as indicated below, I might first start with the recommendations noted below, with a stronger goal towards more cross-game code reusability.  Once that is complete, then begin work on the new game.

* [High Level Architectural Overview](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/High-Level.md)
* [Startup and application/game loop](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Startup-GameLoop.md)
* [Entities & Components](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Entities-Components.md)
* [Systems](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Systems.md)
* [Particle System](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/ParticleSystem.md)
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

* (completed) ~~Refactor the keyboard and mouse input as described at the end of the [Systems](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Systems.md) documentation.~~
* Refactor the level management code to be fully data driven as suggested in the introduction to the [Levels](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/Levels.md) documentation.
* (completed) ~~Refactor the code to be closer to an ECS architecture.~~
  * This is done as much as I'm going to do.  Don't have the time to do more work on it.  The biggest remaining item is refactoring the `Player` entity, and doing that is a bigger effort than is obvious by looking at it.  I have to move on to other projects and real life.
* Once C++20 is well supported the three compilers used to build it (MSVC, g++, llvm), take advantage of the new language capabilities where/if it makes sense.  In particular, I'm thinking of refactoring to use modules to give myself experience with that capability.
* Add support for localization.  It would be an interesting exercise to revise the code to handle multiple languages.
* [Gyruss](https://en.wikipedia.org/wiki/Gyruss) was a favorite game of mine back in the day.  Have considered a new version of the game design that follows that style of gameplay.
