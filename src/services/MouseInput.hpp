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

#include "misc/math.hpp"

#include <SFML/Window/Event.hpp>
#include <chrono>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

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
    using ButtonHandler = std::function<void(sf::Mouse::Button, math::Point2f, const std::chrono::microseconds)>;
    using MoveHandler = std::function<void(math::Point2f, const std::chrono::microseconds)>;

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

    std::uint32_t registerMouseMovedHandler(MoveHandler handler);
    std::uint32_t registerMousePressedHandler(ButtonHandler handler);
    std::uint32_t registerMouseReleasedHandler(ButtonHandler handler);

    void unregisterMouseMovedHandler(std::uint32_t id);
    void unregisterMousePressedHandler(std::uint32_t id);
    void unregisterMouseReleasedHandler(std::uint32_t id);

    void signalMouseMoved(math::Point2f point);
    void signalMousePressed(sf::Mouse::Button button, math::Point2f point);
    void signalMouseReleased(sf::Mouse::Button button, math::Point2f point);
    void update(const std::chrono::microseconds elapsedTime);

  private:
    MouseInput() {}

    struct ButtonInfo
    {
        ButtonInfo() = default;

        ButtonInfo(sf::Mouse::Button button, math::Point2f point) :
            button(button),
            point(point)
        {
        }

        sf::Mouse::Button button;
        math::Point2f point;
    };

    struct MoveInfo
    {
        MoveInfo() = default;

        MoveInfo(math::Point2f point) :
            point(point)
        {
        }

        math::Point2f point;
    };

    std::uint32_t nextId{ 0 };
    std::vector<MoveInfo> m_moved;
    std::vector<ButtonInfo> m_pressed;
    std::vector<ButtonInfo> m_released;

    std::unordered_map<std::uint32_t, MoveHandler> m_handlersMoved;
    std::unordered_map<std::uint32_t, ButtonHandler> m_handlersPressed;
    std::unordered_map<std::uint32_t, ButtonHandler> m_handlersReleased;

    auto getNextId() { return nextId++; }
};
