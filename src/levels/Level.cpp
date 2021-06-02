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

#include "Level.hpp"

#include "entities/PowerupBomb.hpp"
#include "entities/PowerupRapidFire.hpp"
#include "entities/PowerupSpreadFire.hpp"
#include "misc/misc.hpp"
#include "services/Configuration.hpp"
#include "services/Content.hpp"

namespace levels
{

    Level::Level(std::function<void(std::shared_ptr<entities::Powerup>&)> emitPowerup, const std::string key) :
        m_generator(m_rd()),
        m_distUniform(0.0f, 1.0f),
        m_emitPowerup(emitPowerup),
        m_levelKey(key)
    {
        using namespace std::string_literals;
        using namespace config;

        const config_path MESSAGE_READY = { DOM_LEVELS, key, DOM_CONTENT, DOM_MESSAGES, DOM_READY };
        const config_path MESSAGE_SUCCESS = { DOM_LEVELS, key, DOM_CONTENT, DOM_MESSAGES, DOM_SUCCESS };
        const config_path MESSAGE_FAILURE = { DOM_LEVELS, key, DOM_CONTENT, DOM_MESSAGES, DOM_FAILURE };
        const config_path INITIAL_VIRUS_COUNT = { DOM_LEVELS, key, DOM_SETTINGS, DOM_INITIAL_VIRUS_COUNT };
        const config_path MAX_VIRUS_COUNT = { DOM_LEVELS, key, DOM_SETTINGS, DOM_MAX_VIRUS_COUNT };
        const config_path NANO_BOT_COUNT = { DOM_LEVELS, key, DOM_SETTINGS, DOM_NANO_BOT_COUNT };
        const config_path MIN_POWERUP_TIME = { DOM_LEVELS, key, DOM_SETTINGS, "min-powerup-time"s };
        const config_path BOMB_POWERUP_TIME = { DOM_LEVELS, key, DOM_SETTINGS, "bomb-powerup-time"s };
        const config_path RAPID_FIRE_POWERUP_TIME = { DOM_LEVELS, key, DOM_SETTINGS, "rapid-fire-powerup-time"s };
        const config_path SPREAD_FIRE_POWERUP_TIME = { DOM_LEVELS, key, DOM_SETTINGS, "spread-fire-powerup-time"s };

        m_messageReady = Configuration::get<std::string>(MESSAGE_READY);
        m_messageSuccess = Configuration::get<std::string>(MESSAGE_SUCCESS);
        m_messageFailure = Configuration::get<std::string>(MESSAGE_FAILURE);

        m_backgroundImagePath = { DOM_LEVELS, key, DOM_CONTENT, DOM_IMAGE, DOM_BACKGROUND };
        m_backgroundImageKey = "image/" + key + "-background";

        m_backgroundMusicPath = { DOM_LEVELS, key, DOM_CONTENT, DOM_MUSIC, DOM_BACKGROUND };
        m_backgroundMusicKey = "music/" + key + "-background";

        m_initialVirusCount = Configuration::get<std::uint8_t>(INITIAL_VIRUS_COUNT);
        m_maxVirusCount = Configuration::get<std::uint8_t>(MAX_VIRUS_COUNT);
        m_nanoBotCount = Configuration::get<std::uint8_t>(NANO_BOT_COUNT);

        m_timeMinPowerup = misc::msTous(Configuration::get<std::chrono::milliseconds>(MIN_POWERUP_TIME));
        m_timeBombPowerup = misc::msTous(Configuration::get<std::chrono::milliseconds>(BOMB_POWERUP_TIME));
        m_timeRapidFirePowerup = misc::msTous(Configuration::get<std::chrono::milliseconds>(RAPID_FIRE_POWERUP_TIME));
        m_timeSpreadFirePowerup = misc::msTous(Configuration::get<std::chrono::milliseconds>(SPREAD_FIRE_POWERUP_TIME));
    }

    // --------------------------------------------------------------
    //
    // Get the content necessary for the level loaded here.
    //
    // --------------------------------------------------------------
    void Level::loadContent()
    {
        if (!Content::has<sf::Texture>(m_backgroundImageKey))
        {
            Content::load<sf::Texture>(
                m_backgroundImageKey,
                Configuration::get<std::string>(m_backgroundImagePath),
                nullptr, nullptr);
        }

        if (!Content::has<sf::Music>(m_backgroundMusicKey))
        {
            Content::load<sf::Music>(
                m_backgroundMusicKey,
                Configuration::get<std::string>(m_backgroundMusicPath),
                nullptr, nullptr);
        }
    }

    // --------------------------------------------------------------
    //
    // Only thing done here is to determine if a new powerup should
    // be created.
    //
    // --------------------------------------------------------------
    void Level::update(const std::chrono::microseconds elapsedTime)
    {
        updatePowerup<entities::PowerupBomb>(elapsedTime, m_timeBombPowerup, m_nextBombPowerup, m_nextBombPowerupCompute);
        updatePowerup<entities::PowerupRapidFire>(elapsedTime, m_timeRapidFirePowerup, m_nextRapidFirePowerup, m_nextRapidFirePowerupCompute);
        updatePowerup<entities::PowerupSpreadFire>(elapsedTime, m_timeSpreadFirePowerup, m_nextSpreadFirePowerup, m_nextSpreadFirePowerupCompute);
    }
} // namespace levels
