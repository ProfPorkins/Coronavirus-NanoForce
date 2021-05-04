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

#include "components/Damage.hpp"
#include "components/Lifetime.hpp"
#include "components/Momentum.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "misc/math.hpp"

namespace entities
{

    Bullet::Bullet(std::uint16_t damage, std::chrono::microseconds lifetime, float size)
    {
        this->addComponent(std::make_unique<components::Position>(math::Point2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Size>(math::Dimension2f(size, size)));
        this->addComponent(std::make_unique<components::Momentum>(math::Vector2f(0.0f, 0.0f)));
        this->addComponent(std::make_unique<components::Lifetime>(lifetime));
        this->addComponent(std::make_unique<components::Damage>(damage));
        //
        // Very long story for why I put this back in here, writeup below...
        this->addComponent(std::make_unique<components::Orientation>(0.0f));
        //
        // This code is I used to detect the bug.  It was located in the Movement system, inside
        // the 'updateEntity' method.  After a bullet was created, it didn't have an Orientation
        // component.  But right after the 'update' of the gameloop finished, magically an Orientation
        // key in the Bullet entity would appear, but there was no value associated with it.  I tried
        // all kinds of things to figure out how it was happening, but never discovered the reason.  The
        // problem exists with both the MSVC and g++ compilers.  I'm sure it is a bug in my code somewhere,
        // but it is elusive.
        //
        // The tempoary (long-term?) solution is to go ahead and have an Orientation on a Bullet.
        //if (entity.hasComponent<components::Orientation>())
        //{
        //    // TODO: Bug I do NOT understand.  Bullets are magically having an Orientation component added to the
        //    //       keys in the Entity, but there is no value.  There is clearly some memory overwrite or similar
        //    //       type of bug, but I can't find it.  Unitl then, am testing the orientation component to make
        //    //       sure it isn't empty before using it.
        //    //       This same bug appears with both the MSVC and g++ compilers.
        //    auto orientation = entity.getComponent<components::Orientation>();
        //    if (orientation != nullptr)
        //    {
        //        orientation->set(orientation->get() + momentum->getRotateRate() * elapsedTime.count());
        //    }
        //}
    }

} // namespace entities
