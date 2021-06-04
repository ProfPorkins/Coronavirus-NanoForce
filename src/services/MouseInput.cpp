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

#include "MouseInput.hpp"

bool MouseInput::initialize()
{
    return true;
}

std::uint32_t MouseInput::registerMouseMovedHandler(MoveHandler handler)
{
    auto id = getNextId();
    m_handlersMoved[id] = handler;

    return id;
}

std::uint32_t MouseInput::registerMousePressedHandler(ButtonHandler handler)
{
    auto id = getNextId();
    m_handlersPressed[id] = handler;

    return id;
}

std::uint32_t MouseInput::registerMouseReleasedHandler(ButtonHandler handler)
{
    auto id = getNextId();
    m_handlersReleased[id] = handler;

    return id;
}

void MouseInput::unregisterMouseMovedHandler(std::uint32_t id)
{
    m_handlersMoved.erase(id);
}

void MouseInput::unregisterMousePressedHandler(std::uint32_t id)
{
    m_handlersPressed.erase(id);
}

void MouseInput::unregisterMouseReleasedHandler(std::uint32_t id)
{
    m_handlersReleased.erase(id);
}

void MouseInput::signalMouseMoved(math::Point2f point)
{
    m_moved.push_back(point);
}

void MouseInput::signalMousePressed(sf::Mouse::Button button, math::Point2f point)
{
    m_pressed.push_back({ button, point });
}

void MouseInput::signalMouseReleased(sf::Mouse::Button button, math::Point2f point)
{
    m_released.push_back({ button, point });
}

void MouseInput::update(const std::chrono::microseconds elapsedTime)
{
    //
    // Invoke all button pressed handlers
    for (auto&& [button, point] : m_pressed)
    {
        for (auto&& [id, handler] : m_handlersPressed)
        {
            handler(button, point, elapsedTime);
        }
    }
    m_pressed.clear();

    //
    // Invoke all button released handlers
    for (auto&& [button, point] : m_released)
    {
        for (auto&& [id, handler] : m_handlersReleased)
        {
            handler(button, point, elapsedTime);
        }
    }
    m_released.clear();

    //
    // Invoke all the mouse moved handlers
    for (auto&& point : m_moved)
    {
        for (auto&& [id, handler] : m_handlersMoved)
        {
            handler(point, elapsedTime);
        }
    }
    m_moved.clear();
}
