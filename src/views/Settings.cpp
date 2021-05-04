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

#include "Settings.hpp"

#include "UIFramework/KeyboardOption.hpp"
#include "UIFramework/Selection.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <sstream>

namespace views
{
    static const auto HEADING_COLOR = sf::Color(51, 102, 255);

    bool Settings::start()
    {
        MenuView::start();
        // Need to ensure the next state stays the Settings before checking to see if
        // the view is already initialized.
        m_nextState = ViewState::Settings;
        if (m_initialized)
            return true;

        //
        // Locate the restart message to near the bottom of the view
        m_restart2.setPosition({ -(m_restart2.getRegion().width / 2.0f),
                                 (Configuration::getGraphics().getViewCoordinates().height / 2.0f) - m_restart2.getRegion().height * 2.0f });
        m_restart1.setPosition({ -(m_restart1.getRegion().width / 2.0f),
                                 m_restart2.getRegion().top - m_restart2.getRegion().height * 2.0f });
        m_restart1.hide();
        m_restart2.hide();

        addFullScreenOption();
        addResolutionOption();
        addMusicOption();
        addKeyboardOptions();

        m_activeOption = 0;
        m_options[m_activeOption]->setActive();

        m_initialized = true;

        return true;
    }

    void Settings::signalKeyPressed([[maybe_unused]] sf::Event::KeyEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        if (m_selectKey.isVisible())
        {
            m_options[m_activeOption]->signalKeyPressed(event, elapsedTime);
        }
        else
        {
            switch (event.code)
            {
                case sf::Keyboard::Down:
                    m_options[m_activeOption]->setInactive();
                    m_activeOption = (static_cast<std::size_t>(m_activeOption) + 1) % m_options.size();
                    // A little hack, special case for when resolution isn't visible
                    if (!m_options[m_activeOption]->isVisible())
                    {
                        m_activeOption++;
                    }
                    m_options[m_activeOption]->setActive();
                    break;
                case sf::Keyboard::Up:
                    m_options[m_activeOption]->setInactive();
                    m_activeOption--;
                    if (m_activeOption < 0)
                        m_activeOption = static_cast<std::int8_t>(m_options.size() - 1); // A little hack, special case for when resolution isn't visible
                    if (!m_options[m_activeOption]->isVisible())
                    {
                        m_activeOption--;
                    }
                    m_options[m_activeOption]->setActive();
                    break;
                case sf::Keyboard::Escape:
                    m_nextState = ViewState::MainMenu;
                    break;
                default:
                    m_options[m_activeOption]->signalKeyPressed(event, elapsedTime);
                    break;
            }
        }
    }

    void Settings::signalMouseMoved(math::Point2f point, [[maybe_unused]] std::chrono::microseconds elapsedTime)
    {
        for (decltype(m_options.size()) item = 0; item < m_options.size(); item++)
        {
            if (m_options[item]->getRegion().contains(point))
            {
                if (!m_options[item]->isActive())
                {
                    m_options[m_activeOption]->setInactive();
                    m_activeOption = static_cast<std::int8_t>(item);
                    m_options[m_activeOption]->setActive();
                }
            }
        }
    }

    void Settings::signalMouseReleased(sf::Mouse::Button button, math::Point2f point, const std::chrono::microseconds elapsedTime)
    {
        m_options[m_activeOption]->signalMouseReleased(button, point, elapsedTime);
    }

    ViewState Settings::update([[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        return m_nextState;
    }

    void Settings::render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
    {
        MenuView::render(renderTarget, elapsedTime);

        for (auto&& option : m_options)
        {
            option->render(renderTarget);
        }

        m_restart1.render(renderTarget);
        m_restart2.render(renderTarget);
        m_selectKey.render(renderTarget);
    }

    // --------------------------------------------------------------
    //
    // The full screen options of either Yes or No
    //
    // --------------------------------------------------------------
    void Settings::addFullScreenOption()
    {
        //
        // Doing the dirty deed and using 1/0 to represent true/false
        std::vector<std::tuple<std::string, std::uint8_t>> fullScreenOptions = { { "Yes", static_cast<std::uint8_t>(1) }, { "No", static_cast<std::uint8_t>(0) } };
        auto fullScreen = std::make_shared<ui::Selection>(
            (-0.30f * Configuration::getGraphics().getViewCoordinates().height),
            "Full Screen",
            fullScreenOptions,
            Configuration::get<bool>(config::GRAPHICS_FULL_SCREEN) ? 0 : 1,
            Content::get<sf::Font>(content::KEY_FONT_SETTINGS), HEADING_COLOR, sf::Color::Yellow, Configuration::get<std::uint8_t>(config::FONT_SETTINGS_ITEM_SIZE),
            [this](std::uint8_t option) {
                m_restart1.show();
                m_restart2.show();
                auto value = static_cast<bool>(option);
                Configuration::set<bool>(config::GRAPHICS_FULL_SCREEN, value);

                // If the full screen is true, then hide the resolution option, otherwise show it
                if (value)
                {
                    m_options[1]->hide();
                }
                else
                {
                    m_options[1]->show();
                }
            });
        m_options.push_back(fullScreen);
    }

    // --------------------------------------------------------------
    //
    // Have to query the system for the available (full screen) resolutions to
    // make those available for selection.  In reality, the game uses the
    // desktop resolution for full screen mode.  Therefore, the querying of
    // the full-screen modes is just to give the player something to
    // choose for windowed mode.
    //
    // --------------------------------------------------------------
    void Settings::addResolutionOption()
    {
        //
        // Query SFML for the possible video modes
        auto modes = sf::VideoMode::getFullscreenModes();

        // Reverse the order, because SFML gives them from "best to worst", and we
        // want the opposite of that.
        std::reverse(modes.begin(), modes.end());

        // Filter to only 24/32 bpp and no duplicate resolutions
        std::vector<sf::VideoMode> allowedModes;
        std::copy_if(modes.begin(), modes.end(), std::back_inserter(allowedModes), [&](sf::VideoMode mode) {
            if (mode.bitsPerPixel == 32 || mode.bitsPerPixel == 24)
            {
                bool unique = std::none_of(allowedModes.begin(), allowedModes.end(), [&mode](sf::VideoMode test) {
                    return mode.height == test.height && mode.width == test.width;
                });
                return unique;
            }

            return false;
        });
        // Finally, transform into a vector of tuples to be given to the option control
        // While doing this, identify which is the current configuration setting
        std::size_t initialSelection = 0;
        std::vector<std::tuple<std::string, std::uint8_t>> resolutionOptions;
        std::transform(allowedModes.begin(), allowedModes.end(), std::back_inserter(resolutionOptions),
                       [&](sf::VideoMode mode) {
                           std::stringstream str;
                           str << mode.width << " x " << mode.height;

                           // This checks for the matching video mode, but ignoring bpp for now
                           if (Configuration::get<std::uint16_t>(config::GRAPHICS_WIDTH) == mode.width && Configuration::get<std::uint16_t>(config::GRAPHICS_HEIGHT) == mode.height)
                           {
                               initialSelection = resolutionOptions.size();
                           }

                           return std::tuple<std::string, std::uint8_t>(str.str(), static_cast<std::uint8_t>(resolutionOptions.size()));
                       });

        // Now, actually create the option control
        auto resolution = std::make_shared<ui::Selection>(
            (-0.20f * Configuration::getGraphics().getViewCoordinates().height),
            "Resolution",
            resolutionOptions, initialSelection,
            Content::get<sf::Font>(content::KEY_FONT_SETTINGS), HEADING_COLOR, sf::Color::Yellow, Configuration::get<std::uint8_t>(config::FONT_SETTINGS_ITEM_SIZE),
            [allowedModes, this](std::uint8_t option) { // NOTE: allowedModes is captured by copy; see note in Option.hpp for why
                m_restart1.show();
                m_restart2.show();
                auto value = allowedModes[option];
                Configuration::set<std::uint16_t>(config::GRAPHICS_WIDTH, static_cast<std::uint16_t>(value.width));
                Configuration::set<std::uint16_t>(config::GRAPHICS_HEIGHT, static_cast<std::uint16_t>(value.height));
            });
        m_options.push_back(resolution);
        if (Configuration::get<bool>(config::GRAPHICS_FULL_SCREEN))
        {
            m_options[m_options.size() - 1]->hide();
        }
    }

    // --------------------------------------------------------------
    //
    // The play background music options of either Yes or No
    //
    // --------------------------------------------------------------
    void Settings::addMusicOption()
    {
        //
        // Doing the dirty deed and using 1/0 to represent true/false
        std::vector<std::tuple<std::string, std::uint8_t>> musicOptions = { { "Yes", static_cast<std::uint8_t>(1) }, { "No", static_cast<std::uint8_t>(0) } };
        auto music = std::make_shared<ui::Selection>(
            (-0.10f * Configuration::getGraphics().getViewCoordinates().height),
            "Background Music",
            musicOptions,
            Configuration::get<bool>(config::PLAY_BACKGROUND_MUSIC) ? 0 : 1,
            Content::get<sf::Font>(content::KEY_FONT_SETTINGS), HEADING_COLOR, sf::Color::Yellow, Configuration::get<std::uint8_t>(config::FONT_SETTINGS_ITEM_SIZE),
            [this](std::uint8_t option) {
                auto value = static_cast<bool>(option);
                Configuration::set<bool>(config::PLAY_BACKGROUND_MUSIC, value);
            });
        m_options.push_back(music);
    }

    // --------------------------------------------------------------
    //
    // Give the user the ability to change the keyboard inputs.
    //
    // --------------------------------------------------------------
    void Settings::addKeyboardOptions()
    {
        auto top = (0.05f * Configuration::getGraphics().getViewCoordinates().height);
        //
        // Locate the "press a key..." right above the keyboard input options
        m_selectKey.setPosition({ -(m_selectKey.getRegion().width / 2.0f), top });
        m_selectKey.hide();

        top += m_selectKey.getRegion().height * 1.25f;

        auto thrust = std::make_shared<ui::KeyboardOption>(
            0.0f, top,
            "Thrust", Configuration::get<std::string>(config::KEYBOARD_UP),
            Content::get<sf::Font>(content::KEY_FONT_SETTINGS), HEADING_COLOR, sf::Color::Yellow, Configuration::get<std::uint8_t>(config::FONT_SETTINGS_ITEM_SIZE),
            [this]() {
                m_selectKey.show();
            },
            [this](std::string key) {
                m_selectKey.hide();
                if (key != "esc")
                {
                    Configuration::set<std::string>(config::KEYBOARD_UP, key);
                }
            });

        auto rotateLeft = std::make_shared<ui::KeyboardOption>(
            -(Configuration::getGraphics().getViewCoordinates().width / 2.0f) * 0.35f,
            thrust->getRegion().getBottom(),
            "Rotate Left", Configuration::get<std::string>(config::KEYBOARD_LEFT),
            Content::get<sf::Font>(content::KEY_FONT_SETTINGS), HEADING_COLOR, sf::Color::Yellow, Configuration::get<std::uint8_t>(config::FONT_SETTINGS_ITEM_SIZE),
            [this]() {
                m_selectKey.show();
            },
            [this](std::string key) {
                m_selectKey.hide();
                if (key != "esc")
                {
                    Configuration::set<std::string>(config::KEYBOARD_LEFT, key);
                }
            });

        auto rotateRight = std::make_shared<ui::KeyboardOption>(
            (Configuration::getGraphics().getViewCoordinates().width / 2.0f) * 0.35f,
            thrust->getRegion().getBottom(),
            "Rotate Right", Configuration::get<std::string>(config::KEYBOARD_RIGHT),
            Content::get<sf::Font>(content::KEY_FONT_SETTINGS), HEADING_COLOR, sf::Color::Yellow, Configuration::get<std::uint8_t>(config::FONT_SETTINGS_ITEM_SIZE),
            [this]() {
                m_selectKey.show();
            },
            [this](std::string key) {
                m_selectKey.hide();
                if (key != "esc")
                {
                    Configuration::set<std::string>(config::KEYBOARD_RIGHT, key);
                }
            });

        auto firePrimary = std::make_shared<ui::KeyboardOption>(
            -(Configuration::getGraphics().getViewCoordinates().width / 2.0f) * 0.35f,
            rotateRight->getRegion().getBottom(),
            "Primary Fire", Configuration::get<std::string>(config::KEYBOARD_PRIMARY_FIRE),
            Content::get<sf::Font>(content::KEY_FONT_SETTINGS), HEADING_COLOR, sf::Color::Yellow, Configuration::get<std::uint8_t>(config::FONT_SETTINGS_ITEM_SIZE),
            [this]() {
                m_selectKey.show();
            },
            [this](std::string key) {
                m_selectKey.hide();
                if (key != "esc")
                {
                    Configuration::set<std::string>(config::KEYBOARD_PRIMARY_FIRE, key);
                }
            });

        auto fireSecondary = std::make_shared<ui::KeyboardOption>(
            (Configuration::getGraphics().getViewCoordinates().width / 2.0f) * 0.35f,
            rotateRight->getRegion().getBottom(),
            "Secondary Fire", Configuration::get<std::string>(config::KEYBOARD_SECONDARY_FIRE),
            Content::get<sf::Font>(content::KEY_FONT_SETTINGS), HEADING_COLOR, sf::Color::Yellow, Configuration::get<std::uint8_t>(config::FONT_SETTINGS_ITEM_SIZE),
            [this]() {
                m_selectKey.show();
            },
            [this](std::string key) {
                m_selectKey.hide();
                if (key != "esc")
                {
                    Configuration::set<std::string>(config::KEYBOARD_SECONDARY_FIRE, key);
                }
            });

        // Add them in this order, because it impacts the keyboard control order ast he
        // user navigates the options.
        m_options.push_back(thrust);
        m_options.push_back(rotateLeft);
        m_options.push_back(rotateRight);
        m_options.push_back(firePrimary);
        m_options.push_back(fireSecondary);
    }
} // namespace views
