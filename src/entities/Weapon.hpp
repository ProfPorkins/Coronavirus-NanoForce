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

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace entities
{
    class Weapon : public Entity
    {
      public:
        Weapon() = default;
        Weapon(std::string key) :
            m_lastFire(std::chrono::system_clock::now())
        {
            loadAttributes(key);
            // Subtract the fire delay so it can immediately fire
            m_lastFire -= m_fireDelay;
        }

        void setParent(std::shared_ptr<entities::Entity> parent) { m_parent = parent; }
        virtual void fire(std::function<void(std::shared_ptr<entities::Entity>&)> emitBullet, std::function<void(std::shared_ptr<entities::Entity>&)> emitBomb);

      protected:
        std::shared_ptr<entities::Entity> m_parent;
        std::chrono::system_clock::time_point m_lastFire;
        std::chrono::microseconds m_fireDelay{ 0 };

        std::uint16_t m_itemDamage{ 0 };
        std::chrono::microseconds m_itemLifetime{ 0 }; // Can't think of a better name?
        float m_itemSize{ 0 };

        std::string m_soundKey;

        void loadAttributes(std::string key);
        virtual void fireImpl([[maybe_unused]] std::function<void(std::shared_ptr<entities::Entity>&)>& emitBullet, [[maybe_unused]] std::function<void(std::shared_ptr<entities::Entity>&)>& emitBomb){};
    };
} // namespace entities
