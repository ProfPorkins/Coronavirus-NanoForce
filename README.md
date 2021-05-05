# Coronavirus - Nano Force

This repository contains the source code for the *Coronavirus - Nano Force* game.  The reason for making it publicly available is to provide an example of a simple, but complete game.

Currently (5/5/2021) there isn't much writeup, but it is coming.  Over the next few weeks I plan to provide some detail on the overall design and discussion of the implementation.

## Game Design

Fundamentally, it is a variation on [Asteroids](https://en.wikipedia.org/wiki/Asteroids_%28video_game%29).  Highlighting a few of the differences.  The gameplay arena has borders, specifically it is played in petri dishes.  A single shot does not destroy or split a virus (an asteroid), instead it requires multiple shots to surround and destroy virus.  Viruses can spawn new viruses during the game.  There are two types of weapons, some of which are obtained by collecting powerups.  Finally, there are no equivalents to the UFOs.

I freely admit I'm not a game designer and this game is not anything particularly amazing.  My purpose in writing the game was for my own enjoyment, the writing of the game was *my* fun.  An additional goal was to write a simple game and make the source code available.  By simple I mean a complete game, with some complexity required; I don't mean something as simple as tic-tac-toe.

## Building The Game

The code is written in C++, uses the [SFML](https://www.sfml-dev.org/), [CTTI](https://github.com/Manu343726/ctti), and [RapidJson](https://github.com/Tencent/rapidjson) libraries.  It builds and runs on Windows using MSVC, on Linux using g++, and macOS using llvm.  Additionally, the project utilizes [CMake](https://cmake.org/) to generate platform specific build systems.

## Code Design

A few answers to questions

* Does it work? Yes
* Could the design and implementation be improved? Yes
* Does it use your favorite architectural design? No
* Does it use your favorite C++ features? No
* Does it use your favorite third-party library? No
* Why didn't you do XYZ this other way in CMake? Because I barely know how to use CMake
* Why didn't you use XYZ for the build system? Because I didn't
* Why aren't there any unit or other types tests? I wrote this for fun, writing unit tests doesn't fall into my category of coding activities I consider fun

## Possible Future Revisions

The following are ideas I have about revisions to the implementation I may (or may not) make in the future.

* Refactor the code to be closer to an ECS architecture.
* Once C++20 is well supported the three compilers used to build it (MSVC, g++, llvm), take advantage of the new language capabilities where/if it makes sense.  In particular, I'm thinking of refactoring to use modules to give myself experience with that capability.
* [Gyruss](https://en.wikipedia.org/wiki/Gyruss) was a favorite game of mine back in the day.  Have considered a new version of the game design that follows that style of gameplay.
