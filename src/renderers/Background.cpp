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

#include "Background.hpp"

#include "misc/math.hpp"
#include "services/Configuration.hpp"

namespace renderers
{
    Background::Background(std::shared_ptr<sf::Texture> texture, math::Dimension2f size, math::Point2f position, float orientation)
    {
        m_sprite = std::make_shared<sf::Sprite>();
        m_sprite->setTexture(*texture);
        // Point about which rotation takes place, the center of the texture
        m_sprite->setOrigin({ texture->getSize().x / 2.0f, texture->getSize().y / 2.0f });
        m_sprite->setScale(math::getViewScale(size, m_sprite->getTexture()));

        m_sprite->setPosition(position);
        m_sprite->setRotation(orientation);
    }

    void Background::render(sf::RenderTarget& renderTarget)
    {
        renderTarget.draw(*m_sprite);
    }

} // namespace renderers
