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

#include "Powerup.hpp"

#include "components/Powerup.hpp"
#include "entities/PowerupBomb.hpp"
#include "entities/PowerupRapidFire.hpp"
#include "entities/PowerupSpreadFire.hpp"
#include "misc/misc.hpp"
#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"

#include <chrono>
#include <functional>
#include <random>

namespace systems
{
    Powerup::Powerup(levels::Level& level, std::function<void(std::shared_ptr<entities::Powerup>&)> emitPowerup, const std::string levelKey) :
        System({}),
        m_level(level),
        m_emitPowerup(emitPowerup),
        m_generator(m_rd()),
        m_distUniform(0.0f, 1.0f)
    {
        using namespace std::string_literals;
        using namespace config;

        const config_path MIN_POWERUP_TIME = { DOM_LEVELS, levelKey, DOM_SETTINGS, "min-powerup-time"s };
        const config_path BOMB_POWERUP_TIME = { DOM_LEVELS, levelKey, DOM_SETTINGS, "bomb-powerup-time"s };
        const config_path RAPID_FIRE_POWERUP_TIME = { DOM_LEVELS, levelKey, DOM_SETTINGS, "rapid-fire-powerup-time"s };
        const config_path SPREAD_FIRE_POWERUP_TIME = { DOM_LEVELS, levelKey, DOM_SETTINGS, "spread-fire-powerup-time"s };

        m_timeMinPowerup = misc::msTous(Configuration::get<std::chrono::milliseconds>(MIN_POWERUP_TIME));
        m_timeBombPowerup = misc::msTous(Configuration::get<std::chrono::milliseconds>(BOMB_POWERUP_TIME));
        m_timeRapidFirePowerup = misc::msTous(Configuration::get<std::chrono::milliseconds>(RAPID_FIRE_POWERUP_TIME));
        m_timeSpreadFirePowerup = misc::msTous(Configuration::get<std::chrono::milliseconds>(SPREAD_FIRE_POWERUP_TIME));
    }

    // --------------------------------------------------------------
    //
    // Only thing done here is to determine if a new powerup should
    // be created.
    //
    // --------------------------------------------------------------
    void Powerup::update(const std::chrono::microseconds elapsedTime)
    {
        updatePowerup<entities::PowerupBomb>(elapsedTime, m_timeBombPowerup, m_nextBombPowerup, m_nextBombPowerupCompute);
        updatePowerup<entities::PowerupRapidFire>(elapsedTime, m_timeRapidFirePowerup, m_nextRapidFirePowerup, m_nextRapidFirePowerupCompute);
        updatePowerup<entities::PowerupSpreadFire>(elapsedTime, m_timeSpreadFirePowerup, m_nextSpreadFirePowerup, m_nextSpreadFirePowerupCompute);
    }

} // namespace systems
