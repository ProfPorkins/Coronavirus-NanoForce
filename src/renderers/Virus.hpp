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

#include "entities/Entity.hpp"
#include "entities/Virus.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

namespace renderers
{
    // --------------------------------------------------------------
    //
    // This renderer knows how to render a virus and however many
    // bullets have surrounded it.
    //
    // --------------------------------------------------------------
    class Virus
    {
      public:
        Virus(std::shared_ptr<sf::Texture> texVirus, std::shared_ptr<sf::Texture> texBullet);

        void render(entities::Virus& entity, sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime);
        void render(std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Virus>>& entities, sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime);

      private:
        std::shared_ptr<sf::Sprite> m_sprite;
        std::shared_ptr<sf::Sprite> m_bullet;
    };

} // namespace renderers
