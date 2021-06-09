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

#include "RendererParticleSystem.hpp"

namespace systems
{
    // --------------------------------------------------------------
    //
    // This class is a friend to the renderers::ParticleSystem so that
    // it has access to private data, without having to expose it
    // to everyone else.
    //
    // --------------------------------------------------------------
    void RendererParticleSystem::update(systems::ParticleSystem& ps, sf::RenderTarget& renderTarget)
    {
        for (decltype(ps.m_particleCount) p = 0; p < ps.m_particleCount; p++)
        {
            auto& particle = ps.m_inUse[p];
            particle->sprite.setPosition(particle->center);
            particle->sprite.setRotation(particle->rotation);

            renderTarget.draw(particle->sprite);
        }
    }

} // namespace systems
