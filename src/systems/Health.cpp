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

#include "Health.hpp"

#include "components/Health.hpp"

namespace systems
{
    void Health::update(const std::chrono::microseconds elapsedTime)
    {
        for (auto&& [id, entity] : m_entities)
        {
            auto health = entity->getComponent<components::Health>();
            health->addElapsedIncrementTime(elapsedTime);

            if (health->getRemainingIncrements() > 0)
            {
                auto elapsedIncrementTime = health->getElapsedIncrementTime() + elapsedTime;
                // Compute how many health increments have occurred since the last update
                auto increments = static_cast<std::uint8_t>(elapsedIncrementTime / health->getIncrementTime());

                if (increments > 0)
                {
                    health->add(increments);
                    health->subtractRemainingIncrements(increments);
                    health->subtractElapsedIncrementTime(health->getIncrementTime() * increments);
                }
            }
        }
    }

} // namespace systems