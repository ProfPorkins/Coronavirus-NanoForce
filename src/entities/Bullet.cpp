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

#include "Bullet.hpp"

#include "components/Collidable.hpp"
#include "components/Damage.hpp"
#include "components/Lifetime.hpp"
#include "components/Momentum.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "components/Sprite.hpp"
#include "misc/math.hpp"
#include "services/Content.hpp"
#include "services/ContentKey.hpp"

namespace entities
{

    Bullet::Bullet(std::uint16_t damage, std::chrono::microseconds lifetime, float size)
    {
        this->addComponent(std::make_unique<components::Position>(math::Point2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Size>(math::Dimension2f(size, size)));
        this->addComponent(std::make_unique<components::Momentum>(math::Vector2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Lifetime>(lifetime));
        this->addComponent(std::make_unique<components::Damage>(damage));
        this->addComponent(std::make_unique<components::Sprite>(Content::get<sf::Texture>(content::KEY_IMAGE_BASIC_GUN_BULLET)));
        this->addComponent(std::make_unique<components::Orientation>(0.0f));
        this->addComponent(std::make_unique<components::Collidable>(components::Collidable::Type::Bullet));
    }

} // namespace entities
