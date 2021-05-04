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

#include "Age.hpp"

#include "components/Age.hpp"
#include "components/Size.hpp"

#include <algorithm>

namespace systems
{
    void Age::update(const std::chrono::microseconds elapsedTime, std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Virus>>& entities)
    {
        for (auto&& [id, entity] : entities)
        {
            auto age = entity->getComponent<components::Age>();
            auto size = entity->getComponent<components::Size>();

            age->update(elapsedTime);

            auto sizeUpdate = math::lerp(age->get(), std::chrono::microseconds(0), age->getMaturity(), age->getMinSize(), age->getMaxSize());
            sizeUpdate = std::min(age->getMaxSize(), sizeUpdate); // Cap the max size
            size->set({ sizeUpdate, sizeUpdate });
        }
    }

} // namespace systems