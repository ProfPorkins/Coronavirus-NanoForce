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

namespace components
{
    class Control : public Component
    {
      public:
        Control(double thrustRate, float rotateRate, float maxSpeed) :
            m_thrustRate(thrustRate),
            m_rotateRate(rotateRate),
            m_maxSpeed(maxSpeed)
        {
        }

        auto isThrusting() { return m_thrusting; }
        void setThrusting(bool state) { m_thrusting = state; }
        auto getThrustRate() { return m_thrustRate; }
        auto getRotateRate() { return m_rotateRate; }
        auto getMaxSpeed() { return m_maxSpeed; }

      private:
        bool m_thrusting{ false };
        const double m_thrustRate{ 0.0 }; // per us
        const float m_rotateRate{ 0.0f }; // degrees per us
        const float m_maxSpeed{ 0.0f };   // per us
    };
} // namespace components
