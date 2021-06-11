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

#include "PetriDish.hpp"

#include "components/Momentum.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "entities/Entity.hpp"
#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"

#include <chrono>

namespace levels
{
    PetriDish::PetriDish(std::string key, bool training) :
        Level(key),
        m_training(training),
        m_generator(m_rd()),
        m_distUniform(0.0f, 1.0f),
        m_distCircle(0.0f, 2 * 3.14159f)
    {
        m_backgroundSize = { 100.0f, 100.0f };
    }

    // --------------------------------------------------------------
    //
    // Generate some initial viruses to get the level started.
    // Randomly choose the age of the viruses, so there is a
    // good mix of ages.
    //
    // --------------------------------------------------------------
    std::vector<std::shared_ptr<entities::Virus>> PetriDish::initializeViruses()
    {
        std::vector<std::shared_ptr<entities::Virus>> viruses;

        //
        // Start out with X viruses, at randomly chosen locations
        auto arenaDistance = std::min(Configuration::getGraphics().getViewCoordinates().width, Configuration::getGraphics().getViewCoordinates().height);
        arenaDistance /= 2;    // Starting from center of the arena, therefore use half the distance
        arenaDistance *= 0.75; // Additional scaling to not be right on the edge at the start
        for (int i = 1; i <= m_initialVirusCount; i++)
        {
            // All viruses in the training levels start at age 0, while the patient levels have a distribution of starting ages
            std::shared_ptr<entities::Virus> virus = nullptr;
            if (m_training)
            {
                virus = std::make_shared<entities::Virus>();
            }
            else
            {
                // Choose an age
                auto maxAge = misc::msTous(std::chrono::milliseconds(Configuration::get<std::uint32_t>(config::VIRUS_AGE_MATURITY)));
                auto age = std::chrono::duration_cast<std::chrono::microseconds>(m_distUniform(m_generator) * maxAge);
                virus = std::make_shared<entities::Virus>(age);
            }
            //
            // Choose a random angle
            auto angle = m_distCircle(m_generator);

            //
            // Choose a distance along that angle for the position
            auto distance = m_distUniform(m_generator);
            auto point = math::Point2f{
                std::cos(angle) * distance * arenaDistance,
                std::sin(angle) * distance * arenaDistance
            };

            virus->getComponent<components::Position>()->set(point);
            viruses.push_back(virus);
        }

        return viruses;
    }

    // --------------------------------------------------------------
    //
    // A safe start is defined as no viruses within 4X (the size of
    // the player) distance of the player.
    //
    // The center is the preferred start location, but if it isn't available, then
    // start searching elsewhere after some time has passed trying the center.
    //
    // --------------------------------------------------------------
    std::optional<math::Point2f> PetriDish::findSafeStart(std::chrono::microseconds howLongWaiting, const std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Entity>>& viruses)
    {
        const float shipSize = Configuration::get<float>(config::PLAYER_SIZE);

        auto getMinDistance = [](math::Point2f position, const std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Entity>>& viruses) {
            auto minDistance = std::numeric_limits<float>::max();
            for (auto&& [id, virus] : viruses)
            {
                auto vPosition = virus->getComponent<components::Position>()->get();
                auto x = position.x - vPosition.x;
                auto y = position.y - vPosition.y;
                float distance = std::sqrt(x * x + y * y);
                minDistance = std::min(minDistance, distance);
            }
            return minDistance;
        };

        // Wait a little while for the center, before trying to find another location
        if (howLongWaiting <= misc::msTous(std::chrono::microseconds(6000)))
        {
            if (getMinDistance({ 0, 0 }, viruses) >= 5 * shipSize)
            {
                return math::Point2f{ 0.0f, 0.0f };
            }
            return std::nullopt;
        }
        else
        {
            //
            // Start searching other locations.  Want to find the largest,
            // minimum distance to the viruses.
            math::Point2f bestPosition;
            float bestDistance = std::numeric_limits<float>::min();

            for (float radian = 0; radian < 2 * 3.14159f; radian += (2 * 3.14159f) / 40)
            {
                auto position1 = math::Point2f{ std::cos(radian) * 20, std::sin(radian) * 20 };
                auto position2 = math::Point2f{ std::cos(radian) * 40, std::sin(radian) * 40 };
                auto distance1 = getMinDistance(position1, viruses);
                auto distance2 = getMinDistance(position2, viruses);
                if (distance1 > bestDistance && distance1 > distance2)
                {
                    bestDistance = distance1;
                    bestPosition = position1;
                }
                else if (distance2 > bestDistance && distance2 > distance1)
                {
                    bestDistance = distance2;
                    bestPosition = position2;
                }
            }
            if (bestDistance >= 4 * shipSize)
            {
                return bestPosition;
            }
        }

        return std::nullopt;
    }

    bool PetriDish::collidesWithBorder(entities::Entity& entity)
    {
        static auto const ARENA_RADIUS = Configuration::getGraphics().getViewCoordinates().height / 2.0f - 4.0f;

        auto position = entity.getComponent<components::Position>();
        auto radius = entity.getComponent<components::Size>()->getOuterRadius();

        auto distance = std::sqrt(position->get().x * position->get().x + position->get().y * position->get().y);
        // Add in half of its outer radius to account for the size
        distance += (radius / 2.0f);

        return distance > ARENA_RADIUS;
    }

    void PetriDish::bounceOffBorder(entities::Entity& entity)
    {
        //
        // Reflect the direction of the ship.  Have to take negative
        // vectors because we are inside the circle, not outside.
        auto momentum = entity.getComponent<components::Momentum>();
        auto l = math::Vector2f(-momentum->get().x, -momentum->get().y);
        //
        // Normal to the circle is the vector formed by position of the player (more or less)
        // from the center of the circle, which is just the position of the player.
        momentum->set(math::reflect(entity.getComponent<components::Position>()->get(), l));
    }

    math::Point2f PetriDish::computePowerupPosition()
    {
        // Lookup the arena distance
        auto arenaDistance = std::min(Configuration::getGraphics().getViewCoordinates().width, Configuration::getGraphics().getViewCoordinates().height);
        arenaDistance /= 2;    // Starting from center of the arena, therefore use half the distance
        arenaDistance *= 0.75; // Additional scaling to not be right on the edge at the start
        //
        // Choose a random angle
        auto angle = m_distCircle(m_generator);

        //
        // Choose a distance along that angle for the position
        auto distance = m_distUniform(m_generator);
        auto point = math::Point2f{
            std::cos(angle) * distance * arenaDistance,
            std::sin(angle) * distance * arenaDistance
        };

        return point;
    }

} // namespace levels