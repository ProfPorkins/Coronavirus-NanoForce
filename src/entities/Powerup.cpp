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

#include "Powerup.hpp"

#include "components//Audio.hpp"
#include "components/AnimatedSprite.hpp"
#include "components/Lifetime.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "misc/math.hpp"
#include "misc/misc.hpp"
#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"

namespace entities
{

    Powerup::Powerup(std::string key, math::Point2f position)
    {
        using namespace config;

        const config_path POWERUP_IMAGE = { DOM_ENTITY, key, DOM_POWERUP, DOM_IMAGE };
        const config_path POWERUP_AUDIO = { DOM_ENTITY, key, DOM_POWERUP, DOM_AUDIO };
        const config_path POWERUP_SIZE = { DOM_ENTITY, key, DOM_POWERUP, DOM_SIZE };
        const config_path POWERUP_LIFETIME = { DOM_ENTITY, key, DOM_POWERUP, DOM_LIFETIME };
        const config_path POWERUP_SPRITE_COUNT = { DOM_ENTITY, key, DOM_POWERUP, DOM_SPRITE_COUNT };
        const config_path POWERUP_SPRITE_TIME = { DOM_ENTITY, key, DOM_POWERUP, DOM_SPRITE_TIME };

        //
        // Powerups know how to read themselves from the configuration
        std::string imageKey = "image/powerup-" + key;
        auto texture = Content::get<sf::Texture>(imageKey);

        auto spriteCount = Configuration::get<std::uint8_t>(POWERUP_SPRITE_COUNT);
        auto lifetime = misc::msTous(Configuration::get<std::chrono::milliseconds>(POWERUP_LIFETIME));
        auto size = Configuration::get<float>(POWERUP_SIZE);
        auto spriteTime = misc::msTous(Configuration::get<std::chrono::milliseconds>(POWERUP_SPRITE_TIME));

        this->addComponent(std::make_unique<components::Position>(position));
        this->addComponent(std::make_unique<components::Size>(math::Dimension2f(size, size)));
        this->addComponent(std::make_unique<components::Lifetime>(lifetime));
        this->addComponent(std::make_unique<components::Audio>("audio/powerup-" + key));
        this->addComponent(std::make_unique<components::AnimatedSprite>(texture, spriteCount, spriteTime));
        // Have to adjust the width dimension by the number of sprites in the image in
        // order for the rendering size to come out correctly.
        auto sizeCmp = this->getComponent<components::Size>();
        auto sprite = this->getComponent<components::AnimatedSprite>();
        sprite->getSprite()->setScale(math::getViewScale({ sizeCmp->get().width * sprite->getSpriteCount(), sizeCmp->get().height }, sprite->getSprite()->getTexture()));
    }

} // namespace entities
