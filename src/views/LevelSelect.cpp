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

#include "LevelSelect.hpp"

#include "GameModel.hpp"
#include "levels/LevelName.hpp"
#include "services/KeyboardInput.hpp"
#include "services/MouseInput.hpp"

namespace views
{
    bool LevelSelect::start()
    {
        MenuView::start();
        m_nextState = ViewState::LevelSelect;

        if (!m_initialized)
        {
            using namespace std::string_literals;

            //
            // Creating a text item of the whole alphabet so we can always find the
            // tallest character in the font and use that for the height to separate menu items.
            sf::Text alphabetTitle("ABCDEFGHIHKLMNOPQRTSUVWXYZ", *Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT), Configuration::get<std::uint8_t>(config::FONT_LEVEL_SELECT_TITLE_SIZE));
            alphabetTitle.scale(Configuration::getGraphics().getScaleUI());
            sf::Text alphabetItem("ABCDEFGHIHKLMNOPQRTSUVWXYZ", *Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT), Configuration::get<std::uint8_t>(config::FONT_LEVEL_SELECT_ITEM_SIZE));
            alphabetItem.scale(Configuration::getGraphics().getScaleUI());

            auto coords = Configuration::getGraphics().getViewCoordinates();
            auto top = -(coords.height / 2.0f) + (coords.height / 2.0f) * 0.35f;
            m_titleTraining.setPosition({ -(m_titleTraining.getRegion().width / 2.0f), top });
            top += alphabetTitle.getGlobalBounds().height * 1.5f;

            //
            // Prepare the level items
            // TODO: It would be nice to dynamically build these from the configuration, but I'm a little too lazy
            //       to do that for now.
            auto trainLevel1 = std::make_shared<ui::MenuItem>(
                0.0f, top,
                Configuration::get<std::string>(config::TRAINING_LEVEL1_NAME),
                Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT),
                sf::Color::White, sf::Color::Yellow, alphabetItem.getCharacterSize(),
                [this]()
                {
                    GameModel::selectLevel(levels::LevelName::Training1);
                    m_nextState = ViewState::GamePlay;
                });
            m_menuItems.push_back(trainLevel1);
            top += alphabetItem.getGlobalBounds().height * 1.5f;

            auto trainLevel2 = std::make_shared<ui::MenuItem>(
                0.0f, top,
                Configuration::get<std::string>(config::TRAINING_LEVEL2_NAME),
                Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT),
                sf::Color::White, sf::Color::Yellow, alphabetItem.getCharacterSize(),
                [this]()
                {
                    GameModel::selectLevel(levels::LevelName::Training2);
                    m_nextState = ViewState::GamePlay;
                });
            m_menuItems.push_back(trainLevel2);
            top += alphabetItem.getGlobalBounds().height * 1.5f;

            auto trainLevel3 = std::make_shared<ui::MenuItem>(
                0.0f, top,
                Configuration::get<std::string>(config::TRAINING_LEVEL3_NAME),
                Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT),
                sf::Color::White, sf::Color::Yellow, alphabetItem.getCharacterSize(),
                [this]()
                {
                    GameModel::selectLevel(levels::LevelName::Training3);
                    m_nextState = ViewState::GamePlay;
                });
            m_menuItems.push_back(trainLevel3);
            top += alphabetItem.getGlobalBounds().height * 1.5f;

            auto trainLevel4 = std::make_shared<ui::MenuItem>(
                0.0f, top,
                Configuration::get<std::string>(config::TRAINING_LEVEL4_NAME),
                Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT),
                sf::Color::White, sf::Color::Yellow, alphabetItem.getCharacterSize(),
                [this]()
                {
                    GameModel::selectLevel(levels::LevelName::Training4);
                    m_nextState = ViewState::GamePlay;
                });
            m_menuItems.push_back(trainLevel4);
            top += alphabetItem.getGlobalBounds().height * 1.5f;

            auto trainLevel5 = std::make_shared<ui::MenuItem>(
                0.0f, top,
                Configuration::get<std::string>(config::TRAINING_LEVEL5_NAME),
                Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT),
                sf::Color::White, sf::Color::Yellow, alphabetItem.getCharacterSize(),
                [this]()
                {
                    GameModel::selectLevel(levels::LevelName::Training5);
                    m_nextState = ViewState::GamePlay;
                });
            m_menuItems.push_back(trainLevel5);
            top += alphabetItem.getGlobalBounds().height * 2.0f;

            m_titlePatient.setPosition({ -(m_titlePatient.getRegion().width / 2.0f), top });
            top += alphabetTitle.getGlobalBounds().height * 1.5f;

            auto patientLevel1 = std::make_shared<ui::MenuItem>(
                0.0f, top,
                Configuration::get<std::string>(config::PATIENT_LEVEL1_NAME),
                Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT),
                sf::Color::White, sf::Color::Yellow, alphabetItem.getCharacterSize(),
                [this]()
                {
                    GameModel::selectLevel(levels::LevelName::Patient1);
                    m_nextState = ViewState::GamePlay;
                });
            m_menuItems.push_back(patientLevel1);
            top += alphabetItem.getGlobalBounds().height * 1.5f;

            auto patientLevel2 = std::make_shared<ui::MenuItem>(
                0.0f, top,
                Configuration::get<std::string>(config::PATIENT_LEVEL2_NAME),
                Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT),
                sf::Color::White, sf::Color::Yellow, alphabetItem.getCharacterSize(),
                [this]()
                {
                    GameModel::selectLevel(levels::LevelName::Patient2);
                    m_nextState = ViewState::GamePlay;
                });
            m_menuItems.push_back(patientLevel2);
            top += alphabetItem.getGlobalBounds().height * 1.5f;

            auto patientLevel3 = std::make_shared<ui::MenuItem>(
                0.0f, top,
                Configuration::get<std::string>(config::PATIENT_LEVEL3_NAME),
                Content::get<sf::Font>(content::KEY_FONT_LEVEL_SELECT),
                sf::Color::White, sf::Color::Yellow, alphabetItem.getCharacterSize(),
                [this]()
                {
                    GameModel::selectLevel(levels::LevelName::Patient3);
                    m_nextState = ViewState::GamePlay;
                });
            m_menuItems.push_back(patientLevel3);

            //
            // Go through the items and center everything horizontally
            for (auto&& item : m_menuItems)
            {
                item->setRegion({ -item->getRegion().width / 2.0f,
                                  item->getRegion().top,
                                  item->getRegion().width,
                                  alphabetItem.getGlobalBounds().height * 1.25f }); // TODO: This 1.25f should not be necessary!!
            }

            //
            // Select the first menu choice by default
            m_activeMenuItem = 0;
            m_menuItems[m_activeMenuItem]->setActive();

            m_initialized = true;
        }

        //
        // Get the keyboard inputs registered
        KeyboardInput::instance().registerKeyReleasedHandler("escape", [this]()
                                                             { m_nextState = ViewState::MainMenu; });
        KeyboardInput::instance().registerKeyReleasedHandler("down", [this]()
                                                             {
                                                                 m_menuItems[m_activeMenuItem]->setInactive();
                                                                 m_activeMenuItem = (static_cast<std::size_t>(m_activeMenuItem) + 1) % m_menuItems.size();
                                                                 m_menuItems[m_activeMenuItem]->setActive();
                                                             });
        KeyboardInput::instance().registerKeyReleasedHandler("up", [this]()
                                                             {
                                                                 m_menuItems[m_activeMenuItem]->setInactive();
                                                                 m_activeMenuItem--;
                                                                 if (m_activeMenuItem < 0)
                                                                 {

                                                                     m_activeMenuItem = static_cast<std::int8_t>(m_menuItems.size() - 1);
                                                                 }
                                                                 m_menuItems[m_activeMenuItem]->setActive();
                                                             });

        for (auto&& menuItem : m_menuItems)
        {
            menuItem->start();
        }

        //
        // Get the mouse inputs registered
        m_mouseMovedHandlerId = MouseInput::instance().registerMouseMovedHandler([this](math::Point2f point, const std::chrono::microseconds elapsedTime)
                                                                                 { onMouseMoved(point, elapsedTime); });
        m_mouseReleasedHandlerId = MouseInput::instance().registerMouseReleasedHandler([this](sf::Mouse::Button button, math::Point2f point, const std::chrono::microseconds elapsedTime)
                                                                                       { onMouseReleased(button, point, elapsedTime); });

        return true;
    }

    void LevelSelect::stop()
    {
        KeyboardInput::instance().unregisterKeyReleasedHandler("escape");
        KeyboardInput::instance().unregisterKeyReleasedHandler("up");
        KeyboardInput::instance().unregisterKeyReleasedHandler("down");

        for (auto&& menuItem : m_menuItems)
        {
            menuItem->stop();
        }

        MouseInput::instance().unregisterMouseMovedHandler(m_mouseMovedHandlerId);
        MouseInput::instance().unregisterMouseReleasedHandler(m_mouseReleasedHandlerId);
    }

    ViewState LevelSelect::update([[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        return m_nextState;
    }

    void LevelSelect::render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
    {
        MenuView::render(renderTarget, elapsedTime);

        for (auto&& item : m_menuItems)
        {
            item->render(renderTarget);
        }

        m_titleTraining.render(renderTarget);
        m_titlePatient.render(renderTarget);
    }

    void LevelSelect::onMouseMoved(math::Point2f point, [[maybe_unused]] std::chrono::microseconds elapsedTime)
    {
        for (decltype(m_menuItems.size()) item = 0; item < m_menuItems.size(); item++)
        {
            if (m_menuItems[item]->getRegion().contains(point))
            {
                if (!m_menuItems[item]->isActive())
                {
                    m_menuItems[m_activeMenuItem]->setInactive();
                    m_activeMenuItem = static_cast<std::int8_t>(item);
                    m_menuItems[m_activeMenuItem]->setActive();
                }
            }
        }
    }

    void LevelSelect::onMouseReleased(sf::Mouse::Button button, math::Point2f point, const std::chrono::microseconds elapsedTime)
    {
        for (auto&& item : m_menuItems)
        {
            item->onMouseReleased(button, point, elapsedTime);
        }
    }

} // namespace views
