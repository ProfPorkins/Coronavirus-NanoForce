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

#include "KeyboardInput.hpp"

bool KeyboardInput::initialize()
{
    initializeStringToKeyMapping();

    return true;
}

void KeyboardInput::registerHandler(std::string key, bool repeat, std::chrono::microseconds rate, std::function<void(std::chrono::microseconds)> handler)
{
    m_handlers[m_stringToKey[key]] = { repeat, rate, handler };
}

void KeyboardInput::registerHandler(std::string key, std::function<void(std::chrono::microseconds)> handler)
{
    m_handlers[m_stringToKey[key]] = { false, std::chrono::microseconds(0), handler };
}

std::uint32_t KeyboardInput::registerKeyPressedHandler(std::function<void(sf::Keyboard::Key)> handler)
{
    auto id = getNextId();
    m_handlersAnyPressed[id] = handler;

    return id;
}

void KeyboardInput::registerKeyPressedHandler(std::string key, std::function<void()> handler)
{
    m_handlersPressed[m_stringToKey[key]] = handler;
}

std::uint32_t KeyboardInput::registerKeyReleasedHandler(std::function<void(sf::Keyboard::Key)> handler)
{
    auto id = getNextId();
    m_handlersAnyReleased[id] = handler;

    return id;
}

void KeyboardInput::registerKeyReleasedHandler(std::string key, std::function<void()> handler)
{
    m_handlersReleased[m_stringToKey[key]] = handler;
}

void KeyboardInput::unregisterHandler(std::string key)
{
    m_handlers.erase(m_stringToKey[key]);
}

void KeyboardInput::unregisterKeyPressedHandler(std::uint32_t id)
{
    m_handlersAnyPressed.erase(id);
}

void KeyboardInput::unregisterKeyPressedHandler(std::string key)
{
    m_handlersPressed.erase(m_stringToKey[key]);
}

void KeyboardInput::unregisterKeyReleasedHandler(std::uint32_t id)
{
    m_handlersAnyReleased.erase(id);
}

void KeyboardInput::unregisterKeyReleasedHandler(std::string key)
{
    m_handlersReleased.erase(m_stringToKey[key]);
}

void KeyboardInput::unregisterAll()
{
    m_handlers.clear();
    m_handlersPressed.clear();
    m_handlersReleased.clear();
}

void KeyboardInput::signalKeyPressed(sf::Event::KeyEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
{
    m_keysPressed[event.code] = event;
    //
    // Because we can continuously receive the keypressed event, check to
    // see if we already have this property.  If we do, we don't want to
    // overwrite the value that already exists.
    if (m_keyRepeat.find(event.code) == m_keyRepeat.end())
    {
        m_keyRepeat[event.code] = false;
    }
}

void KeyboardInput::signalKeyReleased(sf::Event::KeyEvent event, [[maybe_unused]] const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
{
    m_keysPressed.erase(event.code);
    m_keyRepeat.erase(event.code);

    m_keysReleased[event.code] = event;
}

void KeyboardInput::update(const std::chrono::microseconds elapsedTime)
{
    for (auto&& [key, event] : m_keysPressed)
    {
        auto itr = m_handlers.find(key);
        if (itr != m_handlers.end())
        {
            itr->second.elapsedTime += elapsedTime;
            if (itr->second.repeat == true)
            {
                //
                // Check the rate vs elapsed time for this key before invoking the handler
                if (itr->second.elapsedTime >= itr->second.rate)
                {
                    itr->second.handler(elapsedTime);
                    m_keyRepeat[key] = true;
                    //
                    // Reset the elapsed time, adding in any extra time beyond the repeat
                    // rate that may have accumulated.
                    itr->second.elapsedTime = (itr->second.elapsedTime - itr->second.rate);
                }
            }
            else if (itr->second.repeat == false && m_keyRepeat[key] == false)
            {
                itr->second.handler(elapsedTime);
                m_keyRepeat[key] = true;
            }
        }

        //
        // "any key pressed" handlers
        for (auto&& handler : m_handlersAnyPressed)
        {
            handler.second.handler(key);
        }

        //
        // "keypressed" handlers
        auto itrPressed = m_handlersPressed.find(key);
        if (itrPressed != m_handlersPressed.end())
        {
            if (!itrPressed->second.signaled)
            {
                itrPressed->second.signaled = true;
                itrPressed->second.handler();
            }
        }
    }

    //
    // Key released handlers
    for (auto&& [key, event] : m_keysReleased)
    {
        //
        // "any key released" handlers
        for (auto&& handler : m_handlersAnyReleased)
        {
            handler.second(key);
        }

        auto itrReleased = m_handlersReleased.find(key);
        if (itrReleased != m_handlersReleased.end())
        {
            itrReleased->second();
        }

        //
        // If there is a matching "keypressed" handler, reset it's signaled state
        auto itrPressed = m_handlersPressed.find(key);
        if (itrPressed != m_handlersPressed.end())
        {
            itrPressed->second.signaled = false;
        }
    }
    m_keysReleased.clear();
}

void KeyboardInput::initializeStringToKeyMapping()
{
    m_stringToKey["up"] = sf::Keyboard::Up;
    m_stringToKey["down"] = sf::Keyboard::Down;
    m_stringToKey["left"] = sf::Keyboard::Left;
    m_stringToKey["right"] = sf::Keyboard::Right;
    m_stringToKey["space"] = sf::Keyboard::Space;
    m_stringToKey["enter"] = sf::Keyboard::Enter;
    m_stringToKey["escape"] = sf::Keyboard::Escape;

    m_stringToKey["a"] = sf::Keyboard::A;
    m_stringToKey["b"] = sf::Keyboard::B;
    m_stringToKey["c"] = sf::Keyboard::C;
    m_stringToKey["d"] = sf::Keyboard::D;
    m_stringToKey["e"] = sf::Keyboard::E;
    m_stringToKey["f"] = sf::Keyboard::F;
    m_stringToKey["g"] = sf::Keyboard::G;
    m_stringToKey["h"] = sf::Keyboard::H;
    m_stringToKey["i"] = sf::Keyboard::I;
    m_stringToKey["j"] = sf::Keyboard::J;
    m_stringToKey["k"] = sf::Keyboard::K;
    m_stringToKey["l"] = sf::Keyboard::L;
    m_stringToKey["m"] = sf::Keyboard::M;
    m_stringToKey["n"] = sf::Keyboard::N;
    m_stringToKey["o"] = sf::Keyboard::O;
    m_stringToKey["p"] = sf::Keyboard::P;
    m_stringToKey["q"] = sf::Keyboard::Q;
    m_stringToKey["r"] = sf::Keyboard::R;
    m_stringToKey["s"] = sf::Keyboard::S;
    m_stringToKey["t"] = sf::Keyboard::T;
    m_stringToKey["u"] = sf::Keyboard::U;
    m_stringToKey["v"] = sf::Keyboard::V;
    m_stringToKey["w"] = sf::Keyboard::W;
    m_stringToKey["x"] = sf::Keyboard::X;
    m_stringToKey["y"] = sf::Keyboard::Y;
    m_stringToKey["z"] = sf::Keyboard::Z;
}
