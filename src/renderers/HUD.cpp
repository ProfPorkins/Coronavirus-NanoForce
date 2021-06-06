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

#include "HUD.hpp"

import misc;

#include "components/Size.hpp"
#include "misc/math.hpp"
#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"
#include "services/ContentKey.hpp"

#include <string>

namespace renderers
{
    HUD::HUD()
    {
        m_spritePlayer = std::make_shared<sf::Sprite>();
        auto texture = Content::get<sf::Texture>(content::KEY_IMAGE_PLAYER);
        m_spritePlayer->setTexture(*texture);
        m_spritePlayer->setOrigin({ texture->getSize().x / 2.0f, texture->getSize().y / 2.0f });
        // Need to rotate it into the orientation we want for the HUD
        m_spritePlayer->setRotation(270.0f);
        // We get to choose our own size, doesn't have to match the actual player's in-game size
        m_spritePlayer->setScale(math::getViewScale(4.0f, m_spritePlayer->getTexture()));

        //
        // Set the size and postion of the background panel for the remaining ship panel
        auto coords = Configuration::getGraphics().getViewCoordinates();
        auto playerSize = Configuration::get<float>(config::PLAYER_SIZE);
        m_backgroundBots.setSize({ playerSize * 6.0f, playerSize * 2 });
        m_backgroundBots.setPosition({ coords.width / 2 - 7 * playerSize, -coords.height / 2 + coords.height * 0.10f - playerSize });
        m_backgroundBots.setFillColor(sf::Color(0, 0, 255, 100));
        m_backgroundBots.setOutlineColor(sf::Color::White);
        m_backgroundBots.setOutlineThickness(0.1f);

        //
        // Set the size and position of the background panel for the time played panel
        // Make it the same size as the player panel for symmetry purposes.
        m_backgroundTime.setSize(m_backgroundBots.getSize());
        m_backgroundTime.setPosition({ -(coords.width / 2) + playerSize, m_backgroundBots.getPosition().y });
        m_backgroundTime.setFillColor(sf::Color(0, 0, 255, 100));
        m_backgroundTime.setOutlineColor(sf::Color::White);
        m_backgroundTime.setOutlineThickness(0.1f);

        //
        // Set the size and position of the background panel for the number of
        // viruses killed.  Make it the same size as the other panels
        // for symmetry purposes.
        m_backgroundViruses.setSize(m_backgroundBots.getSize());
        m_backgroundViruses.setPosition({ -(coords.width / 2) + playerSize, coords.height / 2 - coords.height * 0.10f - playerSize });
        m_backgroundViruses.setFillColor(sf::Color(0, 0, 255, 100));
        m_backgroundViruses.setOutlineColor(sf::Color::White);
        m_backgroundViruses.setOutlineThickness(0.1f);

        //
        // Center the time in the time panel
        m_textTime = std::make_unique<ui::Text>(
            0.0f, 0.0f, "000:00.00",
            Content::get<sf::Font>(content::KEY_FONT_GAMEPLAY),
            sf::Color::White,
            sf::Color::Black,
            Configuration::get<std::uint8_t>(config::FONT_GAMEPLAY_SCORE_SIZE));
        m_textTime->setPosition({ m_backgroundTime.getPosition().x + (m_backgroundTime.getSize().x - m_textTime->getRegion().width) / 2.0f,
                                  m_backgroundTime.getPosition().y + (m_backgroundTime.getSize().y - m_textTime->getRegion().height * 1.5f) / 2.0f });

        //
        // Create the killed text widget
        m_textKilled = std::make_unique<ui::Text>(
            0.0f, 0.0f, "0",
            Content::get<sf::Font>(content::KEY_FONT_GAMEPLAY),
            sf::Color::White,
            sf::Color::Black,
            Configuration::get<std::uint8_t>(config::FONT_GAMEPLAY_SCORE_SIZE));
    }

    void HUD::render(std::uint8_t howManyNanoBots, std::chrono::milliseconds timePlayed, std::uint32_t virusesKilled, sf::RenderTarget& renderTarget)
    {
        //
        // Get the remaining nano bots rendered.
        // The / 2 on the width and height is because 0, 0 is the center of the window.
        renderTarget.draw(m_backgroundBots);

        auto coords = Configuration::getGraphics().getViewCoordinates();
        for (auto bot = 1; bot <= howManyNanoBots; bot++)
        {
            auto whereX = bot * (5 + 1);
            m_spritePlayer->setPosition({ coords.width / 2 - whereX, -coords.height / 2 + coords.height * 0.10f });
            renderTarget.draw(*m_spritePlayer);
        }

        //
        // Render how long played
        renderTarget.draw(m_backgroundTime);

        m_textTime->setText(misc::timeToMMMSSString(timePlayed));
        m_textTime->render(renderTarget);

        //
        // Render how many viruses killed
        renderTarget.draw(m_backgroundViruses);

        //
        // Center in the field
        m_textKilled->setText(std::to_string(virusesKilled));
        m_textKilled->setPosition({ m_backgroundViruses.getPosition().x + (m_backgroundViruses.getSize().x - m_textKilled->getRegion().width) / 2.0f,
                                    m_backgroundViruses.getPosition().y + (m_backgroundViruses.getSize().y - m_textKilled->getRegion().height * 1.5f) / 2.0f });
        m_textKilled->render(renderTarget);
    }
} // namespace renderers
