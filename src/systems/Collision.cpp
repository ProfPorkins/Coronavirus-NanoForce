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

#include "Collision.hpp"

#include "components/Bullets.hpp"
#include "components/Damage.hpp"
#include "components/Health.hpp"
#include "entities/Player.hpp"
#include "entities/Powerup.hpp"
#include "misc/math.hpp"

#include <memory>
#include <optional>

namespace systems
{
    // --------------------------------------------------------------
    //
    // This system groups entities by the collidable (which is entity) type
    //
    // --------------------------------------------------------------
    bool Collision::addEntity(std::shared_ptr<entities::Entity> entity)
    {
        if (System::addEntity(entity))
        {
            switch (entity->getComponent<components::Collidable>()->get())
            {
                case components::Collidable::Type::Bullet:
                    m_bullets[entity->getId()] = entity;
                    break;
                case components::Collidable::Type::Virus:
                    m_viruses[entity->getId()] = entity;
                    break;
                case components::Collidable::Type::Powerup:
                    m_powerups[entity->getId()] = entity;
                    break;
                case components::Collidable::Type::Player:
                    m_player = entity;
                    break;
            }
            return true;
        }

        return false;
    }

    // --------------------------------------------------------------
    //
    // Remove from the appropriate type collection.
    //
    // --------------------------------------------------------------
    void Collision::removeEntity(entities::Entity::IdType entityId)
    {
        auto entity = m_entities[entityId];
        if (entity != nullptr)
        {
            switch (entity->getComponent<components::Collidable>()->get())
            {
                case components::Collidable::Type::Bullet:
                    m_bullets.erase(entityId);
                    break;
                case components::Collidable::Type::Virus:
                    m_viruses.erase(entityId);
                    break;
                case components::Collidable::Type::Powerup:
                    m_powerups.erase(entityId);
                    break;
                case components::Collidable::Type::Player:
                    m_player = nullptr;
                    break;
            }
        }
        System::removeEntity(entityId);
    }

    void Collision::update([[maybe_unused]] const std::chrono::microseconds elapsedTime)
    {
        checkPlayerCollision();
        checkBulletCollision();
    }

    // --------------------------------------------------------------
    //
    // Bullets can collide with only viruses
    //
    // --------------------------------------------------------------
    void Collision::checkBulletCollision()
    {
        //
        // Let's see if any bullets hit any viruses
        std::vector<entities::Entity::IdType> bulletsToRemove;
        // Using a set for the dead viruses so that duplicates don't happen, plus want to
        // wait to remove them until after iterating through everything.
        std::unordered_set<entities::Entity::IdType> deadViruses;
        for (auto&& [bulletId, bullet] : m_bullets)
        {
            for (auto&& [virusId, virus] : m_viruses)
            {
                if (math::collides(*std::static_pointer_cast<entities::Entity>(bullet), *std::static_pointer_cast<entities::Entity>(virus)))
                {
                    bulletsToRemove.push_back(bulletId);
                    virus->getComponent<components::Bullets>()->add();
                    auto damage = bullet->getComponent<components::Damage>();
                    auto health = virus->getComponent<components::Health>();
                    health->subtract(damage->get());
                    if (health->get() <= 0)
                    {
                        deadViruses.insert(virusId);
                        // Don't check anymore viruses for this bullet
                        break;
                    }
                }
            }
        }
        for (auto&& id : bulletsToRemove)
        {
            m_removeEntity(id);
        }

        for (auto&& id : deadViruses)
        {
            m_onVirusDeath(m_viruses[id].get());
            m_removeEntity(id);
        }
    }

    // --------------------------------------------------------------
    //
    // Player can collide with powerups or viruses
    //
    // --------------------------------------------------------------
    void Collision::checkPlayerCollision()
    {
        if (m_player)
        {
            // Let's see if the player picked up any powerups
            std::optional<entities::Entity::IdType> powerupToRemove;
            for (auto&& [id, powerup] : m_powerups)
            {
                if (math::collides(*m_player, *powerup))
                {
                    // Apply the powerup to the player
                    std::static_pointer_cast<entities::Player>(m_player)->applyPowerup(std::static_pointer_cast<entities::Powerup>(powerup));
                    powerupToRemove = id;
                }
            }
            if (powerupToRemove.has_value())
            {
                m_removeEntity(powerupToRemove.value());
            }

            //
            // Check to see if any viruses hit the player
            for (auto&& [id, virus] : m_viruses)
            {
                if (math::collides(*m_player, *virus))
                {
                    m_onPlayerDeath();
                    break;
                }
            }
        }
    }

} // namespace systems