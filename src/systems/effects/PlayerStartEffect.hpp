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

#include "ParticleEffect.hpp"
#include "misc/math.hpp"

#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>
#include <random>
#include <string>

namespace systems
{
    class PlayerStartEffect : public ParticleEffect
    {
      public:
        PlayerStartEffect(math::Point2f center, std::uint16_t howMany, std::chrono::microseconds lifetime);
        virtual void update(const std::chrono::microseconds elapsedTime, std::function<std::unique_ptr<Particle>()> getParticle, std::function<void(std::unique_ptr<Particle>)> addParticle);

      private:
        math::Point2f m_center;
        std::uint16_t m_howMany;
        std::chrono::microseconds m_lifetime;
        std::shared_ptr<sf::Texture> m_texture;

        // Random number stuff
        std::random_device m_rd;
        std::mt19937 m_generator;
        std::normal_distribution<float> m_distNormal;
        std::uniform_real_distribution<float> m_distCircle;
    };
} // namespace systems
