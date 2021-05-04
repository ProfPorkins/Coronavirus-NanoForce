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
#include <random>

namespace components
{
    class Birth : public Component
    {
      public:
        Birth(std::chrono::microseconds maturityAge, std::chrono::microseconds gestationMin, std::chrono::microseconds gestationMean, std::chrono::microseconds gestationStdev) :
            m_maturityAge(maturityAge),
            m_gestationMin(gestationMin),
            m_distribution(static_cast<double>(gestationMean.count()), static_cast<double>(gestationStdev.count()))
        {
        }

        auto getMinAge() { return m_maturityAge; }
        auto getGestationMin() { return m_gestationMin; }
        auto& getDistribution() { return m_distribution; }

        auto getCurrentGestation() { return m_currentGestation; }
        void setGestationTime(std::chrono::microseconds howLong)
        {
            m_currentGestation = howLong;
            m_gestation = true;
        }
        void resetGestation() { m_gestation = false; }
        bool isGestating() { return m_gestation; }

        void update(std::chrono::microseconds elapsedTime)
        {
            if (m_gestation)
                m_currentGestation -= elapsedTime;
        }

      private:
        std::chrono::microseconds m_maturityAge;
        std::chrono::microseconds m_gestationMin;
        std::chrono::microseconds m_currentGestation{ 0 };
        bool m_gestation{ false };

        std::normal_distribution<double> m_distribution; // Yes, each component has its own distribution
    };
} // namespace components
