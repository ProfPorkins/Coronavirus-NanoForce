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

#include "ControllerInput.hpp"

namespace systems
{
    void ControllerInput::registerHandler(ControllerButton button, bool repeat, std::chrono::microseconds rate, std::function<void(std::chrono::microseconds)> handler)
    {
        m_handlersButton[button] = { repeat, rate, handler };
    }

    void ControllerInput::registerHandler(ControllerButton button, std::function<void(std::chrono::microseconds)> handler)
    {
        m_handlersButton[button] = { false, std::chrono::microseconds(0), handler };
    }

    void ControllerInput::registerButtonPressedHandler(ControllerButton button, std::function<void()> handler)
    {
        m_handlersButtonPressed[button] = handler;
    }

    void ControllerInput::registerButtonReleasedHandler(ControllerButton button, std::function<void()> handler)
    {
        m_handlersButtonReleased[button] = handler;
    }

    void ControllerInput::registerHandler(ControllerAxis axis, AxisHandler handler)
    {
        m_handlersAxis[axis] = handler;
    }

    void ControllerInput::unregisterHandler(ControllerButton button)
    {
        m_handlersButton.erase(button);
    }

    void ControllerInput::unregisterButtonPressedHandler(ControllerButton button)
    {
        m_handlersButtonPressed.erase(button);
    }

    void ControllerInput::unregisterButtonReleasedHandler(ControllerButton button)
    {
        m_handlersButtonReleased.erase(button);
    }

    void ControllerInput::unregisterHandler(ControllerAxis axis)
    {
        m_handlersAxis.erase(axis);
    }

    void ControllerInput::unregisterAll()
    {
        m_handlersButton.clear();
        m_handlersButtonPressed.clear();
        m_handlersButtonReleased.clear();

        m_handlersAxis.clear();
    }

    void ControllerInput::signalButtonPressed(sf::Event::JoystickButtonEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        m_buttonsPressed[static_cast<ControllerButton>(event.button)] = event;
        //
        // Because we can continuously receive the buttonpressed event, check to
        // see if we already have this property.  If we do, we don't want to
        // overwrite the value that already exists.
        if (m_buttonRepeat.find(static_cast<ControllerButton>(event.button)) == m_buttonRepeat.end())
        {
            m_buttonRepeat[static_cast<ControllerButton>(event.button)] = false;
        }
    }

    void ControllerInput::signalButtonReleased(sf::Event::JoystickButtonEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        m_buttonsPressed.erase(static_cast<ControllerButton>(event.button));
        m_buttonRepeat.erase(static_cast<ControllerButton>(event.button));

        m_buttonsReleased[static_cast<ControllerButton>(event.button)] = event;
    }

    void ControllerInput::signalJoystickMoved(sf::Event::JoystickMoveEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        m_axisEvents.push_back(event);
    }

    void ControllerInput::update(const std::chrono::microseconds elapsedTime)
    {
        //
        // Axis Handlers
        for (auto&& event : m_axisEvents)
        {
            if (auto itr = m_handlersAxis.find(static_cast<ControllerAxis>(event.axis)); itr != m_handlersAxis.end())
            {
                // Scale the position to [0, 1)
                itr->second(elapsedTime, event.position / 100.0f);
            }
        }
        m_axisEvents.clear();

        //
        // Button pressed hanlders
        for (auto&& [button, event] : m_buttonsPressed)
        {
            auto itr = m_handlersButton.find(button);
            if (itr != m_handlersButton.end())
            {
                itr->second.elapsedTime += elapsedTime;
                if (itr->second.repeat == true)
                {
                    //
                    // Check the rate vs elapsed time for this button before invoking the handler
                    if (itr->second.elapsedTime >= itr->second.rate)
                    {
                        itr->second.handler(elapsedTime);
                        m_buttonRepeat[button] = true;
                        //
                        // Reset the elapsed time, adding in any extra time beyond the repeat
                        // rate that may have accumulated.
                        itr->second.elapsedTime = (itr->second.elapsedTime - itr->second.rate);
                    }
                }
                else if (itr->second.repeat == false && m_buttonRepeat[button] == false)
                {
                    itr->second.handler(elapsedTime);
                    m_buttonRepeat[button] = true;
                }
            }
            //
            // button pressed handlers
            auto itrPressed = m_handlersButtonPressed.find(button);
            if (itrPressed != m_handlersButtonPressed.end())
            {
                if (!itrPressed->second.signaled)
                {
                    itrPressed->second.signaled = true;
                    itrPressed->second.handler();
                }
            }
        }

        //
        // button released handlers
        for (auto&& [key, event] : m_buttonsReleased)
        {
            auto itrReleased = m_handlersButtonReleased.find(key);
            if (itrReleased != m_handlersButtonReleased.end())
            {
                itrReleased->second();
            }

            //
            // If there is a matching "keypressed" handler, reset it's signaled state
            auto itrPressed = m_handlersButtonPressed.find(key);
            if (itrPressed != m_handlersButtonPressed.end())
            {
                itrPressed->second.signaled = false;
            }
        }
        m_buttonsReleased.clear();
    }
} // namespace systems