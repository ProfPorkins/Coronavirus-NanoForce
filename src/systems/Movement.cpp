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

#include "Movement.hpp"

#include "components/Drag.hpp"
#include "components/Momentum.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "services/Configuration.hpp"

namespace systems
{
    void updateEntity(levels::Level& level, entities::Entity& entity, const std::chrono::microseconds elapsedTime, bool testBorder = true);
    void testArenaBorder(levels::Level& level, entities::Entity& entity, const std::chrono::microseconds elapsedTime);

    void updateEntity(levels::Level& level, entities::Entity& entity, const std::chrono::microseconds elapsedTime, bool testBorder)
    {
        auto momentum = entity.getComponent<components::Momentum>();
        auto position = entity.getComponent<components::Position>();

        position->set({ position->get().x + momentum->get().x * elapsedTime.count(), position->get().y + momentum->get().y * elapsedTime.count() });

        if (entity.hasComponent<components::Orientation>())
        {
            auto orientation = entity.getComponent<components::Orientation>();
            orientation->set(orientation->get() + momentum->getRotateRate() * elapsedTime.count());
        }

        // Apply drag to the entity.
        if (entity.hasComponent<components::Drag>())
        {
            // Apply it in the direction of momentum
            auto magnitude = std::sqrt(momentum->get().x * momentum->get().x + momentum->get().y * momentum->get().y);
            magnitude -= static_cast<decltype(magnitude)>(entity.getComponent<components::Drag>()->get() * elapsedTime.count());
            magnitude = std::max(0.0f, magnitude);

            // A little indirect: Convert the momentum vector into an angle, then convert back to
            // a vector, so we get a unit vector as a result.  Could do this another way, but
            // performance isn't an issue here and this works just great.
            auto vector = math::vectorFromRadians(std::atan2(momentum->get().y, momentum->get().x));
            momentum->set({ vector.x * magnitude, vector.y * magnitude });
        }

        //
        // This will/should only ever be false when recursively invoked from 'testArenaBorder' itself.
        if (testBorder)
        {
            testArenaBorder(level, entity, elapsedTime);
        }
    }

    // --------------------------------------------------------------
    //
    // Returns true if the entity hit the arena border.
    //
    // --------------------------------------------------------------
    void testArenaBorder(levels::Level& level, entities::Entity& entity, const std::chrono::microseconds elapsedTime)
    {
        if (level.collidesWithBorder(entity))
        {
            //
            // Need to reflect the entity with the arena border
            level.bounceOffBorder(entity);
            //
            // After reflecting, have to move it a little bit, so it doesn't get stuck on the border.
            updateEntity(level, entity, elapsedTime, false);
        }
    }

    //void Movement::update(levels::Level& level, const std::chrono::microseconds elapsedTime, std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Entity>>& entities)
    //{
    //    for (auto&& [id, entity] : entities)
    //    {
    //        updateEntity(level, *entity, elapsedTime);
    //    }
    //}

    //void Movement::update(levels::Level& level, const std::chrono::microseconds elapsedTime, std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Virus>>& entities)
    //{
    //    for (auto&& [id, entity] : entities)
    //    {
    //        updateEntity(level, *std::static_pointer_cast<entities::Entity>(entity), elapsedTime);
    //    }
    //}

    //void Movement::update(levels::Level& level, entities::Entity& entity, const std::chrono::microseconds elapsedTime)
    //{
    //    updateEntity(level, entity, elapsedTime);
    //}

    void Movement::update(levels::Level& level, std::chrono::microseconds elapsedTime)
    {
        for (auto&& [id, entity] : m_entities)
        {
            (void)id; // unused
            updateEntity(level, *entity, elapsedTime);
        }
    }

} // namespace systems