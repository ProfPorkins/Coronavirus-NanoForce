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

#include "GameStatus.hpp"

#include "Configuration.hpp"
#include "ConfigurationPath.hpp"
#include "Content.hpp"
#include "ContentKey.hpp"

namespace renderers
{
    GameStatus::GameStatus()
    {
        m_text = std::make_shared<ui::Text>(
            0.0f, 0.0f, "",
            Content::get<sf::Font>(content::KEY_FONT_GAME_STATUS),
            sf::Color::Yellow, sf::Color::Black,
            Configuration::get<std::uint8_t>(config::FONT_GAME_STATUS_SIZE));
    }

    void GameStatus::setMessage(const std::string message)
    {
        m_text->setText(message);
        // Center it
        m_text->setPosition({ -m_text->getRegion().width / 2.0f, m_text->getRegion().top });
    }

    void GameStatus::render(sf::RenderTarget& renderTarget)
    {
        m_text->render(renderTarget);
    }
} // namespace renderers
