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

#include "WeaponBomb.hpp"

#include "components/Momentum.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "entities/Bomb.hpp"
#include "misc/math.hpp"
#include "services/ContentKey.hpp"

namespace entities
{

    WeaponBomb::WeaponBomb(std::string key) :
        Weapon(key)
    {
        m_soundKey = content::KEY_AUDIO_BOMB_FIRE;
    }

    void WeaponBomb::fireImpl(std::function<void(std::shared_ptr<entities::Entity>&)>& emitBullet, std::function<void(std::shared_ptr<entities::Entity>&)>& emitBomb)
    {
        auto bomb = std::make_shared<entities::Bomb>(m_itemLifetime, m_itemSize, emitBullet);

        //
        // Move the position to be right at the end of the player's ship
        auto vector = math::vectorFromDegrees(m_parent->getComponent<components::Orientation>()->get());
        auto position = m_parent->getComponent<components::Position>()->get();
        position.x += vector.x * (m_parent->getComponent<components::Size>()->get().width / 2.0f);
        position.y += vector.y * (m_parent->getComponent<components::Size>()->get().height / 2.0f);
        bomb->getComponent<components::Position>()->set(position);

        //
        // Add an additional bit of momentum so it moves faster than the player's ship
        auto momentum = m_parent->getComponent<components::Momentum>()->get();
        momentum.x += vector.x * 0.000025f;
        momentum.y += vector.y * 0.000025f;
        bomb->getComponent<components::Momentum>()->set(momentum);

        auto b = std::static_pointer_cast<entities::Entity>(bomb);
        emitBomb(b);
    }

} // namespace entities
