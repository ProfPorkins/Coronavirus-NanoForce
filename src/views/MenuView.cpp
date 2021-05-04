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

#include "MenuView.hpp"

namespace views
{
    bool MenuView::start()
    {
        if (m_initialized)
            return true;

        using namespace std::string_literals;

        //
        // Prepare the background image for rendering
        // Note: Only reason for testing if it is there is so that during
        //       development the time to load the image doesn't have to be taken.
        if (Content::has<sf::Texture>(content::KEY_IMAGE_MENU_BACKGROUND))
        {
            auto texture = Content::get<sf::Texture>(content::KEY_IMAGE_MENU_BACKGROUND);
            m_background.setTexture(*texture);
            m_background.setOrigin({ texture->getSize().x / 2.0f, texture->getSize().y / 2.0f });
            m_background.setPosition({ 0.0f, 0.0f });
            auto aspectRatio = static_cast<float>(Configuration::getGraphics().getResolution().width) / Configuration::getGraphics().getResolution().height;
            if (aspectRatio >= 1.0)
            {
                m_background.setScale({ Configuration::getGraphics().getViewCoordinates().width / texture->getSize().x,
                                        Configuration::getGraphics().getViewCoordinates().width / texture->getSize().y });
            }
            else
            {
                // Try to make it look decent for vertical resolutions
                m_background.setScale({ Configuration::getGraphics().getViewCoordinates().width / texture->getSize().x,
                                        Configuration::getGraphics().getViewCoordinates().height / texture->getSize().y });
            }
        }

        //
        // Offset the game title a little from the top of the view
        m_title.setPosition({ -(m_title.getRegion().width / 2.0f),
                              -(Configuration::getGraphics().getViewCoordinates().height / 2.0f) + m_title.getRegion().height * 0.5f });

        m_initialized = true;

        return true;
    }

    void MenuView::render(sf::RenderTarget& renderTarget, [[maybe_unused]] const std::chrono::microseconds elapsedTime)
    {
        renderTarget.clear(sf::Color::Black);
        renderTarget.draw(m_background);

        m_title.render(renderTarget);
    }
} // namespace views
