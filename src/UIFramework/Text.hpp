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

#include "Element.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <string>

namespace ui
{
    // --------------------------------------------------------------
    //
    // Represents any text that needs to be drawn. The text is drawn
    // on a single line, this isn't some fancy pants proportional
    // rendering jobbie.
    //
    // --------------------------------------------------------------
    class Text : public Element
    {
      public:
        Text(float left, float top, const std::string text, std::shared_ptr<sf::Font> font, sf::Color fillColor, unsigned int fontSize);
        Text(float left, float top, const std::string text, std::shared_ptr<sf::Font> font, sf::Color fillColor, sf::Color outlineColor, unsigned int fontSize);
        Text(const std::string text, std::shared_ptr<sf::Font> font, sf::Color fillColor, unsigned int fontSize);
        Text(const std::string text, std::shared_ptr<sf::Font> font, sf::Color fillColor, sf::Color outlineColor, unsigned int fontSize);

        virtual void render(sf::RenderTarget& renderTarget) override;
        virtual void setPosition(const math::Point2f& corner) override;

        inline void setFillColor(sf::Color color) { m_text.setFillColor(color); }
        inline void setOutlineColor(sf::Color color) { m_text.setOutlineColor(color); }
        void setText(const std::string text);

      private:
        sf::Text m_text;
    };
} // namespace ui