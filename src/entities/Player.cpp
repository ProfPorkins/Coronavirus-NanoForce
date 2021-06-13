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

#include "Player.hpp"

#include "components//Audio.hpp"
#include "components/Collidable.hpp"
#include "components/Drag.hpp"
#include "components/Momentum.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Powerup.hpp"
#include "components/Size.hpp"
#include "components/Sprite.hpp"
#include "components/ControlParams.hpp"
#include "entities/WeaponBomb.hpp"
#include "entities/WeaponEmpty.hpp"
#include "entities/WeaponGun.hpp"
#include "entities/WeaponRapidFire.hpp"
#include "entities/WeaponSpreadFire.hpp"
#include "misc/math.hpp"
#include "misc/misc.hpp"
#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"
#include "services/ContentKey.hpp"
#include "services/SoundPlayer.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

namespace entities
{

    // --------------------------------------------------------------
    //
    // Create a player based on details from the configuration file.
    //
    // --------------------------------------------------------------
    std::shared_ptr<Player> Player::create()
    {
        Specification spec;
        spec.thrustRate = Configuration::get<double>(config::PLAYER_THRUST_RATE);
        spec.dragRate = Configuration::get<double>(config::PLAYER_DRAG_RATE) * misc::PER_MS_TO_US;
        spec.rotateRate = Configuration::get<float>(config::PLAYER_ROTATE_RATE);
        spec.maxSpeed = Configuration::get<float>(config::PLAYER_MAX_SPEED);
        spec.size = Configuration::get<float>(config::PLAYER_SIZE);

        auto player = std::make_shared<Player>(spec);

        player->attachPrimaryWeapon(std::make_shared<entities::WeaponGun>(config::ENTITY_WEAPON_BASIC_GUN));
        player->attachSecondaryWeapon(std::make_shared<entities::WeaponEmpty>());

        return player;
    }

    Player::Player(Specification spec)
    {
        this->addComponent(std::make_unique<components::Position>(math::Point2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Orientation>(0.0f));
        this->addComponent(std::make_unique<components::Size>(math::Dimension2f(spec.size, spec.size)));
        this->addComponent(std::make_unique<components::Momentum>(math::Vector2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::ControlParams>(spec.thrustRate, spec.rotateRate, spec.maxSpeed));
        this->addComponent(std::make_unique<components::Drag>(spec.dragRate));
        this->addComponent(std::make_unique<components::Sprite>(Content::get<sf::Texture>(content::KEY_IMAGE_PLAYER)));
        this->addComponent(std::make_unique<components::Collidable>(components::Collidable::Type::Player));

        //
        // This is a unique one, due to the way it works, handing it directly here, rather than a
        // fire-and-forget using the SoundBuffer.
        m_thrust.setBuffer(*Content::get<sf::SoundBuffer>(content::KEY_AUDIO_THRUST));
    }

    // --------------------------------------------------------------
    //
    // Due to circular pointers between weapons and the player, the shared_ptr for a player
    // won't automatically go out of scope.  Therefore, this method is used to tell a player
    // to cleanup after itself.
    //
    // --------------------------------------------------------------
    void Player::cleanup()
    {
        m_weaponPrimary = nullptr;
        m_weaponSecondary = nullptr;
    }

    Player::~Player()
    {
        // The thrust sound can still be playing when the player dies because the key
        // hasn't been released.  This ensures the thrust sounds stops when the player
        // no longer exists.
        m_thrust.stop();
    }

    // --------------------------------------------------------------
    //
    // Player needs to update because thrust is based on a start/stop
    // from a key being pressed or released.
    //
    // --------------------------------------------------------------
    void Player::update(const std::chrono::microseconds elapsedTime)
    {
        if (m_thrusting)
        {
            accelerate(elapsedTime);
        }
    }

    void Player::applyPowerup(std::shared_ptr<entities::Powerup> powerup)
    {
        SoundPlayer::play(powerup->getComponent<components::Audio>()->get());

        switch (powerup->getComponent<components::Powerup>()->get())
        {
            case components::Powerup::Type::RapidFire:
            {
                auto weapon = std::make_shared<WeaponRapidFire>(config::ENTITY_WEAPON_RAPID_FIRE);
                this->attachPrimaryWeapon(weapon);
            }
            break;
            case components::Powerup::Type::SpreadFire:
            {
                auto weapon = std::make_shared<WeaponSpreadFire>(config::ENTITY_WEAPON_SPREAD_FIRE);
                this->attachPrimaryWeapon(weapon);
            }
            break;
            case components::Powerup::Type::Bomb:
            {
                auto weapon = std::make_shared<WeaponBomb>(config::ENTITY_WEAPON_BOMB);
                this->attachSecondaryWeapon(weapon);
            }
            break;
        }
    }

    void Player::attachPrimaryWeapon(std::shared_ptr<entities::Weapon> weapon)
    {
        weapon->setParent(shared_from_this());
        m_weaponPrimary = weapon;
    }

    void Player::attachSecondaryWeapon(std::shared_ptr<entities::Weapon> weapon)
    {
        weapon->setParent(shared_from_this());
        m_weaponSecondary = weapon;
    }

} // namespace entities
