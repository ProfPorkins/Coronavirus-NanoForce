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

#include "MenuItem.hpp"

#include "services/Content.hpp"
#include "services/KeyboardInput.hpp"

namespace ui
{
    MenuItem::MenuItem(float left, float top, const std::string text, std::shared_ptr<sf::Font> font, sf::Color color, sf::Color activeColor, unsigned int fontSize, std::function<void()> handler) :
        Element({ left, top, 0, 0 }),
        m_inactiveLabel(text, font, color, sf::Color::Black, fontSize),
        m_activeLabel("~" + text + "~", font, activeColor, sf::Color::Black, fontSize),
        m_handler(handler)
    {
        m_displayLabel = &m_inactiveLabel;

        // Pick the largest text label for the width
        m_region.width = m_activeLabel.getRegion().width;
    }

    MenuItem::MenuItem(const std::string text, std::shared_ptr<sf::Font> font, sf::Color color, sf::Color activeColor, unsigned int fontSize, std::function<void()> handler) :
        MenuItem(0, 0, text, font, color, activeColor, fontSize, handler)
    {
    }

    void MenuItem::start()
    {
        m_handlerId = KeyboardInput::instance().registerKeyReleasedHandler([this](sf::Keyboard::Key key) {
            if (this->isActive() && key == sf::Keyboard::Enter) {
                this->select();
            } });
    }

    void MenuItem::stop()
    {
        KeyboardInput::instance().unregisterKeyReleasedHandler(m_handlerId);
    }

    void MenuItem::signalMouseReleased([[maybe_unused]] sf::Mouse::Button button, math::Point2f point, [[maybe_unused]] std::chrono::microseconds elapsedTime)
    {
        if (this->getRegion().contains(point))
        {
            this->select();
        }
    }

    void MenuItem::render(sf::RenderTarget& renderTarget)
    {
        if (this->isVisible())
        {
            m_displayLabel->render(renderTarget);
        }
    }

    void MenuItem::setRegion(const math::Rectangle2f& region)
    {
        Element::setRegion(region);
        //
        // Have to compute where to locate the inactive label within the center
        // of the active label.
        auto inactiveLeft = (region.left + region.width / 2.0f) - m_inactiveLabel.getRegion().width / 2.0f;
        m_inactiveLabel.setPosition({ inactiveLeft, region.top });
        m_activeLabel.setPosition({ region.left, region.top });
    }

    void MenuItem::setActive()
    {
        Activate::setActive();
        m_displayLabel = &m_activeLabel;
    }
    void MenuItem::setInactive()
    {
        Activate::setInactive();
        m_displayLabel = &m_inactiveLabel;
    }

} // namespace ui
