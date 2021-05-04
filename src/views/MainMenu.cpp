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

#include "MainMenu.hpp"

namespace views
{
    bool MainMenu::start()
    {
        MenuView::start();
        // Need to ensure the next state stays the MainMenu before checking to see if
        // the view is already initialized.
        m_nextState = ViewState::MainMenu;
        if (m_initialized)
            return true;

        using namespace std::string_literals;

        //
        // Creating a text item of the whole alphabet so we can always find the
        // tallest character in the font and use that for the height to separate menu items.
        sf::Text alphabet("ABCDEFGHIHKLMNOPQRTSUVWXYZ", *Content::get<sf::Font>(content::KEY_FONT_MENU), Configuration::get<std::uint8_t>(config::FONT_MENU_SIZE));
        alphabet.scale(Configuration::getGraphics().getScaleUI());

        //
        // Prepare the menu items
        auto menuNewGame = std::make_shared<ui::MenuItem>(
            "New Game"s,
            Content::get<sf::Font>(content::KEY_FONT_MENU),
            sf::Color::White, sf::Color::Yellow, alphabet.getCharacterSize(),
            [this]() { m_nextState = ViewState::LevelSelect; });
        m_menuItems.push_back(menuNewGame);

        auto menuGraphicsOptions = std::make_shared<ui::MenuItem>(
            "Settings"s, Content::get<sf::Font>(content::KEY_FONT_MENU),
            sf::Color::White, sf::Color::Yellow, alphabet.getCharacterSize(),
            [this]() { m_nextState = ViewState::Settings; });
        m_menuItems.push_back(menuGraphicsOptions);

        auto menuAbout = std::make_shared<ui::MenuItem>(
            "About"s, Content::get<sf::Font>(content::KEY_FONT_MENU),
            sf::Color::White, sf::Color::Yellow, alphabet.getCharacterSize(),
            [this]() { m_nextState = ViewState::About; });
        m_menuItems.push_back(menuAbout);

        auto menuCredits = std::make_shared<ui::MenuItem>(
            "Credits"s, Content::get<sf::Font>(content::KEY_FONT_MENU), sf::Color::White,
            sf::Color::Yellow, alphabet.getCharacterSize(),
            [this]() { m_nextState = ViewState::Credits; });
        m_menuItems.push_back(menuCredits);

        auto menuExit = std::make_shared<ui::MenuItem>(
            "Quit"s, Content::get<sf::Font>(content::KEY_FONT_MENU), sf::Color::White,
            sf::Color::Yellow, alphabet.getCharacterSize(),
            [this]() { m_nextState = ViewState::Exit; });
        m_menuItems.push_back(menuExit);

        //
        // Go back through the menu items and center everything vertically and horizontally
        auto totalHeight = m_menuItems.size() * alphabet.getGlobalBounds().height * 1.5f;
        auto currentY = 0.0f - totalHeight / 2.0f;
        for (auto&& item : m_menuItems)
        {
            item->setRegion({ -item->getRegion().width / 2.0f,
                              currentY,
                              item->getRegion().width,
                              alphabet.getGlobalBounds().height * 1.25f }); // TODO: This 1.25f should not be necessary!!
            currentY += alphabet.getGlobalBounds().height * 1.5f;
        }

        //
        // Select the first menu choice by default
        m_activeMenuItem = 0;
        m_menuItems[m_activeMenuItem]->setActive();

        m_initialized = true;

        return true;
    }

    void MainMenu::signalKeyPressed([[maybe_unused]] sf::Event::KeyEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        switch (event.code)
        {
            case sf::Keyboard::Down:
                m_menuItems[m_activeMenuItem]->setInactive();
                m_activeMenuItem = (m_activeMenuItem + 1) % m_menuItems.size();
                m_menuItems[m_activeMenuItem]->setActive();
                break;
            case sf::Keyboard::Up:
                m_menuItems[m_activeMenuItem]->setInactive();
                m_activeMenuItem--;
                if (m_activeMenuItem < 0)
                    m_activeMenuItem = static_cast<std::int8_t>(m_menuItems.size() - 1);
                m_menuItems[m_activeMenuItem]->setActive();
                break;
            default:
                m_menuItems[m_activeMenuItem]->signalKeyPressed(event, elapsedTime);
                break;
        }
    }

    void MainMenu::signalMouseMoved(math::Point2f point, [[maybe_unused]] std::chrono::microseconds elapsedTime)
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

    void MainMenu::signalMouseReleased(sf::Mouse::Button button, math::Point2f point, const std::chrono::microseconds elapsedTime)
    {
        for (auto&& item : m_menuItems)
        {
            item->signalMouseReleased(button, point, elapsedTime);
        }
    }

    ViewState MainMenu::update([[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        return m_nextState;
    }

    void MainMenu::render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
    {
        MenuView::render(renderTarget, elapsedTime);

        for (auto&& item : m_menuItems)
        {
            item->render(renderTarget);
        }
    }
} // namespace views
