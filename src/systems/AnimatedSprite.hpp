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

#include "components/AnimatedSprite.hpp"
#include "entities/Entity.hpp"

#include <chrono>
#include <memory>
#include <unordered_map>

namespace systems
{
    class AnimatedSprite
    {
      public:
        template <typename T>
        void update(const std::chrono::microseconds elapsedTime, std::unordered_map<entities::Entity::IdType, std::shared_ptr<T>>& entities)
        {
            for (auto&& [id, entity] : entities)
            {
                update(*std::static_pointer_cast<entities::Entity>(entity), elapsedTime);
            }
        }

      private:
        void update(entities::Entity& entity, const std::chrono::microseconds elapsedTime);
    };
} // namespace systems
