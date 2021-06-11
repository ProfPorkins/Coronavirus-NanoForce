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

#include "GameModel.hpp"

#include "components/Bullets.hpp"
#include "components/Damage.hpp"
#include "components/Health.hpp"
#include "components/Lifetime.hpp"
#include "components/Momentum.hpp"
#include "components/Orientation.hpp"
#include "components/Position.hpp"
#include "components/Size.hpp"
#include "entities/Bomb.hpp"
#include "entities/Bullet.hpp"
#include "entities/PowerupBomb.hpp"
#include "entities/PowerupRapidFire.hpp"
#include "entities/PowerupSpreadFire.hpp"
#include "levels/PetriDish.hpp"
#include "misc/math.hpp"
#include "misc/misc.hpp"
#include "services/Configuration.hpp"
#include "services/ConfigurationPath.hpp"
#include "services/Content.hpp"
#include "services/ContentKey.hpp"
#include "services/KeyboardInput.hpp"
#include "services/SoundPlayer.hpp"
#include "systems/effects/CircleExpansionEffect.hpp"
#include "systems/effects/PlayerStartEffect.hpp"

#include <chrono>
#include <cmath>
#include <optional>
#include <unordered_set>

// Static member implementation
levels::LevelName GameModel::m_levelSelect{ levels::LevelName::Training1 };
std::atomic_bool GameModel::m_contentError{ false };

void GameModel::selectLevel(levels::LevelName whichLevel)
{
    m_levelSelect = whichLevel;
}

GameModel::GameModel()
{
    switch (m_levelSelect)
    {
        case levels::LevelName::Training1:
            m_level = std::make_unique<levels::PetriDish>(config::TRAINING_1, true);
            break;
        case levels::LevelName::Training2:
            m_level = std::make_unique<levels::PetriDish>(config::TRAINING_2, true);
            break;
        case levels::LevelName::Training3:
            m_level = std::make_unique<levels::PetriDish>(config::TRAINING_3, true);
            break;
        case levels::LevelName::Training4:
            m_level = std::make_unique<levels::PetriDish>(config::TRAINING_4, true);
            break;
        case levels::LevelName::Training5:
            m_level = std::make_unique<levels::PetriDish>(config::TRAINING_5, true);
            break;
        case levels::LevelName::Patient1:
            m_level = std::make_unique<levels::PetriDish>(config::PATIENT_1, false);
            break;
        case levels::LevelName::Patient2:
            m_level = std::make_unique<levels::PetriDish>(config::PATIENT_2, false);
            break;
        case levels::LevelName::Patient3:
            m_level = std::make_unique<levels::PetriDish>(config::PATIENT_3, false);
            break;
    }

    m_level->loadContent();
}

// --------------------------------------------------------------
//
// This is where all game model initialization occurs.
//
// --------------------------------------------------------------
void GameModel::initialize()
{
    // Busy wait for the content to finish loading.  It's okay, it happens super fast on first time level is started
    while (!this->contentReady())
        ;

    m_rendererBackground = std::make_unique<renderers::Background>(
        Content::get<sf::Texture>(m_level->getBackgroundImageKey()),
        m_level->getBackgroundSize(),
        math::Point2f(0.0f, 0.0f));
    m_rendererHUD = std::make_unique<renderers::HUD>();
    m_rendererStatus = std::make_unique<renderers::GameStatus>();

    m_viruses.clear();

    m_sysMovement = std::make_unique<systems::Movement>(*m_level);
    m_sysAge = std::make_unique<systems::Age>();
    m_sysAnimatedSprite = std::make_unique<systems::AnimatedSprite>();
    m_sysBirth = std::make_unique<systems::Birth>([this](entities::Entity::IdType parentId) { this->onVirusBirth(parentId); });
    m_sysHealth = std::make_unique<systems::Health>();
    m_sysLifetime = std::make_unique<systems::Lifetime>([this](entities::Entity::IdType entityId) { this->removeEntity(entityId); });
    m_sysPowerup = std::make_unique<systems::Powerup>(
        *m_level,
        [this](std::shared_ptr<entities::Powerup>& powerup) { this->emitPowerup(powerup); },
        m_level->getKey());
    m_sysCollision = std::make_unique<systems::Collision>(
        [this](entities::Entity::IdType entityId) { this->removeEntity(entityId); },
        [this](entities::Entity::IdType entityId) { this->onVirusDeath(entityId); },
        [this]() { this->onPlayerDeath(); });
    m_sysParticle = std::make_unique<systems::ParticleSystem>();

    m_sysRendererSprite = std::make_unique<systems::RendererSprite>();
    m_sysRendererAnimatedSprite = std::make_unique<systems::RendererAnimatedSprite>();
    m_sysRendererSarsCov2 = std::make_unique<systems::RendererVirus>();
    m_sysRendererParticleSystem = std::make_unique<systems::RendererParticleSystem>();

    for (auto&& virus : m_level->initializeViruses())
    {
        m_viruses[virus->getId()] = virus;
        addEntity(virus);
    }

    // -1 because the current bot being played is 1 of the remaining bots
    m_remainingNanoBots = m_level->getNanoBotCount() - 1;
    resetPlayer();

    //
    // Start playing the background music
    if (Configuration::get<bool>(config::PLAY_BACKGROUND_MUSIC) && Content::has<sf::Music>(m_level->getBackgroundMusicKey()))
    {
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->stop();
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->setVolume(15);
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->setLoop(true);
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->play();
    }
}

// --------------------------------------------------------------
//
// Shutdown anything that doesn't automatically shutdown on its own,
// for example, background music.
//
// --------------------------------------------------------------
void GameModel::shutdown()
{
    //
    // Shutdown the background music
    if (Configuration::get<bool>(config::PLAY_BACKGROUND_MUSIC) && Content::has<sf::Music>(m_level->getBackgroundMusicKey()))
    {
        Content::get<sf::Music>(m_level->getBackgroundMusicKey())->stop();
    }

    KeyboardInput::instance().unregisterAll();
}

// --------------------------------------------------------------
//
// This is where everything performs its update.
//
// --------------------------------------------------------------
void GameModel::update(const std::chrono::microseconds elapsedTime)
{
    m_updatePlayer(elapsedTime);

    m_sysParticle->update(elapsedTime);
    m_sysLifetime->update(elapsedTime);
    m_sysMovement->update(elapsedTime);

    // It isn't absolutely essential to the overall game, but the age should be updated
    // before Birth because age is used in the gestation determination in the Birth system.
    m_sysAge->update(elapsedTime);
    m_sysBirth->update(elapsedTime);
    m_sysHealth->update(elapsedTime);
    m_sysPowerup->update(elapsedTime);
    m_sysAnimatedSprite->update(elapsedTime);

    //
    // Add any new viruses in at this time
    for (auto&& virus : m_newViruses)
    {
        m_viruses[virus->getId()] = virus;
        addEntity(virus);
    }
    m_newViruses.clear();

    // NOTE: Important to keep this after adding new viruses and at the end of the update loop.
    //       The reason is that new viruses can be born during the update.  The onVirusDeath method
    //       checks to see if there are no more visuses so it can set the game over message.  If not
    //       placed here, there is a chance for that message to be displayed, only to have a new virus
    //       appear because it was born after that message was set
    m_sysCollision->update(elapsedTime);
}

// --------------------------------------------------------------
//
// All rendering takes place here.
//
// --------------------------------------------------------------
void GameModel::render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime)
{
    renderTarget.clear(sf::Color::Black);
    m_rendererBackground->render(renderTarget);
    m_rendererHUD->render(m_remainingNanoBots + 1, m_timePlayed, m_virusesKilled, renderTarget);
    m_rendererStatus->render(renderTarget);

    m_sysRendererAnimatedSprite->update(elapsedTime, renderTarget);
    m_sysRendererSarsCov2->update(elapsedTime, renderTarget);
    m_sysRendererSprite->update(elapsedTime, renderTarget);
    m_sysRendererParticleSystem->update(*m_sysParticle, renderTarget);
}

// --------------------------------------------------------------
//
// This is used to allow other code to emit bullets into the game model.
//
// --------------------------------------------------------------
void GameModel::emitBullet(std::shared_ptr<entities::Entity>& bullet)
{
    addEntity(bullet);
}

// --------------------------------------------------------------
//
// This is used to allow other code to emit a powerup into the game model.
//
// --------------------------------------------------------------
void GameModel::emitPowerup(std::shared_ptr<entities::Powerup>& powerup)
{
    addEntity(powerup);
}

// --------------------------------------------------------------
//
// A virus was killed, start a sound and a death animation.
//
// --------------------------------------------------------------
void GameModel::onVirusDeath(entities::Entity::IdType entityId)
{
    SoundPlayer::play(content::KEY_AUDIO_VIRUS_DEATH);

    auto virus = m_viruses[entityId];
    auto position = virus->getComponent<components::Position>();
    auto size = virus->getComponent<components::Size>();

    std::uint16_t howMany = virus->getComponent<components::Bullets>()->howMany() * 5;
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_SARSCOV2_PARTICLE, position->get(), 0.0f, howMany, 0.00002f, 1.0f, 0.2f, misc::msTous(std::chrono::milliseconds(1000))));
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_SARSCOV2_PARTICLE, position->get(), size->getInnerRadius() / 2.0f, howMany, 0.0000075f, 1.0f, 0.2f, misc::msTous(std::chrono::milliseconds(1000))));
    //
    // Time these so they finish just before the center
    auto lifetime = misc::msTous(std::chrono::milliseconds(1500));
    auto speed = -size->getInnerRadius() / lifetime.count();
    // All bullets are the same size, and this is just a reference point for creating the particle effect anyway.
    config::config_path WEAPON_ITEM_SIZE = { config::DOM_ENTITY, config::ENTITY_WEAPON_BASIC_GUN, config::DOM_SIZE };
    auto bulletSize = Configuration::get<float>(WEAPON_ITEM_SIZE);
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_BASIC_GUN_BULLET, position->get(), size->getInnerRadius(), virus->getComponent<components::Bullets>()->howMany(), speed, bulletSize, bulletSize / 2.0f, lifetime));

    //
    // One particle for the virus itself slowly going away
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_SARSCOV2, position->get(), 0.0f, static_cast<std::uint16_t>(1), 0.0f, size->getOuterRadius(), 0.01f, lifetime));

    m_virusesKilled++;
    //
    // If this is the last virus, happy, happy!
    m_viruses.erase(entityId);
    if (m_viruses.size() == 0)
    {
        m_rendererStatus->setMessage(m_level->getMessageSuccess());
    }
}

// --------------------------------------------------------------
//
// A new virus was just birthed, need to create the instance here.
//
// --------------------------------------------------------------
void GameModel::onVirusBirth(entities::Entity::IdType parentId)
{
    if (m_viruses.size() < m_level->getMaxViruses())
    {
        auto parentPosition = m_viruses[parentId]->getComponent<components::Position>();
        auto virus = std::make_shared<entities::Virus>();
        virus->getComponent<components::Position>()->set(parentPosition->get());
        m_newViruses.push_back(virus);
    }
}

// --------------------------------------------------------------
//
// When the player dies, show a cool explosion and reset the player
// for a restart.
//
// --------------------------------------------------------------
void GameModel::onPlayerDeath()
{
    SoundPlayer::play(content::KEY_AUDIO_PLAYER_DEATH);

    auto position = m_player->getComponent<components::Position>();
    auto howMany = static_cast<std::uint16_t>(100);
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_PLAYER_PARTICLE, position->get(), 0.0f, howMany, 0.00002f, 1.0f, 0.05f, misc::msTous(std::chrono::milliseconds(500))));
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_PLAYER_PARTICLE, position->get(), 0.0f, howMany, 0.00001f, 1.0f, 0.05f, misc::msTous(std::chrono::milliseconds(750))));
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_PLAYER_PARTICLE, position->get(), 0.0f, howMany, 0.000005f, 1.0f, 0.05f, misc::msTous(std::chrono::milliseconds(1000))));
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_PLAYER_PARTICLE, position->get(), 0.0f, howMany, 0.0000025f, 1.0f, 0.05f, misc::msTous(std::chrono::milliseconds(1250))));

    //
    // One particle for the player's ship slowly going away
    auto size = m_player->getComponent<components::Size>();
    auto orientation = m_player->getComponent<components::Orientation>()->get();
    m_sysParticle->addEffect(std::make_unique<systems::CircleExpansionEffect>(content::KEY_IMAGE_PLAYER, position->get(), 0.0f, 0.0f, size->getOuterRadius(), 0.01f, orientation, misc::msTous(std::chrono::milliseconds(2000))));

    unregisterInputHandlers();
    removeEntity(m_player->getId());
    if (m_remainingNanoBots > 0)
    {
        m_remainingNanoBots--;
        resetPlayer();
    }
    else
    {
        m_rendererStatus->setMessage(m_level->getMessageFailure());
        m_remainingNanoBots--;
        m_updatePlayer = [](std::chrono::microseconds) {};
    }
}

// --------------------------------------------------------------
//
// Set the player update and render functions to a state that only
// checks if it is safe to start and no rendering.
//
// --------------------------------------------------------------
void GameModel::resetPlayer()
{
    if (m_player)
    {
        m_player->cleanup();
    }
    m_player = nullptr;

    m_rendererStatus->setMessage(m_level->getMessageReady());
    m_playerStartCountdown = misc::msTous(std::chrono::milliseconds(3000));
    m_updatePlayer = [this](std::chrono::microseconds elapsedTime) {
        m_playerStartCountdown -= elapsedTime;
        if (m_playerStartCountdown <= std::chrono::microseconds(0))
        {
            if (auto position = m_level->findSafeStart(-m_playerStartCountdown, m_viruses); position.has_value())
            {
                m_rendererStatus->setMessage("");
                startPlayer(position.value());
            }
        }
    };
}

// --------------------------------------------------------------
//
// Once the player is able to start, need to setup the controls,
// along with the update and rendering functions.
//
// --------------------------------------------------------------
void GameModel::startPlayer(math::Point2f position)
{
    // Get the audio playing ASAP
    SoundPlayer::play(content::KEY_AUDIO_PLAYER_START);

    m_player = entities::Player::create();
    addEntity(m_player);

    // A copy of the pointer is needed, because the controls might still happen during the next update when the player dies
    auto player = m_player;
    KeyboardInput::instance().registerKeyPressedHandler(Configuration::get<std::string>(config::KEYBOARD_UP), [player]() { player->startThrust(); });
    KeyboardInput::instance().registerKeyReleasedHandler(Configuration::get<std::string>(config::KEYBOARD_UP), [player]() { player->endThrust(); });
    KeyboardInput::instance().registerHandler(Configuration::get<std::string>(config::KEYBOARD_LEFT), true, std::chrono::microseconds(0), [player](std::chrono::microseconds elapsedTime) { player->rotateLeft(elapsedTime); });
    KeyboardInput::instance().registerHandler(Configuration::get<std::string>(config::KEYBOARD_RIGHT), true, std::chrono::microseconds(0), [player](std::chrono::microseconds elapsedTime) { player->rotateRight(elapsedTime); });
    //
    // Primary weapon fire
    KeyboardInput::instance().registerHandler(
        Configuration::get<std::string>(config::KEYBOARD_PRIMARY_FIRE), true, std::chrono::microseconds(0),
        [this, player]([[maybe_unused]] std::chrono::microseconds elapsedTime) {
            player->getPrimaryWeapon()->fire(
                [this](std::shared_ptr<entities::Entity>& bullet) { this->emitBullet(bullet); },
                [this](std::shared_ptr<entities::Entity>& bomb) { this->addEntity(bomb); });
        });

    //
    // Secondary weapon fire
    KeyboardInput::instance().registerHandler(
        Configuration::get<std::string>(config::KEYBOARD_SECONDARY_FIRE), true, std::chrono::microseconds(0),
        [this, player]([[maybe_unused]] std::chrono::microseconds elapsedTime) {
            player->getSecondaryWeapon()->fire(
                [this](std::shared_ptr<entities::Entity>& bullet) { this->emitBullet(bullet); },
                [this](std::shared_ptr<entities::Entity>& bomb) { this->addEntity(bomb); });
        });

    m_player->getComponent<components::Position>()->set(position);
    m_player->getComponent<components::Momentum>()->set({ 0.0f, 0.0f });
    m_player->getComponent<components::Orientation>()->set(0.0f);

    m_updatePlayer = [this](std::chrono::microseconds elapsedTime) {
        // This is a little sloppy, but it at least is a way to stop the time played clock in a winning condition
        if (m_viruses.size() > 0)
        {
            m_timePlayed += std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);
        }

        // Player is directly updated here, because there isn't (yet) a system associated with thrust
        m_player->update(elapsedTime);
    };

    // Particle effect as a visual cue to show where the player starts
    m_sysParticle->addEffect(std::make_unique<systems::PlayerStartEffect>(
        m_player->getComponent<components::Position>()->get(),
        static_cast<std::uint16_t>(300), misc::msTous(std::chrono::milliseconds(750))));
}

// --------------------------------------------------------------
//
// As entities are added to the game model, they are run by the systems
// to see if they are interested in knowing about them during their
// updates.
//
// --------------------------------------------------------------
void GameModel::addEntity(std::shared_ptr<entities::Entity> entity)
{
    if (entity == nullptr)
        return;

    m_sysAge->addEntity(entity);
    m_sysAnimatedSprite->addEntity(entity);
    m_sysBirth->addEntity(entity);
    m_sysHealth->addEntity(entity);
    m_sysLifetime->addEntity(entity);
    m_sysMovement->addEntity(entity);
    m_sysCollision->addEntity(entity);

    m_sysRendererSprite->addEntity(entity);
    m_sysRendererAnimatedSprite->addEntity(entity);
    m_sysRendererSarsCov2->addEntity(entity);
    // NOTE: The Powerup system doesn't act on entities, nothing to do here
    // NOTE: Particle system renderer does not have entities added to it, it is it's own separate thing
}

// --------------------------------------------------------------
//
// All systems must be given a chance to remove the entity.
//
// --------------------------------------------------------------
void GameModel::removeEntity(entities::Entity::IdType entityId)
{
    //
    // Let each of the systems know to remove the entity
    m_sysAge->removeEntity(entityId);
    m_sysAnimatedSprite->removeEntity(entityId);
    m_sysBirth->removeEntity(entityId);
    m_sysHealth->removeEntity(entityId);
    m_sysLifetime->removeEntity(entityId);
    m_sysMovement->removeEntity(entityId);
    m_sysCollision->removeEntity(entityId);

    m_sysRendererSprite->removeEntity(entityId);
    m_sysRendererAnimatedSprite->removeEntity(entityId);
    m_sysRendererSarsCov2->removeEntity(entityId);
    // NOTE: The Powerup system doesn't act on entities, nothing to do here
    // NOTE: Particle system renderer does not have entities added to it, it is it's own separate thing
}

// --------------------------------------------------------------
//
// Get the content that is shared in common with all levels specified here.
//
// --------------------------------------------------------------
void GameModel::loadContent()
{
    Content::load<sf::Texture>(content::KEY_IMAGE_SARSCOV2, Configuration::get<std::string>(config::VIRUS_IMAGE), nullptr, nullptr);
    Content::load<sf::Texture>(content::KEY_IMAGE_SARSCOV2_PARTICLE, Configuration::get<std::string>(config::VIRUS_PARTICLE), nullptr, nullptr);
    Content::load<sf::Texture>(content::KEY_IMAGE_PLAYER, Configuration::get<std::string>(config::PLAYER_IMAGE), nullptr, nullptr);
    Content::load<sf::Texture>(content::KEY_IMAGE_PLAYER_START_PARTICLE, Configuration::get<std::string>(config::PLAYER_START_PARTICLE), nullptr, nullptr);
    Content::load<sf::Texture>(content::KEY_IMAGE_PLAYER_PARTICLE, Configuration::get<std::string>(config::PLAYER_PARTICLE), nullptr, nullptr);

    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_THRUST, Configuration::get<std::string>(config::PLAYER_AUDIO_THRUST), nullptr, nullptr);
    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_VIRUS_DEATH, Configuration::get<std::string>(config::AUDIO_DEATH), nullptr, nullptr);
    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_PLAYER_DEATH, Configuration::get<std::string>(config::PLAYER_AUDIO_DEATH), nullptr, nullptr);
    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_PLAYER_START, Configuration::get<std::string>(config::PLAYER_AUDIO_START), nullptr, nullptr);

    Content::load<sf::Texture>(content::KEY_IMAGE_BASIC_GUN_BULLET, Configuration::get<std::string>(config::BASIC_GUN_IMAGE_BULLET), nullptr, nullptr);
    Content::load<sf::Texture>(content::KEY_IMAGE_BOMB, Configuration::get<std::string>(config::BOMB_IMAGE), nullptr, nullptr);

    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_BASIC_GUN_FIRE, Configuration::get<std::string>(config::BASIC_GUN_AUDIO_FIRE), nullptr, nullptr);
    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_BOMB_FIRE, Configuration::get<std::string>(config::BOMB_AUDIO_FIRE), nullptr, nullptr);
    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_BOMB_EXPLODE, Configuration::get<std::string>(config::BOMB_AUDIO_EXPLODE), nullptr, nullptr);

    Content::load<sf::Texture>(content::KEY_IMAGE_POWERUP_RAPID_FIRE, Configuration::get<std::string>(config::POWERUP_RAPID_FIRE_IMAGE), nullptr, nullptr);
    Content::load<sf::Texture>(content::KEY_IMAGE_POWERUP_SPREAD_FIRE, Configuration::get<std::string>(config::POWERUP_SPREAD_FIRE_IMAGE), nullptr, nullptr);
    Content::load<sf::Texture>(content::KEY_IMAGE_POWERUP_BOMB, Configuration::get<std::string>(config::POWERUP_BOMB_IMAGE), nullptr, nullptr);

    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_RAPID_GUN_POWERUP, Configuration::get<std::string>(config::POWERUP_RAPID_FIRE_AUDIO), nullptr, nullptr);
    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_SPREAD_GUN_POWERUP, Configuration::get<std::string>(config::POWERUP_SPREAD_FIRE_AUDIO), nullptr, nullptr);
    Content::load<sf::SoundBuffer>(content::KEY_AUDIO_BOMB_POWERUP, Configuration::get<std::string>(config::POWERUP_BOMB_AUDIO), nullptr, nullptr);
}

// --------------------------------------------------------------
//
// Method that indicates whether or not all content for the level
// is ready.
//
// --------------------------------------------------------------
bool GameModel::contentReady()
{
    return (!Content::instance().isError() && !Content::instance().anyPending());
}

// --------------------------------------------------------------
//
// Unregister the handlers setup for the game model.  Can't use
// .unregisterAll, because the view also has handler(s) registered.
//
// --------------------------------------------------------------
void GameModel::unregisterInputHandlers()
{
    KeyboardInput::instance().unregisterKeyPressedHandler(Configuration::get<std::string>(config::KEYBOARD_UP));
    KeyboardInput::instance().unregisterKeyReleasedHandler(Configuration::get<std::string>(config::KEYBOARD_UP));
    KeyboardInput::instance().unregisterHandler(Configuration::get<std::string>(config::KEYBOARD_LEFT));
    KeyboardInput::instance().unregisterHandler(Configuration::get<std::string>(config::KEYBOARD_RIGHT));

    KeyboardInput::instance().unregisterHandler(Configuration::get<std::string>(config::KEYBOARD_PRIMARY_FIRE));
    KeyboardInput::instance().unregisterHandler(Configuration::get<std::string>(config::KEYBOARD_SECONDARY_FIRE));
}
