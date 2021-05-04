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

#include "Virus.hpp"

#include "Configuration.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"

#include <cmath>

namespace renderers
{
    // --------------------------------------------------------------
    //
    // Rendering of a virus takes both th virus texture and the
    // bullet texture.
    //
    // --------------------------------------------------------------
    Virus::Virus(std::shared_ptr<sf::Texture> texVirus, std::shared_ptr<sf::Texture> texBullet)
    {
        m_sprite = std::make_shared<sf::Sprite>();
        m_sprite->setTexture(*texVirus);
        m_sprite->setOrigin({ texVirus->getSize().x / 2.0f, texVirus->getSize().y / 2.0f });

        m_bullet = std::make_shared<sf::Sprite>();
        m_bullet->setTexture(*texBullet);
        m_bullet->setOrigin({ texBullet->getSize().x / 2.0f, texBullet->getSize().y / 2.0f });
    }

    // --------------------------------------------------------------
    //
    // A virus can be surrounded by bullets and those need to be rendered
    // also.  The bullets rotate in a direction opposite to the rotation
    // of the virus.  That is taken care of by grabbing the start angle
    // of the bullets from the virus itself.
    //
    // --------------------------------------------------------------
    void Virus::render(entities::Virus& entity, sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
    {
        auto position = entity.getComponent<components::Position>();
        auto size = entity.getComponent<components::Size>();

        m_sprite->setPosition(position->get());
        m_sprite->setRotation(entity.getComponent<components::Orientation>()->get());
        m_sprite->setScale(math::getViewScale(size->get(), m_sprite->getTexture()));

        renderTarget.draw(*m_sprite);

        //
        // Now, render any bullets attached to this virus.
        if (entity.getBullets().size() > 0)
        {
            auto angle = math::toRadians(entity.getBulletAngleStart());
            entity.updateBulletAngleStart(elapsedTime);
            // Evenly place them around the virus
            auto angleDiff = (2.0f * 3.14159f) / entity.getBullets().size();
            auto radius = size->getInnerRadius();
            for (auto&& bullet : entity.getBullets())
            {
                auto bulletSize = bullet->getComponent<components::Size>();
                auto bulletRadius = bulletSize->getInnerRadius();

                auto x = position->get().x + (radius + bulletRadius) * std::cos(angle);
                auto y = position->get().y + (radius + bulletRadius) * std::sin(angle);
                angle += angleDiff;

                m_bullet->setPosition({ x, y });
                m_bullet->setRotation(0.0f);
                m_bullet->setScale(math::getViewScale(bulletSize->get(), m_bullet->getTexture()));

                renderTarget.draw(*m_bullet);
            }
        }
    }
} // namespace renderers
