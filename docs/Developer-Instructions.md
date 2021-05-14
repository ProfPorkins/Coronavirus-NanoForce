# Developer Setup & Compiling Instructions

This page describes how to setup the development environment you need to compile and run the game.  Instructions for both Windows and Linux are provided.  Familiarity with C++ development on either Windows or Linux is expected.

## Windows

The following development tools must be installed:

* Visual Studio 2019
* git
* CMake

The next step is to clone and build the game.  The following steps can be used to do this.

1. Clone the repository.  This can be done using a command line or a tool with a GUI.  I personally use GitExtensions, but do most things from the command line inside of it.  The repository is located at: https://github.com/ProfPorkins/Coronavirus-NanoForce.git
1. Initialize the submodules: `git submodule update --init --recursive`
1. Run the CMake GUI
   1. For the **Where is the source code** field navigate to the `src` folder of the code.  For example **C:/Users/username/Desktop/Coronavirus-NanoForce/src**
   1. For the **Where to build the binaries** use the same pathname from above, but add a `/build` folder to it.  For example **C:/Users/username/Desktop/Coronavirus-NanoForce/src/build**
   1. Press the **Configure** button.
   1. Ensure the **Grouped** checkbox in the CMake GUI is checked.  This aids in the next few steps to turn on/off a few default settings for the libraries.
   1. Open the **gtest** dropdown in the CMake GUI and ensure all checkboxes are unchecked
   1. Open the **SFML** dropdown in the CMake GUI and ensure only the following checkboxes are checked
      * `SFML_BUILD_AUDIO`
      * `SFML_BUILD_GRAPHICS`
      * `SFML_BUILD_NETWORK`
      * `SFML_BUILD_WINDOW`
   1. Open the **RAPIDJSON** dropdown in the CMake GUID and ensure only the following checkbox is checked
      * `RAPID_JSON_BUILD_CXX17`
   1. Open the **CTTI** dropdown in the CMake GUI and ensure all checkboxes are unchecked
   1. Open the **BUILD** dropdown in the CMake GUID and ensure only the following checkbox is checked
      * `BUILD_SHARED_LIBS`
   1. Press **Configure** again.
   1. Press the **Generate** button.
1. Navigate to the `/build` folder
1. Double-click on the `CoronavirusNF.sln` file.  Alternatively start Visual Studio and open this solution file.
1. Open the context menu (usually right-mouse click) over **CoronavirusNF** and set it as the startup project.
1. Build the solution.
1. From within Visual Studio, run the project
1. Enjoy!

If you want to run the executable directly, a few more steps are necessary

1. Copy the `/assets` folder into the `/build/Debug` (or `/build/Release`) folder
1. Copy the `client.settings.json` file into the `/build/Debug` (or `/build/Release`) folder
1. Inside the `/build/Debug` (or `/build/Release`) folder, run `CoronavirusNF.exe`
1. Enjoy!

Why don't I have CMake copy these files automatically?  Believe me, I have tried, but failed!  I have yet to figure out how to write the instructions necessary to copy into the `/Debug` or `/Release` folders, as appropriate, following a build or configure.  If anyone is willing to help me understand how to do that, I'm open to it.

## Linux

The following instructions are valid for Ubuntu 20.04.  Other Linux distributions may require different steps, especially those not Debian based and without the apt package manager.

The following development tools must be installed:

* git
* g++ - A version that supports C++ 17 is required
* CMake
* (optional) clang-format
* The following dev libraries may need to be installed: `sudo apt install [lib name]`
  * libx11-dev
  * xorg-dev
  * libgl1-mesa-dev
  * libopenal-dev
  * libvorbis-dev
  * libflac-dev
  * libudev-dev

The next step is to clone and build the game.  The following steps can be used to do this.

1. Clone the repository: `git clone https://github.com/ProfPorkins/Coronavirus-NanoForce.git`
1. Navigate into the `Coronavirus-NanoForce` folder: cd `Coronavirus-NanoForce`
1. Initialize the submodules: `git submodule update --init --recursive`
1. Navigate into the `src` folder: `cd src`
1. Create a sub-folder named 'build': `mkdir build`
1. Navigate into this folder: `cd build`
1. Use CMake to create the makefiles: `cmake ..`
1. Once CMake has completed, use make to build the example: `make`
1. Start the game: `./CoronavirusNF`
1. Enjoy!
