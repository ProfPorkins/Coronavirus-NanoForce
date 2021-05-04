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
#include <cstdint>

namespace components
{
    class Health : public Component
    {
      public:
        Health(std::uint16_t health, std::uint8_t numberIncrements, std::chrono::microseconds incrementTime) :
            m_health(health),
            m_remainingIncrements(numberIncrements),
            m_incrementTime(incrementTime)
        {
        }

        auto get() { return m_health; }
        void add(std::uint16_t howMuch) { m_health += howMuch; }
        void subtract(std::uint16_t howMuch) { m_health -= howMuch; }

        auto getIncrementTime() { return m_incrementTime; }
        auto getRemainingIncrements() { return m_remainingIncrements; }
        auto subtractRemainingIncrements(std::uint8_t howMany) { m_remainingIncrements -= howMany; }

        auto getElapsedIncrementTime() { return m_elapsedIncrementTime; }
        void addElapsedIncrementTime(std::chrono::microseconds howMuch) { m_elapsedIncrementTime += howMuch; }
        void subtractElapsedIncrementTime(std::chrono::microseconds howMuch) { m_elapsedIncrementTime -= howMuch; }

      private:
        std::int16_t m_health;                     // Setting this to a signed value so it can go negative
        std::int8_t m_remainingIncrements;         // How many increments to allow; signed for the same reason as above
        std::chrono::microseconds m_incrementTime; // 1 increment each X microseconds
        std::chrono::microseconds m_elapsedIncrementTime{ 0 };
    };
} // namespace components
