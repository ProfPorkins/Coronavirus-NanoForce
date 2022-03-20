/*
Copyright (c) 2021 James Dean Mathias

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"
#include "services/ContentKey.hpp"
#include "services/KeyboardInput.hpp"
#include "services/MouseInput.hpp"
#include "services/SoundPlayer.hpp"
#include "views/About.hpp"
#include "views/Credits.hpp"
#include "views/Gameplay.hpp"
#include "views/LevelSelect.hpp"
#include "views/MainMenu.hpp"
#include "views/Settings.hpp"
#include "views/View.hpp"
#include "views/ViewState.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

//
// This should not be in the Configuration.hpp header because client/server
// will have different configuration files.
const std::string CONFIG_SETTINGS_FILENAME = "client.settings.json";
const std::string CONFIG_DEVELOPER_FILENAME = "client.developer.json";

// --------------------------------------------------------------
//
// Read the json config file, then hand it off to be parsed and
// made usable by the application.
//
// --------------------------------------------------------------
auto readConfiguration()
{
    // Reference: https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    // Using Jerry's answer, because it was benchmarked to be quite fast, even though the config files are small.
    std::ifstream inSettings(CONFIG_SETTINGS_FILENAME);
    std::stringstream bufferSettings;
    bufferSettings << inSettings.rdbuf();
    inSettings.close();

    std::stringstream bufferDeveloper;
    std::ifstream inDeveloper(CONFIG_DEVELOPER_FILENAME);
    if (inDeveloper)
    {
        bufferDeveloper << inDeveloper.rdbuf();
        inDeveloper.close();
    }

    return Configuration::instance()->initialize(bufferSettings.str(), bufferDeveloper.str());
}

// --------------------------------------------------------------
//
// Save the current configuration to the config file.
//
// --------------------------------------------------------------
void saveConfiguration()
{
    auto json = Configuration::instance()->serialize();
    std::ofstream ofFile(CONFIG_SETTINGS_FILENAME);
    ofFile << json;
    ofFile.close();
}

// --------------------------------------------------------------
//
// Based on configuration settings and what the system is actually
// capable of doing, create the appropriate rendering window.
//
// --------------------------------------------------------------
std::shared_ptr<sf::RenderWindow> prepareWindow()
{
    //
    // Create the window : The settings parameter isn't necessary for what I'm doing, but leaving it here for reference
    sf::ContextSettings settings;

    int style = sf::Style::None;
    if (Configuration::get<bool>(config::GRAPHICS_FULL_SCREEN))
    {
        style = sf::Style::Fullscreen;
        //
        // Verify the config resolution is an available full-screen mode, if not, select
        // the first full-screen resolution by default;
        /*auto modes = sf::VideoMode::getFullscreenModes();
        auto isValidMode = std::any_of(modes.begin(), modes.end(),
                                       [](auto mode) {
                                           return mode.width == Configuration::get<std::uint16_t>(config::GRAPHICS_WIDTH) &&
                                                  mode.height == Configuration::get<std::uint16_t>(config::GRAPHICS_HEIGHT) &&
                                                  mode.bitsPerPixel == Configuration::get<std::uint8_t>(config::GRAPHICS_BPP);
                                       });
        if (!isValidMode)
        {
            auto mode = *modes.begin();
            Configuration::set<std::uint16_t>(config::GRAPHICS_WIDTH, static_cast<std::uint16_t>(mode.width));
            Configuration::set<std::uint16_t>(config::GRAPHICS_HEIGHT, static_cast<std::uint16_t>(mode.height));
            Configuration::set<std::uint8_t>(config::GRAPHICS_BPP, static_cast<std::uint8_t>(mode.bitsPerPixel));
        }*/

        //
        // By definition, use whatever resolution the desktop is in
        auto desktop = sf::VideoMode::getDesktopMode();
        Configuration::getGraphics().setResolution({ desktop.width, desktop.height });
        Configuration::getGraphics().setBpp(desktop.bitsPerPixel);
    }
    else
    {
        style = sf::Style::Titlebar | sf::Style::Close;
        Configuration::getGraphics().setResolution({ Configuration::get<std::uint16_t>(config::GRAPHICS_WIDTH),
                                                     Configuration::get<std::uint16_t>(config::GRAPHICS_HEIGHT) });
        Configuration::getGraphics().setBpp(Configuration::get<std::uint8_t>(config::GRAPHICS_BPP));
    }
    auto window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(
            Configuration::getGraphics().getResolution().width,
            Configuration::getGraphics().getResolution().height,
            Configuration::getGraphics().getBpp()),
        "Coronavirus - Nano Force",
        style,
        settings);

    window->setVerticalSyncEnabled(Configuration::get<bool>(config::GRAPHICS_VSYNC));

    return window;
}

void prepareView(std::shared_ptr<sf::RenderWindow> window)
{
    //
    // The aspect ratio is needed in order to know how to organize the viewport
    // for the "game play" area used for the demonstrations.
    auto aspectRatio = static_cast<float>(window->getSize().x) / window->getSize().y;

    //
    // Get the view coordinates to be square, based on the aspect ratio of the window
    if (aspectRatio > 1.0)
    {
        Configuration::getGraphics().setViewCoordinates(
            { Configuration::getGraphics().getViewCoordinates().width * aspectRatio,
              Configuration::getGraphics().getViewCoordinates().height });
    }
    else
    {
        Configuration::getGraphics().setViewCoordinates(
            { Configuration::getGraphics().getViewCoordinates().width,
              Configuration::getGraphics().getViewCoordinates().height * (1.0f / aspectRatio) });
    }

    // Have to set the view after preparing it
    sf::View view({ 0.0f, 0.0f }, Configuration::getGraphics().getViewCoordinates());
    window->setView(view);
}

bool loadMenuContent()
{
    std::atomic_bool success{ true };
    std::condition_variable eventContent;
    std::mutex mutexContent;

    auto onComplete = [&]([[maybe_unused]] std::string key)
    {
        eventContent.notify_one();
    };
    auto onError = [&]([[maybe_unused]] std::string filename)
    {
        success = false;
        eventContent.notify_one();
    };

    //
    // Get the fonts loaded
    Content::load<sf::Font>(content::KEY_FONT_TITLE, Configuration::get<std::string>(config::FONT_TITLE_FILENAME), nullptr, onError);
    Content::load<sf::Font>(content::KEY_FONT_MENU, Configuration::get<std::string>(config::FONT_MENU_FILENAME), nullptr, onError);
    Content::load<sf::Font>(content::KEY_FONT_CREDITS, Configuration::get<std::string>(config::FONT_CREDITS_FILENAME), nullptr, onError);
    Content::load<sf::Font>(content::KEY_FONT_GAME_STATUS, Configuration::get<std::string>(config::FONT_GAME_STATUS_FILENAME), nullptr, onError);
    Content::load<sf::Font>(content::KEY_FONT_SETTINGS, Configuration::get<std::string>(config::FONT_SETTINGS_FILENAME), nullptr, onError);
    Content::load<sf::Font>(content::KEY_FONT_LEVEL_SELECT, Configuration::get<std::string>(config::FONT_LEVEL_SELECT_FILENAME), nullptr, onError);
    Content::load<sf::Font>(content::KEY_FONT_GAMEPLAY, Configuration::get<std::string>(config::FONT_GAMEPLAY_FILENAME), nullptr, onError);

    //
    // Get the menu audio activate and accept clips loaded
    Content::load<sf::SoundBuffer>(content::KEY_MENU_ACTIVATE, Configuration::get<std::string>(config::AUDIO_MENU_ACTIVATE), nullptr, onError);
    Content::load<sf::SoundBuffer>(content::KEY_MENU_ACCEPT, Configuration::get<std::string>(config::AUDIO_MENU_ACCEPT), nullptr, onError);

    //
    // Get the background image loaded
    Content::load<sf::Texture>(content::KEY_IMAGE_MENU_BACKGROUND, Configuration::get<std::string>(config::IMAGE_MENU_BACKGROUND), onComplete, onError);

    //
    // Use an efficient wait for the initial content to finish loading
    std::unique_lock<std::mutex> lock(mutexContent);
    eventContent.wait(lock);
    return success;
}

// --------------------------------------------------------------
//
// This is my hack to reset the application window whenever the user
// changes the resolution or full/window options.
//
// The way I am having to completely reset the configuration completely is pretty bad,
// but I have struggled with SFML and getting the UI to render correctly based on
// different resolutions.  Completely resetting the configuration was the path of least
// resistance to accomplish the task at hand.  In a future program, I'll look into redoing
// how I use the SFML to get things rendered and hopefully also fix this at that time...or
// more likely, not use the SFML, just build from the ground up myself.
//
// --------------------------------------------------------------
std::shared_ptr<sf::RenderWindow> resetWindow(std::unordered_map<views::ViewState, std::shared_ptr<views::View>>& views, std::shared_ptr<views::View>& view, std::shared_ptr<sf::RenderWindow> window)
{
    Configuration::getGraphics().setRestart(false);
    saveConfiguration();
    Configuration::instance()->reset();
    readConfiguration();

    //
    // Shutdown the current window and view
    view->stop();
    window->setActive(false);
    window->close();

    //
    // Create and activate the window for rendering on the main thread
    auto newWindow = prepareWindow();
    prepareView(newWindow);
    newWindow->setActive(true);

    //
    // Construct the different views the game may show, then get
    // the initial view set and ready to run.
    views.clear();
    views[views::ViewState::MainMenu] = std::make_shared<views::MainMenu>();
    views[views::ViewState::LevelSelect] = std::make_shared<views::LevelSelect>();
    views[views::ViewState::GamePlay] = std::make_shared<views::Gameplay>();
    views[views::ViewState::Credits] = std::make_shared<views::Credits>();
    views[views::ViewState::About] = std::make_shared<views::About>();
    views[views::ViewState::Settings] = std::make_shared<views::Settings>();

    return newWindow;
}

int main()
{
    //
    // Read the configuration file so we can get things setup based on that
    if (!readConfiguration())
    {
        std::cout << "Failure in reading configuration file..." << std::endl;
        exit(0);
    }

    //
    // The SoundPlayer singleton needs to be specifically initialized
    SoundPlayer::instance().initialize();

    //
    // The Content singleton needs to be specifically initialized
    Content::instance().initialize();
    if (!loadMenuContent())
    {
        Content::instance().terminate();
        exit(0);
    }

    //
    // The KeyboardInput & MouseInput singletons need to be specifically initialized
    KeyboardInput::instance().initialize();
    MouseInput::instance().initialize();

    //
    // Create and activate the window for rendering on the main thread
    auto window = prepareWindow();
    prepareView(window);
    window->setActive(true);

    //
    // Construct the different views the game may show, then get
    // the initial view set and ready to run.
    std::unordered_map<views::ViewState, std::shared_ptr<views::View>> views;
    views[views::ViewState::MainMenu] = std::make_shared<views::MainMenu>();
    views[views::ViewState::LevelSelect] = std::make_shared<views::LevelSelect>();
    views[views::ViewState::GamePlay] = std::make_shared<views::Gameplay>();
    views[views::ViewState::Credits] = std::make_shared<views::Credits>();
    views[views::ViewState::About] = std::make_shared<views::About>();
    views[views::ViewState::Settings] = std::make_shared<views::Settings>();

    views::ViewState viewState = Configuration::get<bool>(config::DEVELOPER_MAIN_MENU) ? views::ViewState::MainMenu : views::ViewState::GamePlay;
    if (viewState == views::ViewState::GamePlay)
    {
        window->setMouseCursorVisible(false);
    }
    std::shared_ptr<views::View> view = views[viewState];
    if (!view->start())
    {
        std::cout << "Initial view failed to initialize, terminating..." << std::endl;
        exit(0);
    }

    //
    // Grab an initial time-stamp to get the elapsed time working
    auto previousTime = std::chrono::system_clock::now();

    //
    // Get the Window loop running.  The game loop runs inside of this loop
    bool running = true;
    while (running)
    {
        //
        // Figure out the elapsed time in microseconds.  Need this to pass on to
        // the game model.
        auto currentTime = std::chrono::system_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
        previousTime = currentTime;

        // Handle all pending Windows events
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }

            //
            // This is essentially the process input stage of the game loop
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                {
                    KeyboardInput::instance().signalKeyPressed(event.key, elapsedTime, currentTime);
                }
                break;
                case sf::Event::KeyReleased:
                {
                    KeyboardInput::instance().signalKeyReleased(event.key, elapsedTime, currentTime);
                }
                break;
                case sf::Event::MouseMoved:
                {
                    auto viewCoords = window->mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }, window->getView());
                    MouseInput::instance().signalMouseMoved({ viewCoords.x, viewCoords.y });
                }
                break;
                case sf::Event::MouseButtonPressed:
                {
                    auto viewCoords = window->mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, window->getView());
                    MouseInput::instance().signalMousePressed(event.mouseButton.button, { viewCoords.x, viewCoords.y });
                }
                break;
                case sf::Event::MouseButtonReleased:
                {
                    auto viewCoords = window->mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, window->getView());
                    MouseInput::instance().signalMouseReleased(event.mouseButton.button, { viewCoords.x, viewCoords.y });
                }
                break;
                default:
                    // This is here to eliminate a bunch of compiler warnings related to events not handled in this switch statement
                    break;
            }
        }

        //
        // Execute the standard game loop steps

        // Step 1: Process Input
        KeyboardInput::instance().update(elapsedTime);
        MouseInput::instance().update(elapsedTime);

        // Step 2: Update
        auto nextViewState = view->update(elapsedTime, currentTime);

        // Step 3: Render
        view->render(*window, elapsedTime);

        //
        // BUT, we still wait until here to display the window...this is what actually
        // causes the rendering to occur.
        window->display();

        //
        // Constantly check to see if the view should change.
        if (nextViewState != viewState)
        {
            if (nextViewState == views::ViewState::Exit)
            {
                running = false;
            }
            else
            {
                view->stop();
                view = views[nextViewState];
                view->start();
                viewState = nextViewState;
                if (viewState == views::ViewState::GamePlay)
                {
                    window->setMouseCursorVisible(false);
                }
                else
                {
                    window->setMouseCursorVisible(true);
                }
            }
        }
        //
        // Constantly check to see if the window should be restarted, due to a graphics option having changed.
        if (Configuration::getGraphics().restart())
        {
            window = resetWindow(views, view, window);

            view = views[viewState];
            if (!view->start())
            {
                std::cout << "Failed to restart in the newly selection screen/resolution settings, terminating..." << std::endl;
                exit(0);
            }
        }
    }

    // Gracefully shut things down
    saveConfiguration();
    SoundPlayer::instance().terminate();
    Content::instance().terminate();

    // Do this after shutting down the Content singleton so that all textures
    // are released by the various shared pointers.  This cleans up some errors
    // that otherwise show up as SFML is shutting down.
    window->setActive(false);
    window->close();

    return 0;
}
