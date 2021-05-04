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

#include "ExplosionEffect.hpp"

#include "Content.hpp"
#include "systems/Particle.hpp"

#include <cmath>

namespace systems
{
    ExplosionEffect::ExplosionEffect(std::string image, math::Point2f center, float atDistance, std::uint16_t howMany, float mean, float stdev, std::chrono::microseconds lifetime) :
        m_center(center),
        m_atDistance(atDistance),
        m_howMany(howMany),
        m_lifetime(lifetime),
        m_texture(Content::get<sf::Texture>(image)),
        m_generator(m_rd()),
        m_distNormal(mean, stdev),
        m_distCircle(0.0f, 2.0f * 3.14159f)
    {
    }

    void ExplosionEffect::update(const std::chrono::microseconds elapsedTime, std::function<std::unique_ptr<Particle>()> getParticle, std::function<void(std::unique_ptr<Particle>)> addParticle)
    {
        ParticleEffect::update(elapsedTime);

        //
        // Generate a bunch of particles around the center
        for (decltype(m_howMany) i = 0; i < m_howMany; i++)
        {
            if (auto p = getParticle(); p != nullptr)
            {
                p->lifetime = m_lifetime;
                p->alive = std::chrono::microseconds(0);
                p->size = p->sizeStart = p->sizeEnd = 1.0f;
                auto angle = m_distCircle(m_generator);
                p->direction.x = std::cos(angle);
                p->direction.y = std::sin(angle);
                p->center = { m_center.x + p->direction.x * m_atDistance, m_center.y + p->direction.y * m_atDistance };
                p->speed = m_distNormal(m_generator);
                p->sprite->setTexture(*m_texture);
                p->sprite->setOrigin(p->sprite->getTexture()->getSize().x / 2.0f, p->sprite->getTexture()->getSize().y / 2.0f);
                p->sprite->setScale(math::getViewScale(p->size, p->sprite->getTexture()));

                addParticle(std::move(p));
            }
        }
    }
} // namespace systems