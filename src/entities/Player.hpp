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

#include "entities/Bullet.hpp"
#include "entities/Entity.hpp"
#include "entities/Powerup.hpp"
#include "entities/Weapon.hpp"

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

        auto getPrimaryWeapon() { return m_weaponPrimary; }
        auto getSecondaryWeapon() { return m_weaponSecondary; }
        void applyPowerup(std::shared_ptr<entities::Powerup> powerup);

      private:
        std::shared_ptr<entities::Weapon> m_weaponPrimary;
        std::shared_ptr<entities::Weapon> m_weaponSecondary;

        void attachPrimaryWeapon(std::shared_ptr<entities::Weapon> weapon);
        void attachSecondaryWeapon(std::shared_ptr<entities::Weapon> weapon);
    };

} // namespace entities
