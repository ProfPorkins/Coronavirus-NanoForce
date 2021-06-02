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

#include "Bomb.hpp"

#include "components/Damage.hpp"
#include "components/Lifetime.hpp"
#include "components/Momentum.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "entities/Bullet.hpp"
#include "misc/math.hpp"
#include "misc/misc.hpp"
#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/ContentKey.hpp"
#include "services/SoundPlayer.hpp"

namespace entities
{

    Bomb::Bomb(std::chrono::microseconds lifetime, float size, std::function<void(std::shared_ptr<entities::Entity>&)> emitBullet)
    {
        using namespace std::string_literals;
        using namespace config;

        static const config_path BOMB_BULLET_COUNT = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_BULLETS, "count"s };
        static const config_path BOMB_BULLET_DAMAGE = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_BULLETS, DOM_DAMAGE };
        static const config_path BOMB_BULLET_SIZE = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_BULLETS, DOM_SIZE };
        static const config_path BOMB_BULLET_LIFETIME = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_BULLETS, DOM_LIFETIME };

        m_bulletCount = Configuration::get<std::uint16_t>(BOMB_BULLET_COUNT);
        m_bulletDamage = Configuration::get<std::uint16_t>(BOMB_BULLET_DAMAGE);
        m_bulletSize = Configuration::get<float>(BOMB_BULLET_SIZE);
        m_bulletLifetime = misc::msTous(Configuration::get<std::chrono::milliseconds>(BOMB_BULLET_LIFETIME));

        this->addComponent(std::make_unique<components::Position>(math::Point2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Size>(math::Dimension2f(size, size)));
        this->addComponent(std::make_unique<components::Momentum>(math::Vector2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Lifetime>(lifetime, [this, emitBullet]() { explode(emitBullet); }));
        this->addComponent(std::make_unique<components::Orientation>(0.0f));
    }

    void Bomb::explode(std::function<void(std::shared_ptr<entities::Entity>&)> emit)
    {
        SoundPlayer::play(content::KEY_AUDIO_BOMB_EXPLODE);

        auto angle = 0.0f;
        auto angleDiff = (2.0f * 3.14159f) / m_bulletCount;
        for (int i = 1; i <= m_bulletCount; i++)
        {
            angle += angleDiff;
            auto bullet = std::make_shared<entities::Bullet>(m_bulletDamage, m_bulletLifetime, m_bulletSize);

            bullet->getComponent<components::Position>()->set(this->getComponent<components::Position>()->get());
            // Scale the bomb momentum appropriate for its speed
            auto vector = math::Vector2f{ std::cos(angle) * 0.00002f, std::sin(angle) * 0.00002f };
            bullet->getComponent<components::Momentum>()->set(vector);

            auto e = std::static_pointer_cast<entities::Entity>(bullet);
            emit(e);
        }
    }

} // namespace entities
