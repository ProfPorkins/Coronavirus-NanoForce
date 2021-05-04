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

#include "entities/Entity.hpp"
#include "misc/misc.hpp"

#include <chrono>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

namespace entities
{
    class Virus : public Entity
    {
      public:
        struct Specification
        {
            Specification() = default;

            float minSize;
            float maxSize;
            std::chrono::microseconds maxAge;
            std::uint16_t healthStart;
            std::uint8_t healthIncrements;
            std::chrono::microseconds healthIncrementTime;
            std::chrono::microseconds gestationMin;
            std::chrono::microseconds gestationMean;
            std::chrono::microseconds gestationStdev;
            float rotateRate; // degrees per ms
        };

      public:
        static std::shared_ptr<Virus> create(std::chrono::microseconds age = std::chrono::microseconds(0));

        Virus(std::chrono::microseconds age, Specification spec);

        void addBullet(std::shared_ptr<entities::Entity> bullet);
        auto& getBullets() { return m_bullets; }
        float getBulletAngleStart() { return m_bulletAngleStart; }

        void updateBulletAngleStart(std::chrono::microseconds elapsedTime) { m_bulletAngleStart += (m_bulletRotateRate * elapsedTime.count()); }

      private:
        float m_bulletAngleStart{ 0.0f };
        float m_bulletRotateRate{ 0.1f * misc::PER_MS_TO_US }; // degrees per us
        std::vector<std::shared_ptr<entities::Entity>> m_bullets;

        std::random_device m_rd;
        std::mt19937 m_generator;
        std::uniform_real_distribution<float> m_distAngle;
        std::uniform_int_distribution<std::uint16_t> m_distBoolean;

        void selectPath();
    };
} // namespace entities
