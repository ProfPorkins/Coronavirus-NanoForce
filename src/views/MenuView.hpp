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

#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"
#include "services/ContentKey.hpp"
#include "UIFramework/Text.hpp"
#include "View.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace views
{
    // --------------------------------------------------------------
    //
    // Provides a class from which all other "menu" type views should inherit.
    // It provides the background and game title all those views share.
    //
    // --------------------------------------------------------------
    class MenuView : public View
    {
      public:
        using View::View;
        virtual bool start() override;

        virtual void render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime) override;

      private:
        bool m_initialized{ false };
        sf::Sprite m_background;
        ui::Text m_title{ 0.0f, 0.0f, "Coronavirus - Nano Force", Content::get<sf::Font>(content::KEY_FONT_TITLE), sf::Color::Red, sf::Color::Black, Configuration::get<std::uint8_t>(config::FONT_TITLE_SIZE) };
    };
} // namespace views
