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

#include "System.hpp"
#include "components/Collidable.hpp"
#include "entities/Entity.hpp"

#include <chrono>
#include <functional>
#include <memory>

namespace systems
{
    class Collision : public System
    {
      public:
        Collision(std::function<void(entities::Entity::IdType)> removeEntity, std::function<void(entities::Entity::IdType)> onVirusDeath, std::function<void()> onPlayerDeath) :
            System({ ctti::unnamed_type_id<components::Collidable>() }),
            m_removeEntity(removeEntity),
            m_onVirusDeath(onVirusDeath),
            m_onPlayerDeath(onPlayerDeath)
        {
        }

        virtual bool addEntity(std::shared_ptr<entities::Entity> entity) override;
        virtual void removeEntity(entities::Entity::IdType entityId) override;

        virtual void update(std::chrono::microseconds elapsedTime) override;

      private:
        std::function<void(entities::Entity::IdType)> m_removeEntity;
        std::function<void(entities::Entity::IdType)> m_onVirusDeath;
        std::function<void()> m_onPlayerDeath;

        entities::EntityMap m_viruses;
        entities::EntityMap m_bullets;
        entities::EntityMap m_powerups;
        std::shared_ptr<entities::Entity> m_player;

        void checkBulletCollision();
        void checkPlayerCollision();
    };
} // namespace systems
