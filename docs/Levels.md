# Levels

A level is the environment in which the game play takes place.  This includes things like the background image, what music to play (if enabled), max number of viruses, various messages that can appear, finding a safe starting position for the player, along with a few other things.  When I started the game I had grander visions (doesn't everyone) for what the levels would be like.  Originally I planed to have levels much larger than a single screen in which a player would navigate around and clean out the viruses.  Because this is something I do in my spare time for _fun_, I quickly realized I wasn't going to have the time to meet my original vision, at least in the time-frame I set aside to work on this game.

While working on the documentation, I decided to take the time to reduce some of the unnecessary complexity I had in the code from my original _grand_ design.  The code is now more simplified than it was at the time of release.  The code I revised wasn't that much more complicated, it just had more separation of duties into several classes that isn't necessary with the current simple level design.

The following diagram provides an overview of levels in this code.

![Levels Diagram](https://github.com/ProfPorkins/Coronavirus-NanoForce/blob/trunk/docs/images/Levels.png)

The game presents two types of levels, Training and Patient.  With respect to the code, there is only one tiny difference.  Training levels have all viruses start with an age of 0, while Patient levels have virus start with a range of ages.  The age of a virus affects is size and maturity; at full maturity a virus can birth new bouncing baby viruses.

The `Level` class accepts a `key` parameter that is used to lookup the details of each level.  They `key` is part of a series of keys inside of the configuration file that determines the settings for the level.  The following is a segment of the client.developer.json configuration file that shows the parameters for the _Final Checkout_ level...

    "training-5": {
        "name": "Final Checkout",
        "content": {
            "image": {
                "background": "petri-5.png"
            },
            "music": {
                "background": "background-music-2.ogg"
            },
            "messages": {
                "ready": "prepare for final training",
                "failure": "more training needed",
                "success": "training successful"
            }
        },
        "settings": {
            "initial-virus-count": 4,
            "max-virus-count": 8,
            "nano-bot-count": 3,
            "min-powerup-time": 5000,
            "bomb-powerup-time": 30000,
            "rapid-fire-powerup-time": 30000,
            "spread-fire-powerup-time": 30000
        }
    },

It is easy to see the configuration specifies the background art, music, messages, and then the various game play parameters.  Times are in milliseconds.

A few things that could be improved:  The `PetriDish` constructor accepts a `training` parameter that specifies whether or not the level is a training level.  The could (should) be specified in the configuration.  The UI code would need to be revised to group the training levels together, followed by the patient levels; not a difficult task.  Another improvement to the code would be to have it read all of the levels from the configuration file and present them, rather than the number and types being hard-coded into the `GameModel`.  This would make the level configuration completely data driver.  It would not be a lot of work to make this change, another item to add to the future revisions list.

## Arena Boundary

The `Level` class isn't aware of the arena shape, it could be anything; although the game ended up only having circular arenas in the end.  To abstract the shape away from the `Level` class, it provides two pure virtual methods that a concrete, derived, class must implement...

    virtual bool collidesWithBorder(entities::Entity& entity) = 0;
    virtual void bounceOffBorder(entities::Entity& entity) = 0;

The `Movement` system these two methods to determine when any entity (player ship, virus, bomb, bullet) has hit the arena bound and, if so, how to handle it.  If `collidesWithBorder` returns true, then `bounceOffBorder` is called to update the position of the entity, according to the specifics of the level; again, only circles for this game, but could be more complex.

## Emitting Powerups

The `Level` class includes an `update` method.  This method is marked as virtual, but has an implementation at the `Level` class.  This method goes through each of the powerup types and updates their state.  The code for updating a powerup is shown below...

    template <typename T>
    void updatePowerup(const std::chrono::microseconds elapsedTime, const std::chrono::microseconds timeFrame, std::chrono::microseconds& timeRemaining, std::chrono::microseconds& nextCompute)
    {
        nextCompute -= elapsedTime;
        if (nextCompute <= std::chrono::microseconds(0))
        {
            // If the time frame is 0, that means the powerup should never appear
            if (timeFrame > std::chrono::microseconds{ 0 })
            {
                // We want the powerup to appear once per timeFrame.  Therefore
                // compute a uniform random number/time between 0 and timeFrame (ms) and that is when
                // it will appear.
                timeRemaining = std::chrono::duration_cast<std::chrono::microseconds>(m_distUniform(m_generator) * timeFrame);
                timeRemaining = std::max(m_timeMinPowerup, timeRemaining);
                // Don't forget to reset the next time a powerup decision should be computed
                nextCompute = timeFrame;
            }
        }

        timeRemaining -= elapsedTime;
        if (timeRemaining <= std::chrono::microseconds(0))
        {
            std::shared_ptr<entities::Powerup> powerup = std::make_shared<T>(this->computePowerupPosition());
            m_emitPowerup(powerup);
            // Setting to a huge number so we don't generate another one until the time
            // is set in computeNextBombPowerup.
            timeRemaining = std::chrono::microseconds::max();
        }
    }

Some notes about this code:

* It is written as a method template, even though the class isn't a template.  This made allows the different types of powerups to all use the same logic in a relatively straightforward way.  The template type, `T`, is used when the time has come to emit a powerup.  At that time, an instance of the power needs to be created and emitted into the game play.
* The constructor for the `Level` class accepts an `std::function` that is called when a powerup is emitted.  This function is called right after the powerup instance is created, which notifies the `GameModel` a new powerup was just created and to add it to its list of powerup entities it is tracking.
* Powerups are guaranteed to appear within a time-fame, not just a probability of them appearing, but a guarantee they appear.  Notice the code generates a random number that falls within the specified time-frame and then uses that as a countdown timer for when to emit the powerup.

## Safe Starting Location

In order to give the player a fighting chance at the start of a level, and each new ship, a "safe start" location is computed and used.  Again, because the original intention was to have levels of different shapes, the `Level` class defines a pure virtual method concrete, derived classes must implement...

    virtual math::Point2f computePowerupPosition() = 0;

The `PetriDish` level tries to start the player in the center of the area if at all possible.  The algorithm spends up to 6 seconds waiting to place the player at the center.  After those 6 seconds are up, if the center isn't considered safe, it starts looking for other locations at various distances and angles throughout the circle.  In fact, it looks at 80 other possible locations...two circles (with different radii, of course) and 40 points around each of these circles.  A safe starting location is defined as no virus within a distance of 5 units from the center of the area, or no virus within 4 units of a point around any of the other two circles used for the safe search.

The amount of time to look for the center to be a safe starting location, along with the safe distances, and number of circles/points to search are all hard-coded into the code.  Ideally, these could/should be pulled into the game configuration file, making the algorithm parameterized through configuration, rather than code.
