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
#include <vector>

namespace systems
{
    enum class ControllerAxis // The type is sf::Joystick::Axis, which is an enumeration, and can't be used as a type here
    {
        LeftJoystick1 = 0,  // Values: -100 to 100
        LeftJoystick2 = 1,  // Values: -100 to 100
        LeftTrigger = 2,    // Values: 0 to 100
        RightTrigger = 2,   // Values - to -100
        RightJoystick1 = 4, // Values: -100 to 100
        RightJoystick2 = 5, // Values: -100 to 100
        LeftDPad = 6,       // Values: 0, -100
        RightDPad = 6,      // Values: 0, 100
        UpDPad = 6,         // Values: 0, 100
        DownDPad = 6        // Values: 0, -100
    };

    enum class ControllerButton : decltype(sf::Event::JoystickButtonEvent::button)
    {
        A = 0,
        B = 1,
        X = 2,
        Y = 3,
        LeftBumper = 4,
        RightBumper = 5,
        LeftJoystick = 7,
        RightJoystick = 9
    };

    class ControllerInput
    {
      public:
        using AxisHandler = std::function<void(std::chrono::microseconds, float)>;

        void registerHandler(ControllerButton button, bool repeat, std::chrono::microseconds rate, std::function<void(std::chrono::microseconds)> handler);
        void registerHandler(ControllerButton button, std::function<void(std::chrono::microseconds)> handler);
        void registerButtonPressedHandler(ControllerButton button, std::function<void()> handler);
        void registerButtonReleasedHandler(ControllerButton button, std::function<void()> handler);
        void registerHandler(ControllerAxis axis, AxisHandler handler);

        void unregisterHandler(ControllerButton button);
        void unregisterButtonPressedHandler(ControllerButton button);
        void unregisterButtonReleasedHandler(ControllerButton button);
        void unregisterHandler(ControllerAxis axis);

        void unregisterAll();

        void signalButtonPressed(sf::Event::JoystickButtonEvent event, const std::chrono::microseconds elapsedTime, const std::chrono::system_clock::time_point now);
        void signalButtonReleased(sf::Event::JoystickButtonEvent event, const std::chrono::microseconds elapsedTime, const std::chrono::system_clock::time_point now);
        void signalJoystickMoved(sf::Event::JoystickMoveEvent event, const std::chrono::microseconds elapsedTime, const std::chrono::system_clock::time_point now);

        void update(const std::chrono::microseconds elapsedTime);

      private:
        struct InputInfo
        {
            InputInfo() = default;

            InputInfo(bool repeat, std::chrono::microseconds rate, std::function<void(std::chrono::microseconds)> handler) :
                repeat(repeat),
                rate(rate),
                elapsedTime(rate), // Set the elapsedTime to the 'rate' so that it is ready to fire immediately
                handler(handler)
            {
            }

            bool repeat{ false };
            std::chrono::microseconds rate{ 0 };
            std::chrono::microseconds elapsedTime{ 0 };
            std::function<void(std::chrono::microseconds)> handler{ nullptr };
        };

        struct ButtonPressedInfo
        {
            ButtonPressedInfo() = default;

            ButtonPressedInfo(std::function<void()> handler) :
                handler(handler)
            {
            }

            bool signaled{ false };
            std::function<void()> handler{ nullptr };
        };

        std::unordered_map<ControllerButton, sf::Event::JoystickButtonEvent> m_buttonsPressed;
        std::unordered_map<ControllerButton, sf::Event::JoystickButtonEvent> m_buttonsReleased;
        std::vector<sf::Event::JoystickMoveEvent> m_axisEvents;

        std::unordered_map<ControllerButton, bool> m_buttonRepeat;
        std::unordered_map<ControllerButton, InputInfo> m_handlersButton;
        std::unordered_map<ControllerButton, ButtonPressedInfo> m_handlersButtonPressed;
        std::unordered_map<ControllerButton, std::function<void()>> m_handlersButtonReleased;

        std::unordered_map<ControllerAxis, AxisHandler> m_handlersAxis;
    };
} // namespace systems
