# Particle System

While the Particle System is one of the code's systems, it deserves its own writeup because it is a little more complex than the others, and has a different purpose.  What makes the particle system different from the other systems is that it doesn't act upon entities, and the particles in the game are not entities and are completely managed by the particle system itself, rather than being a part of the game model.

There are three parts to the particle system, particles, effects, and the particle system.  Particles are the individual visual elements displayed as a result of things like virus death, player death, or the player start animation.  An effect is used to emit particles over some period of time and in some pattern.  Multiple effects may be used to create an overall result.  For example, when a virus dies, multiple effects are used to create the various circular patterns seen.  Finally, the particle system is responsible for the overall management of the particle effects and the particles emitted from those effects.  It updates the position, size, transparency, and lifetime of the particles, along with updating any active effects.

## Particle

The following is the complete code for a `Particle`...

    struct Particle
    {
        float alphaStart{ 1.0f };
        float alphaEnd{ 1.0f };
        float alpha{ 1.0f };
        float sizeStart{ 0.0f };
        float sizeEnd{ 0.0f };
        float size{ 0.0f };
        math::Point2f center{ 0.0f, 0.0f };
        math::Vector2f direction{ 0.0f, 0.0f };
        float speed{ 0.0f };
        float rotationRate{ 0.0f };
        float rotation{ 0.0f };
        std::chrono::microseconds lifetime{ 0 };
        std::chrono::microseconds alive{ 0 };
        sf::Sprite sprite;
    };

A particle instance is simply a conveyor of data to the `ParticleSystem`.  It has details about its visual appearance, speed, direction, along with how long it has been alive and should be alive.

## Effect

A `ParticleEffect` generates particles over some period of time and n some pattern.  Similar to a particle, an effect has a total lifetime and how long it has been alive.  Any number of different particle effects can be active in the particle system, with each generating particles.  

Effects are added by deriving from `ParticleEffect` and overriding the `update` method with logic unique to the desired effect.  The `update` method for an effect needs some discussion.

    virtual void update(const std::chrono::microseconds elapsedTime, std::function<std::unique_ptr<Particle>()> getParticle = nullptr, std::function<void(std::unique_ptr<Particle>)> addParticle = nullptr);

The implementation of this method in the `ParticleEffect` class simply updates how long the effect has been alive, there is no logic for generating particles.

Not the two functions (as parameters to the method) passed into this method, `getParticle` and `addParticle`.  When the `ParticleSystem` calls the `update` method, it provides these two parameters.  The `getParticle` function is (must be) used to get a pointer to a `Particle` instance.  The reason for this is that the `ParticleSystem` manages the individual particles and we want it to be responsible for the creation of `Particle` instances, rather than the effects.  The `addParticle` function is used by the effect to _emit_ a particle into the particle system.  This must be the same particle instance obtained from the `getParticle` function.

The following is the implementation of the `update` method for the `CircleExpansionEffect`...

    void CircleExpansionEffect::update(const std::chrono::microseconds elapsedTime, std::function<std::unique_ptr<Particle>()> getParticle, std::function<void(std::unique_ptr<Particle>)> addParticle)
    {
        ParticleEffect::update(elapsedTime);

        //
        // Generate a bunch of uniformly distributed particles around the center
        float angleDiff = 2 * 3.14159f / m_howMany;
        float angle = 0.0f;
        for (decltype(m_howMany) i = 0; i < m_howMany; i++, angle += angleDiff)
        {
            if (auto p = getParticle(); p != nullptr)
            {
                p->rotation = m_orientation.has_value() ? m_orientation.value() : 0.0f;
                p->lifetime = m_lifetime;
                p->alive = std::chrono::microseconds(0);
                p->size = m_sizeStart;
                p->sizeStart = m_sizeStart;
                p->sizeEnd = m_sizeEnd;
                p->direction.x = std::cos(angle);
                p->direction.y = std::sin(angle);
                // Adjust the center by the distance the particle should start from the effect center
                p->center = { m_center.x + p->direction.x * m_atDistance, m_center.y + p->direction.y * m_atDistance };
                p->speed = m_speed;
                p->sprite.setTexture(*m_texture);
                p->sprite.setOrigin(p->sprite.getTexture()->getSize().x / 2.0f, p->sprite.getTexture()->getSize().y / 2.0f);
                p->sprite.setScale(math::getViewScale(p->size, p->sprite.getTexture()));

                addParticle(std::move(p));
            }
        }
    }

From this, you can see that all derived effects must call the base class `ParticleEffect::update` method to ensure the length of time the effect has been around is updated.  After that, any logic the effect wants to implement to generate new particles executes.  In this case, this effect generates particles on the edge of a circle, defined by constructor parameters.  When used, this effect has a lifetime of 0, meaning it only generates a circle of particles one time, and then the effect is removed.  The constructor parameter does allow for a longer lifetime, but the game code never creates one with anything other than a lifetime of 0.

## System

The `ParticleSystem` manages all details related to effects and individual particles.  New effects are added from game code using the `addEffect` method, and then forgets about it.  Once added, the particle system updates the effect, manages it's lifetime, along with any particles generated by the effect.

While it is intended there should only be one `ParticleSystem` instance active, it is not written as a singleton or some kind of service.  The reason for this is that it isn't a resource that needs to be globally accessible (in this game).  Particles are only a part of the game play, not part of the menuing system.  Therefore, the `ParticleSystem` is created by the `GameModel` and told to update and render by the `GameModel`.  More than one instance could be created, it would cause a problem necessarily, but it would add unnecessary complexity to the game code.

Like other systems, the `ParticleSystem` exposes an `update` method.  The implementation for this method is two steps: Step 1, update active particles, Step 2, update active effects.  During particle update, they are moved, lifetimes updated, and those whose lifetime has been exceeded are removed.  Similarly for updating effects, their lifetime is updated, the effect `update` method is called to generate new particles, and any effects whose lifetime has been exceeded are removed.

### Particle Memory Optimization

While probably not necessary for this game, I still wanted to optimize the memory allocation pattern for the particle instances.  A naive approach to memory allocation would be to completely rely upon the smart pointers in C++ for allocation/deallocation.  This probably would have been fine for this game, because there are only a few dozen, maybe a few hundred particles ever active at any one time.  What I chose to do instead was preallocate a pool of `Particle` instances and then reuse those instances over and over.  In this way, particles are allocated one time when the particle system is created, after that particle instances are never allocated or deallocated during game play.

Preallocated particles are placed into a First-In, First-Out (FIFO) queue, using the following code...

    void ParticleSystem::preAllocateParticles()
    {
        while (m_available.size() < MAX_PARTICLES)
        {
            m_available.push_back(std::make_unique<Particle>());
        }
    }

When a particle instance is needed, the `getAvailableParticle` method is called.  This method is passed into an effect's `update` method as described in the section on effects above (it is actually passed in as part of a lambda, but I digress).  When called, this method returns the next available particle instance.  The code for this method is shown below...

    std::unique_ptr<Particle> getAvailableParticle()
    {
        if (!m_available.empty())
        {
            // Yuck!
            auto p = std::move(m_available.front());
            m_available.pop_front();
            return p;
        }
        return nullptr; // Could return an optional, but I'm fine with nullptr for this.
    }

The "Yuck!" comment is that I wish the `pop_front()` method returned the instance at the front, allowing the code to all be done on one line; life is so difficult.
