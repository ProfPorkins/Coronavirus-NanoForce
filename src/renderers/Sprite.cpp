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

#include "Sprite.hpp"

#include "Configuration.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "misc/math.hpp"

namespace renderers
{
    Sprite::Sprite(std::shared_ptr<sf::Texture> texture)
    {
        m_sprite = std::make_shared<sf::Sprite>();
        m_sprite->setTexture(*texture);
        // Point about which drawing, rotation, etc takes place, the center of the texture
        m_sprite->setOrigin({ texture->getSize().x / 2.0f, texture->getSize().y / 2.0f });
    }

    void Sprite::render(entities::Entity& entity, sf::RenderTarget& renderTarget)
    {
        auto position = entity.getComponent<components::Position>();
        auto size = entity.getComponent<components::Size>();
        auto orientation = entity.getComponent<components::Orientation>();

        m_sprite->setPosition(position->get());
        // Not all entities (like the bullet) have/need an orientation.
        if (orientation != nullptr)
        {
            m_sprite->setRotation(orientation->get());
        }

        m_sprite->setScale(math::getViewScale(size->get(), m_sprite->getTexture()));

        renderTarget.draw(*m_sprite);
    }
} // namespace renderers
