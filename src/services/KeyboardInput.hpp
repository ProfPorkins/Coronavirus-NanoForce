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

class KeyboardInput
{
  public:
    KeyboardInput()
    {
        initializeStringToKeyMapping();
    }
    void registerHandler(std::string key, bool repeat, std::chrono::microseconds rate, std::function<void(std::chrono::microseconds)> handler);
    void registerHandler(std::string key, std::function<void(std::chrono::microseconds)> handler);
    void registerKeyPressedHandler(std::string key, std::function<void()> handler);
    void registerKeyReleasedHandler(std::string key, std::function<void()> handler);
    void unregisterHandler(std::string key);
    void unregisterKeyPressedHandler(std::string key);
    void unregisterKeyReleasedHandler(std::string key);
    void unregisterAll();

    void signalKeyPressed(sf::Event::KeyEvent event, const std::chrono::microseconds elapsedTime, const std::chrono::system_clock::time_point now);
    void signalKeyReleased(sf::Event::KeyEvent event, const std::chrono::microseconds elapsedTime, const std::chrono::system_clock::time_point now);
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

    struct KeyPressedInfo
    {
        KeyPressedInfo() = default;

        KeyPressedInfo(std::function<void()> handler) :
            handler(handler)
        {
        }

        bool signaled{ false };
        std::function<void()> handler{ nullptr };
    };

    std::unordered_map<std::string, sf::Keyboard::Key> m_stringToKey;
    std::unordered_map<sf::Keyboard::Key, sf::Event::KeyEvent> m_keysPressed;
    std::unordered_map<sf::Keyboard::Key, sf::Event::KeyEvent> m_keysReleased;
    std::unordered_map<sf::Keyboard::Key, bool> m_keyRepeat;
    std::unordered_map<sf::Keyboard::Key, InputInfo> m_handlers;
    std::unordered_map<sf::Keyboard::Key, KeyPressedInfo> m_handlersPressed;
    std::unordered_map<sf::Keyboard::Key, std::function<void()>> m_handlersReleased;

    void initializeStringToKeyMapping();
};
