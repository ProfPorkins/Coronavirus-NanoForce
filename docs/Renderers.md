# Renderers

Renderers are a way to organize the game rendering code into cohesive units.  Each type of thing that needs to be drawn to the screen has an associated renderer.  The following diagram provides an overview of the renderers in this code.

![Renderers Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Renderers.png)

All renderers expose a `render` method, the purpose of this method is to draw the thing.  The information about what to draw might have been specified during the constructor, set by another method, and/or passed in as additional parameter(s) to the `render` method.  All `render` methods accept an `sf::RenderTarget` parameter.  The [SFML](https://www.sfml-dev.org/) is used to handle the platform specifics for rendering, and the `sf::RenderTarget` is the rendering surface used to draw to the screen.

The following code segment shows the `GameModel::render` method implementation...

    void GameModel::render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
    {
        renderTarget.clear(sf::Color::Black);
        m_rendererBackground->render(renderTarget);

        for (auto&& [id, bullet] : m_bullets)
        {
            m_rendererBullet->render(*bullet, renderTarget);
        }

        for (auto&& [id, bomb] : m_bombs)
        {
            m_rendererBomb->render(*bomb, renderTarget);
        }

        for (auto&& [id, powerup] : m_powerups)
        {
            m_rendererPowerup->render(*powerup, renderTarget);
        }

        for (auto&& [id, virus] : m_viruses)
        {
            m_rendererSarsCov2->render(*virus, renderTarget, elapsedTime);
        }

        m_renderPlayer(renderTarget);
        m_rendererParticleSystem->render(m_sysParticle, renderTarget);
        m_rendererHUD->render(m_remainingNanoBots + 1, m_timePlayed, m_virusesKilled, renderTarget);
        m_rendererStatus->render(renderTarget);
    }

You can see how each different renderer is used to render the different elements of the game.  One item of note in this method is the `m_renderPlayer(renderTarget);` line of code.  When a level starts, the player controlled ship isn't yet visible.  `m_renderPlayer` is an `std::function` that accepts an `sf::RenderTarget` parameter.  When the level starts, it is assigned to a lambda that does nothing.  Once the player controlled ship needs to be drawn, it is assigned to a lambda that calls the actual renderer used to draw the player ship.

Because each renderer has a very specific job to do, the code for each is relatively small.  A few are highlighted in the sections below.

## `Sprite`

The SFML has the concept of a sprite through the `sf::Sprite` class.  This code uses these sprites to draw almost everything. For example, the player controlled ship is drawn using this renderer, because it has no other details, simply the ship location, orientation, and texture.  The class declaration for the `Sprite` renderer is shown next...

    class Sprite
    {
      public:
        Sprite(std::shared_ptr<sf::Texture> texture);

        void render(entities::Entity& entity, sf::RenderTarget& renderTarget);

      private:
        std::shared_ptr<sf::Sprite> m_sprite;
    };

The `Sprite` constructor is passed which texture to use for the rendering.  As show in the next code segment, an `sf::Sprite` is created and associated with this texture.  The full class implementation for the `Sprite` renderer is shown next...

    Sprite::Sprite(std::shared_ptr<sf::Texture> texture)
    {
        m_sprite = std::make_shared<sf::Sprite>();
        m_sprite->setTexture(*texture);
        // Point about which drawing, rotation, etc takes place, the center of the texture
        m_sprite->setOrigin({ texture->getSize().x / 2.0f, texture->getSize().y / 2.0f });
    }

    void Sprite::render(entities::Entity& entity, sf::RenderTarget& renderTarget)
    {
        auto position = entity.getComponent<components::Position>();
        auto size = entity.getComponent<components::Size>();
        auto orientation = entity.getComponent<components::Orientation>();

        m_sprite->setPosition(position->get());
        // Not all entities (like the bullet) have/need an orientation.
        if (orientation != nullptr)
        {
            m_sprite->setRotation(orientation->get());
        }

        m_sprite->setScale(math::getViewScale(size->get(), m_sprite->getTexture()));

        renderTarget.draw(*m_sprite);
    }

The `render` method accepts which `Entity` to draw and the `sf::RenderTarget` to use for rendering.  Sprites are simple to render, only needing a position, size, and orientation.  These values are obtained from the entity's components, the `sf::Sprite` is updated with these values and then rendered.  It's that simple!

It is worth noting again, the `Sprite` renderer's constructor accepts the texture to use for drawing, rather than it being passed into the `render` method.  This is for a couple of reasons.  The first is that the `Entity` should not be keeping a piece of data that is used only for rendering, an `Entity` contains data only necessary for the game model.  Therefore, a specific `Sprite` renderer is created for each type of entity, such as the player, a bullet, and a bomb.  The other reason for setting the texture in the constructor is performance.  At construction time, the `sf::Sprite` is set up, rather than having to set it up each time `render` is called, or having the `Entity` set it up and keep track of it (something it shouldn't do).

## `ParticleSystem`

The `ParticleSystem` renderer code is also quite small; the full implementation shown below.  The code is as simple as, "loop through the particles and draw them".  It is simpler than the `Sprite` renderer because particles don't have an orientation.

    void ParticleSystem::render(systems::ParticleSystem& ps, sf::RenderTarget& renderTarget)
    {
        for (decltype(ps.m_particleCount) p = 0; p < ps.m_particleCount; p++)
        {
            auto& particle = ps.m_inUse[p];
            particle->sprite->setPosition(particle->center);
            particle->sprite->setRotation(particle->rotation);

            renderTarget.draw(*particle->sprite);
        }
    }
