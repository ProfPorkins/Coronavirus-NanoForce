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

#include "entities//Powerup.hpp"
#include "entities/Bullet.hpp"
#include "entities/Entity.hpp"
#include "entities/Weapon.hpp"
#include "misc/misc.hpp"

#include <SFML/Audio/Sound.hpp>
#include <chrono>
#include <memory>

namespace entities
{
    // --------------------------------------------------------------
    //
    // The Player entity is the "ship" controlled by the person
    // playing the game.
    //
    // --------------------------------------------------------------
    class Player : public Entity
    {
      public:
        struct Specification
        {
            Specification() = default;

            double thrustRate;
            double dragRate;
            float rotateRate; // degrees per ms
            float maxSpeed;
            float size;
        };

      public:
        static std::shared_ptr<Player> create();
        Player(Specification spec);
        ~Player();

        void cleanup();

        void update(const std::chrono::microseconds elapsedTime);

        auto getPrimaryWeapon() { return m_weaponPrimary; }
        auto getSecondaryWeapon() { return m_weaponSecondary; }
        void applyPowerup(std::shared_ptr<entities::Powerup> powerup);

        void brake(std::chrono::microseconds elapsedTime);
        void rotateLeft(std::chrono::microseconds elapsedTime);
        void rotateRight(std::chrono::microseconds elapsedTime);

        void startThrust()
        {
            m_thrusting = true;
            m_thrust.play();
        }
        void endThrust()
        {
            m_thrusting = false;
            m_thrust.stop();
        }

      private:
        bool m_thrusting{ false };
        double m_thrustRate{ 0.0000001 * misc::PER_MS_TO_US }; // per us
        float m_rotateRate{ 0.0002f };                         // degrees per us
        float m_maxSpeed{ 0.00002f };                          // per us
        std::shared_ptr<entities::Weapon> m_weaponPrimary;
        std::shared_ptr<entities::Weapon> m_weaponSecondary;

        sf::Sound m_thrust;

        void accelerate(std::chrono::microseconds elapsedTime);
        void attachPrimaryWeapon(std::shared_ptr<entities::Weapon> weapon);
        void attachSecondaryWeapon(std::shared_ptr<entities::Weapon> weapon);
    };
} // namespace entities
