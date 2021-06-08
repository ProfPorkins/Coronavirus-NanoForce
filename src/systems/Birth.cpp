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

#include "Birth.hpp"

#include "components/Age.hpp"
#include "components/Birth.hpp"

#include <algorithm>

namespace systems
{
    void Birth::update(const std::chrono::microseconds elapsedTime)
    {
        for (auto&& [id, entity] : m_entities)
        {
            auto age = entity->getComponent<components::Age>();
            auto birth = entity->getComponent<components::Birth>();

            birth->update(elapsedTime);

            //
            // Check to see if it gave birth
            if (birth->isGestating() && birth->getCurrentGestation() <= std::chrono::microseconds(0))
            {
                // Congratulations, a bouncing baby virus!
                m_onBirth(entity->getId());

                birth->resetGestation();
            }

            //
            // Check to see if a new gestation should begin
            if (age->get() >= birth->getMinAge() && !birth->isGestating())
            {
                auto time = std::chrono::microseconds(static_cast<int>(birth->getDistribution()(m_generator)));
                time = std::max(birth->getGestationMin(), time);
                birth->setGestationTime(time);
            }
        }
    }

} // namespace systems