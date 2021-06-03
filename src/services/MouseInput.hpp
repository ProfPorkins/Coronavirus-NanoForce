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

#include <SFML/Window/Event.hpp>
#include <chrono>
#include <functional>
#include <string>
#include <unordered_map>

// --------------------------------------------------------------
//
// Mouse input handler
//
// Note: This is a Singleton
//
// --------------------------------------------------------------
class MouseInput
{
  public:
    MouseInput(const MouseInput&) = delete;
    MouseInput(MouseInput&&) = delete;
    MouseInput& operator=(const MouseInput&) = delete;
    MouseInput& operator=(MouseInput&&) = delete;

    static auto& instance()
    {
        static MouseInput instance;
        return instance;
    }

    bool initialize();

    std::uint32_t registerKeyPressedHandler(std::function<void(sf::Keyboard::Key)> handler);
    std::uint32_t registerKeyReleasedHandler(std::function<void(sf::Keyboard::Key)> handler);

    void unregisterKeyPressedHandler(std::uint32_t id);
    void unregisterKeyReleasedHandler(std::uint32_t id);

    void update(const std::chrono::microseconds elapsedTime);

  private:
    MouseInput() {}

    std::uint32_t nextId{ 0 };

    auto getNextId() { return nextId++; }
};
