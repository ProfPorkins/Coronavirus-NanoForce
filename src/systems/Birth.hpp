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
#include "components/Age.hpp"
#include "components/Birth.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <random>
#include <unordered_map>

namespace systems
{
    class Birth : public System
    {
      public:
        Birth(std::function<void(std::shared_ptr<entities::Entity>)> onBirth) :
            System({ ctti::unnamed_type_id<components::Age>(),
                     ctti::unnamed_type_id<components::Birth>() }),
            m_onBirth(onBirth)
        {
        }

        virtual void update(const std::chrono::microseconds elapsedTime) override;

      private:
        std::function<void(std::shared_ptr<entities::Entity>)> m_onBirth;
        std::random_device m_rd;
        std::mt19937 m_generator;
    };
} // namespace systems
