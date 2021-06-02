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

#include "Virus.hpp"

#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "components//Age.hpp"
#include "components/Birth.hpp"
#include "components/Drag.hpp"
#include "components/Health.hpp"
#include "components/Lifetime.hpp"
#include "components/Momentum.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "misc/math.hpp"

#include <cmath>

namespace entities
{
    // --------------------------------------------------------------
    //
    // Create a virus based on the 'type' from the configuration file.
    //
    // --------------------------------------------------------------
    Virus::Virus(std::chrono::microseconds age) :
        m_generator(m_rd()),
        m_distAngle(0.0f, 360.0f),
        m_distBoolean(0, 1)
    {
        Specification spec = readConfiguration();

        this->addComponent(std::make_unique<components::Position>(math::Point2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Orientation>(0.0f));
        this->addComponent(std::make_unique<components::Momentum>(math::Vector2f(0.0f, 0.0f), spec.rotateRate));

        // These two are needed for the Birth system
        this->addComponent(std::make_unique<components::Age>(age, spec.maxAge, spec.minSize, spec.maxSize));
        this->addComponent(std::make_unique<components::Birth>(
            spec.maxAge,
            spec.gestationMin,
            spec.gestationMean,
            spec.gestationStdev));

        // After adding the size component, have to set its size based upon its age
        auto size = math::lerp(age, std::chrono::microseconds(0), spec.maxAge, spec.minSize, spec.maxSize);
        this->addComponent(std::make_unique<components::Size>(math::Dimension2f(size, size)));

        // Add a health component - Note: Health on a virus increases with age
        this->addComponent(std::make_unique<components::Health>(spec.healthStart, spec.healthIncrements, spec.healthIncrementTime));
        // Get an initial path computed
        selectPath();
    }

    // --------------------------------------------------------------
    //
    // The number of bullets necessary to kill a virus depends on its size
    //
    // --------------------------------------------------------------
    void Virus::addBullet(std::shared_ptr<entities::Entity> bullet)
    {
        m_bullets.push_back(bullet);
    }

    // --------------------------------------------------------------
    //
    // Obtain the virus details from the configuration
    //
    // --------------------------------------------------------------
    Virus::Specification Virus::readConfiguration()
    {
        Virus::Specification spec;

        spec.minSize = Configuration::get<float>(config::VIRUS_SIZE_MIN);
        spec.maxSize = Configuration::get<float>(config::VIRUS_SIZE_MAX);
        spec.maxAge = misc::msTous(std::chrono::milliseconds(Configuration::get<std::uint32_t>(config::VIRUS_AGE_MATURITY)));
        spec.healthStart = Configuration::get<std::uint16_t>(config::VIRUS_HEALTH_START);
        spec.healthIncrements = Configuration::get<std::uint8_t>(config::VIRUS_HEALTH_INCREMENTS);
        spec.healthIncrementTime = misc::msTous(std::chrono::milliseconds(Configuration::get<std::uint32_t>(config::VIRUS_HEALTH_INCREMENT_TIME)));
        spec.gestationMin = misc::msTous(std::chrono::milliseconds(Configuration::get<std::uint32_t>(config::VIRUS_GEST_MIN)));
        spec.gestationMean = misc::msTous(std::chrono::milliseconds(Configuration::get<std::uint32_t>(config::VIRUS_GEST_MEAN)));
        spec.gestationStdev = misc::msTous(std::chrono::milliseconds(Configuration::get<std::uint32_t>(config::VIRUS_GEST_STDEV)));
        spec.rotateRate = Configuration::get<float>(config::VIRUS_ROTATE_RATE) * misc::PER_MS_TO_US;

        return spec;
    }

    // --------------------------------------------------------------
    //
    // Randomly choose a momentum vector
    //
    // --------------------------------------------------------------
    void Virus::selectPath()
    {
        auto momentumCmp = this->getComponent<components::Momentum>();
        auto angle = m_distAngle(m_generator);
        auto momentum = math::Vector2f(std::cos(angle), std::sin(angle));
        //
        // Have to scale back the magnitude of the momentum quite a bit
        auto speed = Configuration::get<float>(config::VIRUS_SPEED);
        momentum.x *= speed;
        momentum.y *= speed;
        momentumCmp->set(momentum);
        //
        // Choose a rotation direction +/-
        auto direction = m_distBoolean(m_generator);
        auto rotateRate = (direction == 0) ? -momentumCmp->getRotateRate() : momentumCmp->getRotateRate();
        momentumCmp->setRotateRate(rotateRate);
        // Bullets rotate the opposite direction
        m_bulletRotateRate = (direction == 0) ? m_bulletRotateRate : -m_bulletRotateRate;
    }

} // namespace entities
