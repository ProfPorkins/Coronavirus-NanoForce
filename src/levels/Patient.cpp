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

#include "Patient.hpp"

#include "Configuration.hpp"
#include "ConfigurationPath.hpp"
#include "components/Position.hpp"

#include <chrono>

namespace levels
{
    Patient::Patient(std::function<void(std::shared_ptr<entities::Powerup>&)> emitPowerup, std::string key) :
        Circle(emitPowerup, key)
    {
    }

    // --------------------------------------------------------------
    //
    // Generate some initial viruses to get the level started.
    // Randomly choose the age of the viruses, so there is a
    // good mix of ages.
    //
    // --------------------------------------------------------------
    std::vector<std::shared_ptr<entities::Virus>> Patient::initializeViruses()
    {
        std::vector<std::shared_ptr<entities::Virus>> viruses;

        //
        // Start out with X viruses, at randomly chosen locations
        auto arenaDistance = std::min(Configuration::getGraphics().getViewCoordinates().width, Configuration::getGraphics().getViewCoordinates().height);
        arenaDistance /= 2;    // Starting from center of the arena, therefore use half the distance
        arenaDistance *= 0.75; // Additional scaling to not be right on the edge at the start
        for (int i = 1; i <= m_initialVirusCount; i++)
        {
            // Choose an age
            auto maxAge = misc::msTous(std::chrono::milliseconds(Configuration::get<std::uint32_t>(config::VIRUS_AGE_MATURITY)));
            auto age = std::chrono::duration_cast<std::chrono::microseconds>(m_distUniform(m_generator) * maxAge);
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

            auto virus = entities::Virus::create(age);
            virus->getComponent<components::Position>()->set(point);
            viruses.push_back(virus);
        }

        return viruses;
    }

} // namespace levels