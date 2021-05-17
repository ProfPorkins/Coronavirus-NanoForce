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

#include "Gameplay.hpp"

namespace views
{

    Gameplay::Gameplay()
    {
        //
        // Go ahead and tell the game model to load all common content
        GameModel::loadContent();
    }

    bool Gameplay::start()
    {
        m_nextState = ViewState::GamePlay;

        //
        // Go ahead and get the game model created and initialize it so it can start
        // loading content immediately.
        m_model = std::make_unique<GameModel>();
        m_model->initialize();

        return true;
    }

    void Gameplay::stop()
    {
        m_model->shutdown();
    }

    void Gameplay::signalKeyPressed(sf::Event::KeyEvent event, const std::chrono::microseconds elapsedTime, const std::chrono::system_clock::time_point now)
    {
        switch (event.code)
        {
            case sf::Keyboard::Escape:
                m_nextState = ViewState::LevelSelect;
                break;
            default:
                m_model->signalKeyPressed(event, elapsedTime, now);
                break;
        }
    }

    void Gameplay::signalKeyReleased(sf::Event::KeyEvent event, const std::chrono::microseconds elapsedTime, const std::chrono::system_clock::time_point now)
    {
        m_model->signalKeyReleased(event, elapsedTime, now);
    }

    ViewState Gameplay::update(const std::chrono::microseconds elapsedTime, [[maybe_unused]] const std::chrono::system_clock::time_point now)
    {
        m_model->update(elapsedTime);
        return m_nextState;
    }

    void Gameplay::render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
    {
        m_model->render(renderTarget, elapsedTime);
    }
} // namespace views
