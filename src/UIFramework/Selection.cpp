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

#include "Selection.hpp"

#include "services/Content.hpp"

#include <algorithm> // std::max

namespace ui
{
    Selection::Selection(float top, const std::string label, const std::vector<std::tuple<std::string, std::uint8_t>> options, std::size_t selectedOption, std::shared_ptr<sf::Font> font, sf::Color color, sf::Color activeColor, unsigned int fontSize, std::function<void(std::uint8_t)> handler) :
        Element({ 0, top, 0, 0 }),
        m_options(options),
        m_selectedOption(selectedOption),
        m_handler(handler),
        m_inactiveLabel(label, font, color, sf::Color::Black, fontSize),
        m_activeLabel("~" + label + "~", font, activeColor, sf::Color::Black, fontSize),
        m_displayOption("", font, sf::Color::White, sf::Color::Black, fontSize)
    {
        m_inactiveLabel.setPosition({ -m_inactiveLabel.getRegion().width / 2.0f, top });
        m_activeLabel.setPosition({ -m_activeLabel.getRegion().width / 2.0f, top });
        m_displayLabel = &m_inactiveLabel;

        updateSelectionText();
        setRegion({
            m_displayOption.getRegion().left,
            top,
            std::max(m_inactiveLabel.getRegion().width, m_displayOption.getRegion().width),
            (m_inactiveLabel.getRegion().height + m_displayOption.getRegion().height) * 1.5f // TODO: That dang 1.5f
        });
    }

    void Selection::onKeyPressed(sf::Keyboard::Key key)
    {
        switch (key)
        {
            case sf::Keyboard::Left:
                selectLeft();
                break;
            case sf::Keyboard::Right:
                selectRight();
                break;
            default: // Needed to eliminate unused enumeration compiler warning
                break;
        }
    }

    void Selection::signalMouseReleased([[maybe_unused]] sf::Mouse::Button button, math::Point2f point, [[maybe_unused]] std::chrono::microseconds elapsedTime)
    {
        //
        // If left half, move left, if right half, move right
        if (this->getRegion().contains(point))
        {
            auto middleX = this->getRegion().left + this->getRegion().width * 0.5f;
            if (point.x <= middleX)
            {
                selectLeft();
            }
            else
            {
                selectRight();
            }
        }
    }

    void Selection::render(sf::RenderTarget& renderTarget)
    {
        if (this->isVisible())
        {
            m_displayLabel->render(renderTarget);
            m_displayOption.render(renderTarget);
        }
    }

    void Selection::setActive()
    {
        Activate::setActive();
        m_displayLabel = &m_activeLabel;
    }
    void Selection::setInactive()
    {
        Activate::setInactive();
        m_displayLabel = &m_inactiveLabel;
    }

    void Selection::selectLeft()
    {
        m_selectedOption = (m_selectedOption == 0) ? m_options.size() - 1 : m_selectedOption - 1;
        updateSelectionText();
        m_handler(std::get<1>(m_options[m_selectedOption]));
    }

    void Selection::selectRight()
    {
        m_selectedOption = (m_selectedOption + 1) % m_options.size();
        updateSelectionText();
        m_handler(std::get<1>(m_options[m_selectedOption]));
    }

    void Selection::updateSelectionText()
    {
        m_displayOption.setText("< " + std::get<0>(m_options[m_selectedOption]) + " >");
        m_displayOption.setPosition({
            -m_displayOption.getRegion().width / 2.0f,
            m_region.top + m_inactiveLabel.getRegion().height * 1.5f,
        });
    }
} // namespace ui