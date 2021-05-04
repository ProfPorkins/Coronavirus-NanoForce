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

#include "About.hpp"

#include "Configuration.hpp"

namespace views
{
    bool About::start()
    {
        MenuView::start();
        // Need to ensure the next state stays the About view before checking to see if
        // the view is already initialized.
        m_nextState = ViewState::About;
        if (m_initialized)
            return true;

        //
        // Wipe everything out and reset the story every time
        m_items.clear();

        auto top = (0.50f * Configuration::getGraphics().getViewCoordinates().height);
        top = addItem("vaccines have failed", top);
        top = addItem("human antibodies are not working", top);
        top = addItem("our last hope is an", top);
        top = addItem("army of nano-drone pilots", top);
        top = addItem("destroying the SARS-COV-2 virus", top);
        top = addItem("patient by patient", top);
        top = addItem("", top);
        top = addItem("join the 'Nano Force' in the fight", top);
        top = addItem("to save humannity from this pandemic", top);
        top = addItem("", top);
        top = addItem("for each patient you are provided", top);
        top = addItem("three nano drones to eliminate", top);
        top = addItem("all SARS-COV-2 viruses", top);
        top = addItem("", top);
        top = addItem("during battle, you'll receive", top);
        top = addItem("help in the form of upgrades", top);
        top = addItem("for the drone weapons", top);
        top = addItem("", top);
        top = addItem("you'll begin your training in", top);
        top = addItem("controlled lab environments", top);
        top = addItem("once you are ready", top);
        top = addItem("you'll finish your training", top);
        top = addItem("with several patient simulations", top);

        //
        // Go through and center everything
        for (auto&& item : m_items)
        {
            item->setPosition({ -item->getRegion().width / 2.0f, item->getRegion().top });
        }

        return true;
    }

    void About::signalKeyPressed([[maybe_unused]] sf::Event::KeyEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
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

    ViewState About::update([[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        // Scroll the text up vertically over time
        static const auto MOVE_RATE_US = Configuration::getGraphics().getViewCoordinates().height / 15000000;
        for (auto&& item : m_items)
        {
            item->setPosition({ item->getRegion().left, item->getRegion().top - MOVE_RATE_US * elapsedTime.count() });
        }

        return m_nextState;
    }

    void About::render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
    {
        static const auto RENDER_TOP = -(0.35f * Configuration::getGraphics().getViewCoordinates().height);
        MenuView::render(renderTarget, elapsedTime);

        for (auto&& item : m_items)
        {
            if (item->getRegion().top > RENDER_TOP)
            {
                item->render(renderTarget);
            }
        }
    }

    float About::addItem(std::string item, float top)
    {
        static const auto ITEM_SPACING_FACTOR = 2.0f;

        //
        // Creating a test item of the whole alphabet so we can always find the
        // tallest character in the font and use that for the height to separate menu items.
        // NOTE: I understand this is wasteful to do this everytime.  But because this is not performance
        //       sensitive, I'm not going to increase the complexity just to do these one time.
        sf::Text alphabetItem("ABCDEFGHIHKLMNOPQRTSUVWXYZ", *Content::get<sf::Font>(content::KEY_FONT_CREDITS), Configuration::get<std::uint8_t>(config::FONT_CREDITS_ITEM_SIZE));
        alphabetItem.scale(Configuration::getGraphics().getScaleUI());

        // We just put a left of 0, because it gets centered later on
        m_items.push_back(std::make_shared<ui::Text>(0.0f, top, item, Content::get<sf::Font>(content::KEY_FONT_CREDITS), sf::Color::White, sf::Color::Black, Configuration::get<std::uint8_t>(config::FONT_CREDITS_ITEM_SIZE)));
        top += alphabetItem.getGlobalBounds().height * ITEM_SPACING_FACTOR;

        return top;
    }
} // namespace views
