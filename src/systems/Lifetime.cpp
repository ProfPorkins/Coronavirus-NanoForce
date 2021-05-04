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

#include "Lifetime.hpp"

#include "components/Lifetime.hpp"

namespace systems
{
    // --------------------------------------------------------------
    //
    // Yes, duplicated code!! (I'm not happy or proud of it)
    //
    // I have tried to make this templated, but even templated
    // there are compiler problems with g++.  With g++ this method
    // won't compile the entity->getCompoennt<> when the body of the code is
    // in the templated method.  I was able to write it as two template
    // specializations, but that is basically the same and writing two
    // overloads, so I went with the overloads.
    //
    // --------------------------------------------------------------
    void Lifetime::update(const std::chrono::microseconds elapsedTime, std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Entity>>& entities)
    {
        std::vector<entities::Entity::IdType> removeThese;
        for (auto&& [id, entity] : entities)
        {
            auto lifetime = entity->getComponent<components::Lifetime>();
            lifetime->update(elapsedTime);
            if (!lifetime->isAlive())
            {
                lifetime->endOfLife();
                removeThese.push_back(id);
            }
        }

        for (auto&& id : removeThese)
        {
            entities.erase(id);
        }
    }

    void Lifetime::update(const std::chrono::microseconds elapsedTime, std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Powerup>>& entities)
    {
        std::vector<entities::Entity::IdType> removeThese;
        for (auto&& [id, entity] : entities)
        {
            auto lifetime = entity->getComponent<components::Lifetime>();
            lifetime->update(elapsedTime);
            if (!lifetime->isAlive())
            {
                lifetime->endOfLife();
                removeThese.push_back(id);
            }
        }

        for (auto&& id : removeThese)
        {
            entities.erase(id);
        }
    }
} // namespace systems