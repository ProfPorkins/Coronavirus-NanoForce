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

#pragma once

#include "UIFramework/Text.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdint>
#include <memory>

namespace renderers
{
    // --------------------------------------------------------------
    //
    // Renders the in-game information for the player.
    //
    // --------------------------------------------------------------
    class HUD
    {
      public:
        HUD();

        void render(std::uint8_t howManyNanoBots, std::chrono::milliseconds timePlayed, std::uint32_t virusesKilled, sf::RenderTarget& renderTarget);

      private:
        std::shared_ptr<sf::Sprite> m_spritePlayer;
        sf::RectangleShape m_backgroundBots;
        sf::RectangleShape m_backgroundTime;
        sf::RectangleShape m_backgroundViruses;

        std::unique_ptr<ui::Text> m_textTime;
        std::unique_ptr<ui::Text> m_textKilled;
    };
} // namespace renderers
