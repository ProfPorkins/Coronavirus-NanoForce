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

#pragma once

#include "entities/Player.hpp"
#include "entities/Powerup.hpp"
#include "entities/Virus.hpp"
#include "levels/Level.hpp"
#include "levels/LevelName.hpp"
#include "renderers/AnimatedSprite.hpp"
#include "renderers/Background.hpp"
#include "renderers/GameStatus.hpp"
#include "renderers/HUD.hpp"
#include "renderers/ParticleSystem.hpp"
#include "renderers/Sprite.hpp"
#include "renderers/Virus.hpp"
#include "systems/Age.hpp"
#include "systems/AnimatedSprite.hpp"
#include "systems/Birth.hpp"
#include "systems/Health.hpp"
#include "systems/Lifetime.hpp"
#include "systems/Movement.hpp"
#include "systems/ParticleSystem.hpp"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

class GameModel
{
  public:
    GameModel();

    static void selectLevel(levels::LevelName whichLevel);
    static void loadContent();

    void initialize();
    void shutdown();

    void update(const std::chrono::microseconds elapsedTime);
    void render(sf::RenderTarget& renderTarget, const std::chrono::microseconds elapsedTime);

  private:
    static levels::LevelName m_levelSelect;
    static std::atomic_bool m_contentError;

    std::unique_ptr<levels::Level> m_level{ nullptr };
    std::chrono::milliseconds m_timePlayed{ 0 };
    std::uint32_t m_virusesKilled{ 0 };

    std::unique_ptr<systems::Movement> m_sysMovement;
    std::unique_ptr<systems::Lifetime> m_sysLifetime;
    std::unique_ptr<systems::Birth> m_sysBirth;
    std::unique_ptr<systems::Health> m_sysHealth;
    std::unique_ptr<systems::AnimatedSprite> m_sysAnimatedSprite;
    std::unique_ptr<systems::Age> m_sysAge;
    systems::ParticleSystem m_sysParticle;

    std::shared_ptr<entities::Player> m_player{ nullptr };
    std::uint8_t m_remainingNanoBots{ 0 };
    std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Virus>> m_viruses;
    std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Entity>> m_bullets;
    std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Entity>> m_bombs;
    std::unordered_map<entities::Entity::IdType, std::shared_ptr<entities::Powerup>> m_powerups;
    std::vector<std::shared_ptr<entities::Virus>> m_newViruses;

    std::unique_ptr<renderers::Sprite> m_rendererPlayer;
    std::unique_ptr<renderers::Sprite> m_rendererBullet;
    std::unique_ptr<renderers::Sprite> m_rendererBomb;
    std::unique_ptr<renderers::Virus> m_rendererSarsCov2;
    std::unique_ptr<renderers::Background> m_rendererBackground;
    std::unique_ptr<renderers::ParticleSystem> m_rendererParticleSystem;
    std::unique_ptr<renderers::AnimatedSprite> m_rendererPowerup;
    std::unique_ptr<renderers::HUD> m_rendererHUD;
    std::unique_ptr<renderers::GameStatus> m_rendererStatus;

    std::function<void(std::chrono::microseconds)> m_updatePlayer;
    std::function<void(sf::RenderTarget&)> m_renderPlayer;
    std::chrono::microseconds m_playerStartCountdown{ 0 };

    void emitBullet(std::shared_ptr<entities::Entity>& bullet);
    void emitBomb(std::shared_ptr<entities::Entity>& bomb);
    void emitPowerup(std::shared_ptr<entities::Powerup>& powerup);
    void onVirusDeath(entities::Entity::IdType entityId);
    void onVirusBirth(entities::Entity::IdType parentId);
    void onPlayerDeath();
    void resetPlayer();
    void startPlayer(math::Point2f position);

    void addEntity(std::shared_ptr<entities::Entity> entity);
    void removeEntity(entities::Entity::IdType entityId);

    bool contentReady();
    void unregisterInputHandlers();
};
