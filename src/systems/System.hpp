#pragma once

#include "entities/Entity.hpp"

#include <chrono>
#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable : 4245) // Disable some compiler warnings that come from ctti
#endif
#include <ctti/type_id.hpp>
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif
#include <initializer_list>
#include <unordered_set>

namespace systems
{
    // --------------------------------------------------------------
    //
    // A system is where all logic associated with the game is handled.
    // Each system is specialized to operate over a particular set of
    // entities, handling things like movement, collision detection,
    // and rendering.
    //
    // --------------------------------------------------------------
    class System
    {
      public:
        System()
        {
        }

        System(const std::initializer_list<ctti::unnamed_type_id_t>& list) :
            m_interests(list)
        {
        }

        virtual bool addEntity(std::shared_ptr<entities::Entity> entity);
        virtual void removeEntity(entities::Entity::IdType entityId);

        virtual void update([[maybe_unused]] std::chrono::microseconds elapsedTime)
        {
        }

      protected:
        entities::EntityMap m_entities;

        virtual bool isInterested(entities::Entity* entity);

      private:
        std::unordered_set<ctti::unnamed_type_id_t> m_interests;
    };

} // namespace systems
