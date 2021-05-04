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

#include "KeyboardOption.hpp"

#include "ContentKey.hpp"
#include "SoundPlayer.hpp"

namespace ui
{
    KeyboardOption::KeyboardOption(float centerX, float top, const std::string label, const std::string key, std::shared_ptr<sf::Font> font, sf::Color color, sf::Color activeColor, unsigned int fontSize, std::function<void()> onStart, std::function<void(std::string)> onComplete) :
        Element({ 0, 0, 0, 0 }),
        m_inactiveLabel(label, font, color, sf::Color::Black, fontSize),
        m_activeLabel("~" + label + "~", font, activeColor, sf::Color::Black, fontSize),
        m_key(key, font, sf::Color::White, sf::Color::Black, fontSize),
        m_onStart(onStart),
        m_onComplete(onComplete)
    {
        m_inactiveLabel.setPosition({ centerX - m_inactiveLabel.getRegion().width / 2.0f, top });
        m_activeLabel.setPosition({ centerX - m_activeLabel.getRegion().width / 2.0f, top });

        m_displayLabel = &m_inactiveLabel;

        //
        // Center the key underneath the label
        m_key.setPosition({ centerX - m_key.getRegion().width / 2.0f,
                            top + m_inactiveLabel.getRegion().height * 1.5f });

        //
        // With both the label and key elements created, can now define the
        // whole control region.
        //
        //  1.5f I don't know why
        //  2.0f to account for both the label and the key
        setRegion({ centerX - m_inactiveLabel.getRegion().width / 2.0f,
                    top,
                    m_inactiveLabel.getRegion().width,
                    m_inactiveLabel.getRegion().height * 1.5f * 2.0f });
    }

    void KeyboardOption::signalKeyPressed(sf::Event::KeyEvent event, [[maybe_unused]] std::chrono::microseconds elapsedTime)
    {
        if (!m_expectingInput && event.code == sf::Keyboard::Enter)
        {
            m_onStart();
            m_expectingInput = true;
        }
        if (!m_expectingInput)
            return;

        if (event.code >= sf::Keyboard::A && event.code <= sf::Keyboard::Z)
        {
            // This is a simple conversion from the SFML code to an ASCI character.
            // This isn't internationalizable, but guess I'll cross that road when it comes
            char key = static_cast<char>('a' + event.code - sf::Keyboard::A);
            acceptInput(std::string(1, key));
        }

        switch (event.code)
        {
            case sf::Keyboard::Escape:
                m_onComplete("esc");
                m_expectingInput = false;
                break;
            case sf::Keyboard::Left:
                acceptInput("left");
                break;
            case sf::Keyboard::Right:
                acceptInput("right");
                break;
            case sf::Keyboard::Up:
                acceptInput("up");
                break;
            case sf::Keyboard::Down:
                acceptInput("down");
                break;
            case sf::Keyboard::Space:
                acceptInput("space");
                break;
            default: // Needed to eliminate unused enumeration compiler warning
                break;
        }
    }

    void KeyboardOption::signalMouseReleased([[maybe_unused]] sf::Mouse::Button button, [[maybe_unused]] math::Point2f point, [[maybe_unused]] std::chrono::microseconds elapsedTime)
    {
        m_onStart();
        m_expectingInput = true;
    }

    void KeyboardOption::render(sf::RenderTarget& renderTarget)
    {
        if (m_visible)
        {
            m_displayLabel->render(renderTarget);
            m_key.render(renderTarget);
        }
    }

    void KeyboardOption::setActive()
    {
        Activate::setActive();
        m_displayLabel = &m_activeLabel;
    }
    void KeyboardOption::setInactive()
    {
        Activate::setInactive();
        m_displayLabel = &m_inactiveLabel;
    }

    void KeyboardOption::acceptInput(const std::string key)
    {
        m_onComplete(key);
        setKey(key);
        SoundPlayer::play(content::KEY_MENU_ACCEPT);
        m_expectingInput = false;
    }

} // namespace ui
