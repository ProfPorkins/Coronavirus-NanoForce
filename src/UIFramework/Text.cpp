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

#include "Text.hpp"

#include "services/Configuration.hpp"

namespace ui
{
    Text::Text(float left, float top, const std::string text, std::shared_ptr<sf::Font> font, sf::Color color, unsigned int fontSize) :
        Element({ left, top, 0, 0 })
    {
        m_text.setPosition({ left, top });
        m_text.setFont(*font);
        m_text.setCharacterSize(fontSize);
        m_text.setFillColor(color);
        m_text.scale(Configuration::getGraphics().getScaleUI());
        //
        // Set the text string last, so the bounds of the element are set based on
        // the font, font size, etc settings.
        setText(text);
    }

    Text::Text(float left, float top, const std::string text, std::shared_ptr<sf::Font> font, sf::Color fillColor, sf::Color outlineColor, unsigned int fontSize) :
        Text(left, top, text, font, fillColor, fontSize)
    {
        m_text.setOutlineColor(outlineColor);
        m_text.setOutlineThickness(1.1f);
    }

    Text::Text(const std::string text, std::shared_ptr<sf::Font> font, sf::Color fillColor, unsigned int fontSize) :
        Text(0, 0, text, font, fillColor, fontSize)
    {
    }
    Text::Text(const std::string text, std::shared_ptr<sf::Font> font, sf::Color fillColor, sf::Color outlineColor, unsigned int fontSize) :
        Text(0, 0, text, font, fillColor, outlineColor, fontSize)
    {
    }

    void Text::render(sf::RenderTarget& renderTarget)
    {
        if (m_visible)
        {
            renderTarget.draw(m_text);
        }
    }

    void Text::setPosition(const math::Point2f& corner)
    {
        Element::setPosition(corner);
        m_text.setPosition({ corner.x, corner.y });
    }

    void Text::setText(const std::string text)
    {
        m_text.setString(text);
        setRegion({ m_region.left, m_region.top, m_text.getGlobalBounds().width, m_text.getGlobalBounds().height });
    }
} // namespace ui
