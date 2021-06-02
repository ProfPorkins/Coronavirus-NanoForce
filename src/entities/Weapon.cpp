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

#include "Weapon.hpp"

#include "misc/misc.hpp"
#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"
#include "services/ContentKey.hpp"
#include "services/SoundPlayer.hpp"

namespace entities
{

    void Weapon::fire(std::function<void(std::shared_ptr<entities::Entity>&)> emitBullet, std::function<void(std::shared_ptr<entities::Entity>&)> emitBomb)
    {
        // Verify the weapon can be fired by checking the time since last fired.
        auto now = std::chrono::system_clock::now();
        if (m_lastFire + m_fireDelay < now)
        {
            SoundPlayer::play(m_soundKey);
            m_lastFire = now;

            fireImpl(emitBullet, emitBomb);
        }
    }

    // --------------------------------------------------------------
    //
    // Each weapon has a handful of attributes in common, those are
    // grabbed from the configuration here.
    //
    // --------------------------------------------------------------
    void Weapon::loadAttributes(std::string key)
    {
        using namespace std::string_literals;
        using namespace config;

        const config_path WEAPON_DAMAGE = { DOM_ENTITY, key, DOM_DAMAGE };
        const config_path WEAPON_FIRE_DELAY = { DOM_ENTITY, key, DOM_FIRE_DELAY };
        const config_path WEAPON_ITEM_LIFETIME = { DOM_ENTITY, key, DOM_LIFETIME };
        const config_path WEAPON_ITEM_SIZE = { DOM_ENTITY, key, DOM_SIZE };

        m_fireDelay = misc::msTous(Configuration::get<std::chrono::milliseconds>(WEAPON_FIRE_DELAY));
        m_itemDamage = Configuration::get<std::uint16_t>(WEAPON_DAMAGE);
        m_itemLifetime = misc::msTous(Configuration::get<std::chrono::milliseconds>(WEAPON_ITEM_LIFETIME));
        m_itemSize = Configuration::get<float>(WEAPON_ITEM_SIZE);
    }

} // namespace entities
