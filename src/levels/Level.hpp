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

#include "entities/Powerup.hpp"
#include "entities/Virus.hpp"
#include "misc/math.hpp"
#include "services/ConfigurationPath.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <functional>
#include <optional>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

namespace levels
{
    class Level
    {
      public:
        Level(std::function<void(std::shared_ptr<entities::Powerup>&)> emitPowerup, const std::string key);
        virtual ~Level(){}; // Needed for std::unique_ptr to be happy

        virtual void loadContent();
        virtual std::vector<std::shared_ptr<entities::Virus>> initializeViruses() = 0;

        auto getBackgroundImageKey() { return m_backgroundImageKey; }
        auto getBackgroundMusicKey() { return m_backgroundMusicKey; }
        auto getBackgroundSize() { return m_backgroundSize; }
        auto getMaxViruses() { return m_maxVirusCount; }
        auto getNanoBotCount() { return m_nanoBotCount; }
        auto getMessageReady() { return m_messageReady; }
        auto getMessageSuccess() { return m_messageSuccess; }
        auto getMessageFailure() { return m_messageFailure; }

        virtual std::optional<math::Point2f> findSafeStart(std::chrono::microseconds howLongWaiting, std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Virus>>& viruses) = 0;
        virtual bool collidesWithBorder(entities::Entity& entity) = 0;
        virtual void bounceOffBorder(entities::Entity& entity) = 0;
        virtual void update(const std::chrono::microseconds elapsedTime);

      protected:
        std::random_device m_rd;
        std::mt19937 m_generator;
        std::uniform_real_distribution<float> m_distUniform;
        std::atomic_uint8_t m_contentToLoad{ 0 };
        std::atomic_bool m_contentError{ false };

        config::config_path m_backgroundImagePath;
        std::string m_backgroundImageKey;
        config::config_path m_backgroundMusicPath;
        std::string m_backgroundMusicKey;
        std::uint8_t m_initialVirusCount;
        std::uint8_t m_maxVirusCount;
        std::uint8_t m_nanoBotCount;

        math::Dimension2f m_backgroundSize{ 0.0f, 0.0f };

        std::function<void(std::shared_ptr<entities::Powerup>&)> m_emitPowerup;

        virtual math::Point2f computePowerupPosition() = 0;

      private:
        std::string m_levelKey;
        std::string m_messageReady;
        std::string m_messageSuccess;
        std::string m_messageFailure;
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
                std::shared_ptr<entities::Powerup> powerup = std::make_shared<T>(this->computePowerupPosition());
                m_emitPowerup(powerup);
                // Setting to a huge number so we don't generate another one until the time
                // is set in computeNextBombPowerup.
                timeRemaining = std::chrono::microseconds::max();
            }
        }
    };
} // namespace levels