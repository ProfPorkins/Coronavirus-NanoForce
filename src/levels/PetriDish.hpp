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

#include "Level.hpp"
#include "entities/Entity.hpp"
#include "misc/math.hpp"

#include <memory>
#include <random>
#include <string>

namespace levels
{
    class PetriDish : public Level
    {
      public:
        PetriDish(std::string key, bool training);

        virtual std::vector<std::shared_ptr<entities::Virus>> initializeViruses() override;
        virtual std::optional<math::Point2f> findSafeStart(std::chrono::microseconds howLongWaiting, const std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Entity>>& viruses) override;
        virtual bool collidesWithBorder(entities::Entity& entity) override;
        virtual void bounceOffBorder(entities::Entity& entity) override;

      protected:
        virtual math::Point2f computePowerupPosition() override;

      private:
        bool m_training{ false };
        std::random_device m_rd;
        std::mt19937 m_generator;
        std::uniform_real_distribution<float> m_distUniform;
        std::uniform_real_distribution<float> m_distCircle;
    };
} // namespace levels