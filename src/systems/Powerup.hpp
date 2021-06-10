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

#include "System.hpp"
#include "components/Powerup.hpp"
#include "entities/Powerup.hpp"
#include "levels/Level.hpp"

#include <atomic>
#include <chrono>
#include <functional>
#include <random>

namespace systems
{
    class Powerup : public System
    {
      public:
        Powerup(levels::Level& level, std::function<void(std::shared_ptr<entities::Powerup>&)> emitPowerup, const std::string levelKey);

        virtual void update(std::chrono::microseconds elapsedTime) override;

      protected:
        virtual bool isInterested([[maybe_unused]] entities::Entity* entity) override
        {
            return false; // This system does not act on entities
        }

      private:
        levels::Level& m_level;
        std::function<void(std::shared_ptr<entities::Powerup>&)> m_emitPowerup;

        std::random_device m_rd;
        std::mt19937 m_generator;
        std::uniform_real_distribution<float> m_distUniform;
        std::atomic_uint8_t m_contentToLoad{ 0 };
        std::atomic_bool m_contentError{ false };

        std::chrono::microseconds m_timeMinPowerup{ 0 };

        std::chrono::microseconds m_timeBombPowerup{ 0 };
        std::chrono::microseconds m_nextBombPowerup{ std::chrono::microseconds::max() };
        std::chrono::microseconds m_nextBombPowerupCompute{ 0 };

        std::chrono::microseconds m_timeRapidFirePowerup{ 0 };
        std::chrono::microseconds m_nextRapidFirePowerup{ std::chrono::microseconds::max() };
        std::chrono::microseconds m_nextRapidFirePowerupCompute{ 0 };

        std::chrono::microseconds m_timeSpreadFirePowerup{ 0 };
        std::chrono::microseconds m_nextSpreadFirePowerup{ std::chrono::microseconds::max() };
        std::chrono::microseconds m_nextSpreadFirePowerupCompute{ 0 };

        template <typename T>
        void updatePowerup(const std::chrono::microseconds elapsedTime, const std::chrono::microseconds timeFrame, std::chrono::microseconds& timeRemaining, std::chrono::microseconds& nextCompute)
        {
            nextCompute -= elapsedTime;
            if (nextCompute <= std::chrono::microseconds(0))
            {
                // If the time frame is 0, that means the powerup should never appear
                if (timeFrame > std::chrono::microseconds{ 0 })
                {
                    // We want the powerup to appear once per timeFrame.  Therefore
                    // compute a uniform random number/time between 0 and timeFrame (ms) and that is when
                    // it will appear.
                    timeRemaining = std::chrono::duration_cast<std::chrono::microseconds>(m_distUniform(m_generator) * timeFrame);
                    timeRemaining = std::max(m_timeMinPowerup, timeRemaining);
                    // Don't forget to reset the next time a powerup decision should be computed
                    nextCompute = timeFrame;
                }
            }

            timeRemaining -= elapsedTime;
            if (timeRemaining <= std::chrono::microseconds(0))
            {
                std::shared_ptr<entities::Powerup> powerup = std::make_shared<T>(m_level.computePowerupPosition());
                m_emitPowerup(powerup);
                // Setting to a huge number so we don't generate another one until the time is (re)set above
                timeRemaining = std::chrono::microseconds::max();
            }
        }
    };
} // namespace systems