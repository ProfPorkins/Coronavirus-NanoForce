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

#include "Component.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <cstdint>
#include <memory>

namespace components
{
    class AnimatedSprite : public Component
    {
      public:
        AnimatedSprite(std::shared_ptr<sf::Texture> texture, std::uint8_t spriteCount, std::chrono::microseconds spriteTime) :
            m_spriteCount(spriteCount),
            m_spriteTime(spriteTime)
        {
            m_sprite = std::make_shared<sf::Sprite>();
            m_sprite->setTexture(*texture);
            // Point about which drawing, rotation, etc takes place, the center of the texture
            m_sprite->setOrigin({ (texture->getSize().x / spriteCount) / 2.0f, texture->getSize().y / 2.0f });
        }

        auto getSprite() { return m_sprite; }
        auto getSpriteCount() { return m_spriteCount; }
        auto getSpriteTime() { return m_spriteTime; }
        auto getCurrentSprite() { return m_currentSprite; }
        auto getCurrentSpriteRect()
        {
            int width = (m_sprite->getTexture()->getSize().x / m_spriteCount);
            int left = m_currentSprite * width;
            return sf::IntRect({ left, 0, width, static_cast<int>(m_sprite->getTexture()->getSize().y) });
        }
        auto incrementSprite() { m_currentSprite = (m_currentSprite + 1) % m_spriteCount; }
        auto getElapsedTime() { return m_elapsedTime; }
        void updateElapsedTime(std::chrono::microseconds howMuch) { m_elapsedTime += howMuch; }

      private:
        std::shared_ptr<sf::Sprite> m_sprite;
        std::uint8_t m_spriteCount;
        std::chrono::microseconds m_spriteTime;
        std::uint8_t m_currentSprite{ 0 };
        std::chrono::microseconds m_elapsedTime{ 0 };
    };
} // namespace components
