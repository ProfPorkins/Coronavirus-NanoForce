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

#include "CircleExpansionEffect.hpp"

#include "services/Content.hpp"
#include "systems/Particle.hpp"

#include <cmath>

namespace systems
{
    CircleExpansionEffect::CircleExpansionEffect(std::string image, math::Point2f center, float atDistance, std::uint16_t howMany, float speed, float sizeStart, float sizeEnd, std::chrono::microseconds lifetime) :
        m_center(center),
        m_atDistance(atDistance),
        m_howMany(howMany),
        m_sizeStart(sizeStart),
        m_sizeEnd(sizeEnd),
        m_speed(speed),
        m_lifetime(lifetime),
        m_texture(Content::get<sf::Texture>(image))
    {
    }

    CircleExpansionEffect::CircleExpansionEffect(std::string image, math::Point2f center, float atDistance, float speed, float sizeStart, float sizeEnd, float orientation, std::chrono::microseconds lifetime) :
        m_center(center),
        m_atDistance(atDistance),
        m_howMany(1),
        m_sizeStart(sizeStart),
        m_sizeEnd(sizeEnd),
        m_speed(speed),
        m_orientation(orientation),
        m_lifetime(lifetime),
        m_texture(Content::get<sf::Texture>(image))
    {
    }

    void CircleExpansionEffect::update(const std::chrono::microseconds elapsedTime, std::function<std::unique_ptr<Particle>()> getParticle, std::function<void(std::unique_ptr<Particle>)> addParticle)
    {
        ParticleEffect::update(elapsedTime);

        //
        // Generate a bunch of uniformly distributed particles around the center
        float angleDiff = 2 * 3.14159f / m_howMany;
        float angle = 0.0f;
        for (decltype(m_howMany) i = 0; i < m_howMany; i++, angle += angleDiff)
        {
            if (auto p = getParticle(); p != nullptr)
            {
                p->rotation = m_orientation.has_value() ? m_orientation.value() : 0.0f;
                p->lifetime = m_lifetime;
                p->alive = std::chrono::microseconds(0);
                p->size = m_sizeStart;
                p->sizeStart = m_sizeStart;
                p->sizeEnd = m_sizeEnd;
                p->direction.x = std::cos(angle);
                p->direction.y = std::sin(angle);
                // Adjust the center by the distance the particle should start from the effect center
                p->center = { m_center.x + p->direction.x * m_atDistance, m_center.y + p->direction.y * m_atDistance };
                p->speed = m_speed;
                p->sprite.setTexture(*m_texture);
                p->sprite.setOrigin(p->sprite.getTexture()->getSize().x / 2.0f, p->sprite.getTexture()->getSize().y / 2.0f);
                p->sprite.setScale(math::getViewScale(p->size, p->sprite.getTexture()));

                addParticle(std::move(p));
            }
        }
    }
} // namespace systems