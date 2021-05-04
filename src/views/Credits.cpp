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

#include "Credits.hpp"

#include "Configuration.hpp"

namespace views
{
    bool Credits::start()
    {
        MenuView::start();
        // Need to ensure the next state stays the Credits view
        m_nextState = ViewState::Credits;

        //
        // Wipe everything out and reset the credits every time
        m_credits.clear();

        //
        // The credits are a scrolling list of text items that identify who and what are
        // responsible for different parts of the game.
        auto top = (0.50f * Configuration::getGraphics().getViewCoordinates().height);
        top = addCredit("Developer", "Daniel Snow", top);
        top = addCredit("Simple and Fast Multimedia Library", "https://www.sfml-dev.org", top);
        top = addCredit("RapidJSON", "https://rapidjson.org", top);
        top = addCredit("CTTI", "https://github.com/Manu343726/ctti", top);
        top = addCredit("Shojumaru Font", "Astigmatic", top);

        top = addCredit("Background Images", "PHIL CDC - phil.cdc.gov", top);
        top = addCredit("Petri Disk Images", "PHIL CDC - phil.cdc.gov", top);
        top = addCredit("SARS COV 2 Image", "PHIL CDC - phil.cdc.gov", top);
        top = addCredit("Other Images", "PHIL CDC - phil.cdc.gov", top);
        top = addCredit("Player Ship", "Kenney.nl - www.kenney.nl", top);
        top = addCredit("Powerup", "Joseph Crown - opengameart.org", top);
        top = addCredit("Steam Achievement Images", "yinakoSGA - opengameart.org", top);
        top = addCredit("Other Artwork", "rubberduck - opengameart.org", top);

        top = addCredit("Music: Heroic Minority", "Alexandr Zhelanov - opengameart.org", top);
        top = addCredit("Music: Battle Theme A", "Alex Smith - opengameart.org", top);

        top = addCredit("Audio: Menu Activate", "SoundScapes - freesound.org", top);
        top = addCredit("Audio: Menu Accept", "LokiF - opengameart.org", top);
        top = addCredit("Audio: Player Start", "Arthur - opengameart.org", top);
        top = addCredit("Audio: Player Death", "Retimer - opengameart.org", top);
        top = addCredit("Audio: Thrust", "nathanshadow - freesound.org", top);
        top = addCredit("Audio: Fire", "Kenny - www.kenny.nl", top);
        top = addCredit("Audio: Fire Bomb", "NenadSimic - opengameart.org", top);
        top = addCredit("Audio: Explode Bomb", "Michel Baradari - opengameart.org", top);
        top = addCredit("Audio: Virus Death", "Michel Baradari - opengameart.org", top);
        top = addCredit("Audio: Powerup", "DoKashiteru - opengameart.org", top);

        //
        // Go through and center everything
        for (auto&& item : m_credits)
        {
            item->setPosition({ -item->getRegion().width / 2.0f, item->getRegion().top });
        }

        return true;
    }

    void Credits::signalKeyPressed([[maybe_unused]] sf::Event::KeyEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        switch (event.code)
        {
            case sf::Keyboard::Escape:
                m_nextState = ViewState::MainMenu;
                break;
            default: // Needed to eliminate unused enumeration compiler warning
                break;
        }
    }

    ViewState Credits::update(const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        //
        // Scroll the credits up vertically over time
        static const auto MOVE_RATE_US = Configuration::getGraphics().getViewCoordinates().height / 8000000;
        for (auto&& item : m_credits)
        {
            item->setPosition({ item->getRegion().left, item->getRegion().top - MOVE_RATE_US * elapsedTime.count() });
        }

        return m_nextState;
    }

    void Credits::render(sf::RenderTarget& renderTarget, [[maybe_unused]] const std::chrono::microseconds elapsedTime)
    {
        static const auto RENDER_TOP = -(0.35f * Configuration::getGraphics().getViewCoordinates().height);
        MenuView::render(renderTarget, elapsedTime);

        for (auto&& item : m_credits)
        {
            if (item->getRegion().top > RENDER_TOP)
            {
                item->render(renderTarget);
            }
        }
    }

    float Credits::addCredit(std::string title, std::string item, float top)
    {
        static const auto TITLE_SPACING_FACTOR = 1.5f;
        static const auto ITEM_SPACING_FACTOR = 2.0f;

        //
        // Creating a test item of the whole alphabet so we can always find the
        // tallest character in the font and use that for the height to separate menu items.
        // NOTE: I understand this is wasteful to do this everytime.  But because this is not performance
        //       sensitive, I'm not going to increase the complexity just to do these one time.
        sf::Text alphabetTitle("ABCDEFGHIHKLMNOPQRTSUVWXYZ", *Content::get<sf::Font>(content::KEY_FONT_CREDITS), Configuration::get<std::uint8_t>(config::FONT_CREDITS_TITLE_SIZE));
        alphabetTitle.scale(Configuration::getGraphics().getScaleUI());
        sf::Text alphabetItem("ABCDEFGHIHKLMNOPQRTSUVWXYZ", *Content::get<sf::Font>(content::KEY_FONT_CREDITS), Configuration::get<std::uint8_t>(config::FONT_CREDITS_ITEM_SIZE));
        alphabetItem.scale(Configuration::getGraphics().getScaleUI());

        m_credits.push_back(std::make_shared<ui::Text>(0.0f, top, title, Content::get<sf::Font>(content::KEY_FONT_CREDITS), sf::Color(51, 102, 255), sf::Color::Black, Configuration::get<std::uint8_t>(config::FONT_CREDITS_TITLE_SIZE)));
        top += alphabetTitle.getGlobalBounds().height * TITLE_SPACING_FACTOR;
        m_credits.push_back(std::make_shared<ui::Text>(0.0f, top, item, Content::get<sf::Font>(content::KEY_FONT_CREDITS), sf::Color::White, sf::Color::Black, Configuration::get<std::uint8_t>(config::FONT_CREDITS_ITEM_SIZE)));
        top += alphabetItem.getGlobalBounds().height * ITEM_SPACING_FACTOR;

        return top;
    }
} // namespace views
