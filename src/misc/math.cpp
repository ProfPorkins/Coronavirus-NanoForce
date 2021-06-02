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

#include "math.hpp"

#include "components/Position.hpp"
#include "components/Size.hpp"
#include "services/Configuration.hpp"

#include <chrono>
#include <cmath>

namespace math
{

    Vector2f vectorFromRadians(float radians)
    {
        return math::Vector2f(std::cos(radians), std::sin(radians));
    }

    Vector2f vectorFromDegrees(float degrees)
    {
        return math::Vector2f(std::cos(toRadians(degrees)), std::sin(toRadians(degrees)));
    }

    // --------------------------------------------------------------
    //
    // Helper function used to reflect a vector about a normal.
    //
    // --------------------------------------------------------------
    Vector2f reflect(Vector2f n, Vector2f l)
    {
        // Create a unit normal (we don't assume it comes in that way)
        auto magnitude = std::sqrt(n.x * n.x + n.y * n.y);
        n.x /= magnitude;
        n.y /= magnitude;
        // This is the reflection computation
        auto ndotl = l.x * n.x + l.y * n.y;
        return { 2 * ndotl * n.x - l.x, 2 * ndotl * n.y - l.y };
    }

    // --------------------------------------------------------------
    //
    // Helper function used to determine if two entities have collided
    // with each other.  Collision is determined if circles from
    // their inner radius have any overlap.
    //
    // --------------------------------------------------------------
    bool collides(entities::Entity& a, entities::Entity& b)
    {
        auto aPosition = a.getComponent<components::Position>();
        auto aRadius = a.getComponent<components::Size>()->getInnerRadius();

        auto bPosition = b.getComponent<components::Position>();
        auto bRadius = b.getComponent<components::Size>()->getInnerRadius();

        auto abX = bPosition->get().x - aPosition->get().x;
        auto abY = bPosition->get().y - aPosition->get().y;
        auto distance = std::sqrt(abX * abX + abY * abY);

        return (distance <= (aRadius + bRadius));
    }

    // --------------------------------------------------------------
    //
    // This function is used to compute a scaling factor for a game object
    // based on how many units in 'size' it should be based on the view
    // coordinates settings, but has a texture of potentially any number of pixels.
    //
    // --------------------------------------------------------------
    sf::Vector2f getViewScale(const Dimension2f size, const sf::Texture* texture)
    {
        auto coords = Configuration::getGraphics().getViewCoordinates();
        return { (size.width / coords.width) * (coords.width / texture->getSize().x),
                 (size.height / coords.height) * (coords.height / texture->getSize().y) };
    }

    sf::Vector2f getViewScale(const float size, const sf::Texture* texture)
    {
        return getViewScale({ size, size }, texture);
    }

    float lerp(std::chrono::microseconds x, std::chrono::microseconds x0, std::chrono::microseconds x1, float y0, float y1)
    {
        return y0 + (x.count() - x0.count()) * ((y1 - y0) / (x1.count() - x0.count()));
    }

} // namespace math