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

#include "components/Powerup.hpp"
#include "entities/Powerup.hpp"
#include "entities/Virus.hpp"
#include "misc/math.hpp"
#include "services/ConfigurationPath.hpp"

#include <algorithm>
#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace levels
{
    class Level
    {
      public:
        Level(const std::string key);
        virtual ~Level(){}; // Needed for std::unique_ptr to be happy

        virtual void loadContent();
        virtual std::vector<std::shared_ptr<entities::Virus>> initializeViruses() = 0;

        auto getKey() { return m_key; }
        auto getBackgroundImageKey() { return m_backgroundImageKey; }
        auto getBackgroundMusicKey() { return m_backgroundMusicKey; }
        auto getBackgroundSize() { return m_backgroundSize; }
        auto getMaxViruses() { return m_maxVirusCount; }
        auto getNanoBotCount() { return m_nanoBotCount; }
        auto getMessageReady() { return m_messageReady; }
        auto getMessageSuccess() { return m_messageSuccess; }
        auto getMessageFailure() { return m_messageFailure; }

        virtual std::optional<math::Point2f> findSafeStart(std::chrono::microseconds howLongWaiting, const std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Entity>>& viruses) = 0;
        virtual bool collidesWithBorder(entities::Entity& entity) = 0;
        virtual void bounceOffBorder(entities::Entity& entity) = 0;

        virtual math::Point2f computePowerupPosition() = 0;

      protected:
        config::config_path m_backgroundImagePath;
        std::string m_backgroundImageKey;
        config::config_path m_backgroundMusicPath;
        std::string m_backgroundMusicKey;
        std::uint8_t m_initialVirusCount;
        std::uint8_t m_maxVirusCount;
        std::uint8_t m_nanoBotCount;

        math::Dimension2f m_backgroundSize{ 0.0f, 0.0f };

      private:
        std::string m_key;
        std::string m_messageReady;
        std::string m_messageSuccess;
        std::string m_messageFailure;
    };
} // namespace levels