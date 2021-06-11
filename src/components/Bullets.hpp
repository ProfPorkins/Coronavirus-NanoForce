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
#include "misc/misc.hpp"

#include <chrono>
#include <cstdint>

namespace components
{
    class Bullets : public Component
    {
      public:
        Bullets()
        {
        }

        auto add() { m_howMany++; }
        auto howMany() { return m_howMany; }
        float getBulletAngleStart() { return m_bulletAngleStart; }

        void updateBulletRotation(std::uint16_t direction) { m_bulletRotateRate = (direction == 0) ? m_bulletRotateRate : -m_bulletRotateRate; }
        void updateBulletAngleStart(std::chrono::microseconds elapsedTime) { m_bulletAngleStart += (m_bulletRotateRate * elapsedTime.count()); }

      private:
        std::uint16_t m_howMany{ 0 };
        float m_bulletAngleStart{ 0.0f };
        float m_bulletRotateRate{ 0.1f * misc::PER_MS_TO_US }; // degrees per us
    };
} // namespace components
