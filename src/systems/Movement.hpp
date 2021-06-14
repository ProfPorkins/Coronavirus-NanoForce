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
#include "components/Momentum.hpp"
#include "components/Position.hpp"
#include "entities/Entity.hpp"
#include "levels/Level.hpp"

#include <chrono>
#include <memory>

namespace systems
{
    class Movement : public System
    {
      public:
        Movement(levels::Level& level) :
            System({ ctti::unnamed_type_id<components::Position>(),
                     ctti::unnamed_type_id<components::Momentum>() }),
            m_level(level)
        {
        }

        virtual void update(std::chrono::microseconds elapsedTime) override;

      private:
        levels::Level& m_level;

        void updateEntity(entities::Entity& entity, const std::chrono::microseconds elapsedTime, bool testBorder = true);
        void testArenaBorder(entities::Entity& entity, const std::chrono::microseconds elapsedTime);
    };
} // namespace systems
