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

#include "ParticleSystem.hpp"

#include "services/Configuration.hpp"
#include "effects/ParticleEffect.hpp"

namespace systems
{
    ParticleSystem::ParticleSystem()
    {
        preAllocateParticles();
    }

    // --------------------------------------------------------------
    //
    // Two simple things need to be done here...
    //   1.  Update the state of all active particles
    //   2.  Update the active effects, generating new particles
    //
    // --------------------------------------------------------------
    void ParticleSystem::update(const std::chrono::microseconds elapsedTime)
    {
        updateParticles(elapsedTime);
        updateEffects(elapsedTime);
    }

    // --------------------------------------------------------------
    //
    // Work through the particles, updating them and discarding those
    // whose lifetime has expired.
    //
    // --------------------------------------------------------------
    void ParticleSystem::updateParticles(const std::chrono::microseconds& elapsedTime)
    {
        //
        // Step 1: Update all existing particles
        decltype(ParticleSystem::m_particleCount) keeperCount = 0;
        for (decltype(ParticleSystem::m_particleCount) p = 0; p < m_particleCount; p++)
        {
            auto particle = std::move(m_inUse[p]);
            particle->alive += elapsedTime;
            //
            // Check to see if it is alive before going to the trouble of doing a full update on the particle
            if (particle->alive < particle->lifetime)
            {
                // Update position
                particle->center.x += (elapsedTime.count() * particle->speed * particle->direction.x);
                particle->center.y += (elapsedTime.count() * particle->speed * particle->direction.y);

                // Update size
                particle->size = math::lerp(static_cast<float>(particle->alive.count()), 0.0f, static_cast<float>(particle->lifetime.count()), particle->sizeStart, particle->sizeEnd);
                if (particle->sizeStart != particle->sizeEnd)
                {
                    particle->sprite.setScale(math::getViewScale(particle->size, particle->sprite.getTexture()));
                }

                // Update alpha transparency
                particle->alpha = math::lerp(static_cast<float>(particle->alive.count()), 0.0f, static_cast<float>(particle->lifetime.count()), particle->alphaStart, particle->alphaEnd);

                // Place it back to the in use set of particles
                m_inUse[keeperCount++] = std::move(particle);
            }
            else
            {
                // Send it back to the available queue
                m_available.push_back(std::move(particle));
            }
        }
        m_particleCount = keeperCount;
    }

    // --------------------------------------------------------------
    //
    // Go through each of the active effects, giving them a chance
    // to generate new particles.
    //
    // --------------------------------------------------------------
    void ParticleSystem::updateEffects(const std::chrono::microseconds& elapsedTime)
    {
        //
        // Step 2: Update active effects
        auto effectCount = m_effects.size();
        while (effectCount-- > 0)
        {
            auto effect = std::move(m_effects.front());
            m_effects.pop_front();
            effect->update(
                elapsedTime,
                [this]() { return getAvailableParticle(); },
                [this](std::unique_ptr<Particle> p) { this->addParticle(std::move(p)); });
            // Put it back in the queue if its lifetime hasn't expired
            if (effect->getAlive() < effect->getLifetime())
            {
                m_effects.push_back(std::move(effect));
            }
        }
    }

    void ParticleSystem::preAllocateParticles()
    {
        while (m_available.size() < MAX_PARTICLES)
        {
            m_available.push_back(std::make_unique<Particle>());
        }
    }

    void ParticleSystem::addParticle(std::unique_ptr<Particle> p)
    {
        m_inUse[m_particleCount++] = std::move(p);
    }

} // namespace systems