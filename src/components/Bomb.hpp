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

#include <chrono>

namespace components
{
    class Bomb : public Component
    {
      public:
        Bomb(std::uint16_t bulletCount, std::uint16_t bulletDamage, float bulletSize, std::chrono::microseconds bulletLifetime) :
            m_bulletCount(bulletCount),
            m_bulletDamage(bulletDamage),
            m_bulletSize(bulletSize),
            m_bulletLifetime(bulletLifetime)
        {
        }

        auto getBulletCount() { return m_bulletCount; }
        auto getBulletDamage() { return m_bulletDamage; }
        auto getBulletSize() { return m_bulletSize; }
        auto getBulletLifetime() { return m_bulletLifetime; }

      private:
        std::uint16_t m_bulletCount;
        std::uint16_t m_bulletDamage;
        float m_bulletSize;
        std::chrono::microseconds m_bulletLifetime;
    };
} // namespace components
