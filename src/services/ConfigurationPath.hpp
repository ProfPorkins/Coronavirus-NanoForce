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

#include <string>
#include <vector>

namespace config
{
    using namespace std::string_literals;
    using config_path = std::vector<std::string>;
    // --------------------------------------------------------------
    //
    // Commonly used settings names
    //
    // --------------------------------------------------------------
    const auto DOM_FILENAME = "filename"s;
    const auto DOM_ENTITY = "entity"s;
    const auto DOM_COMPONENT = "component"s;
    const auto DOM_SIZE = "size"s;
    const auto DOM_MIN = "min"s;
    const auto DOM_MAX = "max"s;
    const auto DOM_MEAN = "mean"s;
    const auto DOM_STDEV = "stdev"s;
    const auto DOM_HEALTH = "health";
    const auto DOM_ROTATE_RATE = "rotate-rate"s;
    const auto DOM_IMAGE = "image"s;
    const auto DOM_AUDIO = "audio"s;
    const auto DOM_PARTICLE = "particle"s;
    const auto DOM_BACKGROUND = "background"s;
    const auto DOM_SETTINGS = "settings"s;
    const auto DOM_LEVEL_SELECT = "level-select"s;
    const auto DOM_DAMAGE = "damage"s;
    const auto DOM_FIRE_DELAY = "fire-delay"s;
    const auto DOM_LIFETIME = "lifetime"s;
    const auto DOM_FIRE = "fire"s;
    const auto DOM_BULLET = "bullet"s;
    const auto DOM_GESTATION = "gestation"s;
    const auto DOM_POWERUP = "powerup"s;
    const auto DOM_SPRITE_COUNT = "sprite-count"s;
    const auto DOM_SPRITE_TIME = "sprite-time"s;
    const auto DOM_BOMB = "bomb"s;
    const auto DOM_EXPLODE = "explode"s;
    const auto DOM_BULLETS = "bullets"s;
    const auto DOM_INITIAL_VIRUS_COUNT = "initial-virus-count"s;
    const auto DOM_MAX_VIRUS_COUNT = "max-virus-count"s;
    const auto DOM_NANO_BOT_COUNT = "nano-bot-count";
    const auto DOM_READY = "ready"s;
    const auto DOM_SUCCESS = "success"s;
    const auto DOM_FAILURE = "failure"s;
    const auto DOM_MESSAGES = "messages"s;
    const auto DOM_NAME = "name"s;

    // --------------------------------------------------------------
    //
    // Developer configuration names
    //
    // --------------------------------------------------------------
    const auto DOM_DEVELOPER = "developer"s;
    const config_path DEVELOPER_MAIN_MENU = { DOM_DEVELOPER, "main-menu"s }; // true if main menu do be displayed, otherwise directly join game

    // --------------------------------------------------------------
    //
    // Graphics configuration names
    //
    // --------------------------------------------------------------
    const auto DOM_GRAPHICS = "graphics"s;
    const auto DOM_GRAPHICS_RESOLUTION = "resolution"s;
    const config_path GRAPHICS_WIDTH = { DOM_GRAPHICS, DOM_GRAPHICS_RESOLUTION, "width"s };
    const config_path GRAPHICS_HEIGHT = { DOM_GRAPHICS, DOM_GRAPHICS_RESOLUTION, "height"s };
    const config_path GRAPHICS_BPP = { DOM_GRAPHICS, "bpp"s };                                 // bits per pixel
    const config_path GRAPHICS_FULL_SCREEN = { DOM_GRAPHICS, "full-screen"s };                 // true if it should run full screen
    const config_path GRAPHICS_VSYNC = { DOM_GRAPHICS, "vsync"s };                             // true if v-sync should be on
    const config_path GRAPHICS_SCALE_TO_RESOLUTION = { DOM_GRAPHICS, "scale-to-resolution"s }; // True if graphics grow/shink with resolution

    // --------------------------------------------------------------
    //
    // Keyboard control configuration names
    //
    // --------------------------------------------------------------
    const auto DOM_KEYBOARD = "keyboard"s;
    const config_path KEYBOARD_UP = { DOM_KEYBOARD, "up"s };
    const config_path KEYBOARD_DOWN = { DOM_KEYBOARD, "down"s };
    const config_path KEYBOARD_LEFT = { DOM_KEYBOARD, "left"s };
    const config_path KEYBOARD_RIGHT = { DOM_KEYBOARD, "right"s };
    const config_path KEYBOARD_PRIMARY_FIRE = { DOM_KEYBOARD, "primary-fire"s };
    const config_path KEYBOARD_SECONDARY_FIRE = { DOM_KEYBOARD, "secondary-fire"s };

    // --------------------------------------------------------------
    //
    // Music configuration names
    //
    // --------------------------------------------------------------
    const auto DOM_MUSIC = "music";
    const config_path PLAY_BACKGROUND_MUSIC = { DOM_MUSIC, "play-background" };

    // --------------------------------------------------------------
    //
    // Content configuration names
    //
    // --------------------------------------------------------------
    const auto DOM_CONTENT = "content"s;
    const auto DOM_FONT = "font"s;
    const auto DOM_TITLE = "title"s;
    const auto DOM_MENU = "menu"s;
    const auto DOM_CREDITS = "credits"s;
    const auto DOM_GAMEPLAY = "gameplay"s;
    const auto DOM_GAME_STATUS = "game-status"s;
    const auto DOM_ITEM_SIZE = "item-size"s;
    const auto DOM_TITLE_SIZE = "title-size"s;

    const config_path FONT_TITLE_FILENAME = { DOM_CONTENT, DOM_FONT, DOM_TITLE, DOM_FILENAME };
    const config_path FONT_TITLE_SIZE = { DOM_CONTENT, DOM_FONT, DOM_TITLE, DOM_SIZE };
    const config_path FONT_MENU_FILENAME = { DOM_CONTENT, DOM_FONT, DOM_MENU, DOM_FILENAME };
    const config_path FONT_MENU_SIZE = { DOM_CONTENT, DOM_FONT, DOM_MENU, DOM_SIZE };

    const config_path FONT_CREDITS_FILENAME = { DOM_CONTENT, DOM_FONT, DOM_CREDITS, DOM_FILENAME };
    const config_path FONT_CREDITS_TITLE_SIZE = { DOM_CONTENT, DOM_FONT, DOM_CREDITS, DOM_TITLE_SIZE };
    const config_path FONT_CREDITS_ITEM_SIZE = { DOM_CONTENT, DOM_FONT, DOM_CREDITS, DOM_ITEM_SIZE };

    const config_path FONT_GAME_STATUS_FILENAME = { DOM_CONTENT, DOM_FONT, DOM_GAME_STATUS, DOM_FILENAME };
    const config_path FONT_GAME_STATUS_SIZE = { DOM_CONTENT, DOM_FONT, DOM_GAME_STATUS, DOM_SIZE };

    const config_path FONT_SETTINGS_FILENAME = { DOM_CONTENT, DOM_FONT, DOM_SETTINGS, DOM_FILENAME };
    const config_path FONT_SETTINGS_ITEM_SIZE = { DOM_CONTENT, DOM_FONT, DOM_SETTINGS, DOM_ITEM_SIZE };
    const config_path FONT_SETTINGS_MESSAGE_SIZE = { DOM_CONTENT, DOM_FONT, DOM_SETTINGS, "message-size"s };

    const config_path FONT_LEVEL_SELECT_FILENAME = { DOM_CONTENT, DOM_FONT, DOM_LEVEL_SELECT, DOM_FILENAME };
    const config_path FONT_LEVEL_SELECT_ITEM_SIZE = { DOM_CONTENT, DOM_FONT, DOM_LEVEL_SELECT, DOM_ITEM_SIZE };
    const config_path FONT_LEVEL_SELECT_TITLE_SIZE = { DOM_CONTENT, DOM_FONT, DOM_LEVEL_SELECT, DOM_TITLE_SIZE };

    const config_path FONT_GAMEPLAY_FILENAME = { DOM_CONTENT, DOM_FONT, DOM_GAMEPLAY, DOM_FILENAME };
    const config_path FONT_GAMEPLAY_SCORE_SIZE = { DOM_CONTENT, DOM_FONT, DOM_GAMEPLAY, "score-size"s };

    const config_path AUDIO_MENU_ACTIVATE = { DOM_CONTENT, DOM_AUDIO, DOM_MENU, "activate"s };
    const config_path AUDIO_MENU_ACCEPT = { DOM_CONTENT, DOM_AUDIO, DOM_MENU, "accept"s };

    const config_path IMAGE_MENU_BACKGROUND = { DOM_CONTENT, DOM_IMAGE, "menu-background"s };

    // --------------------------------------------------------------
    //
    // Entity names
    //
    // --------------------------------------------------------------
    const auto ENTITY_PLAYER = "player"s;
    const auto ENTITY_SARSCOV2 = "sars-cov2"s;
    const auto ENTITY_WEAPON_BASIC_GUN = "basic-gun"s;
    const auto ENTITY_WEAPON_RAPID_FIRE = "rapid-fire"s;
    const auto ENTITY_WEAPON_SPREAD_FIRE = "spread-fire"s;
    const auto ENTITY_WEAPON_BOMB = "bomb"s;

    // --------------------------------------------------------------
    //
    // Entity: player attributes
    //
    // --------------------------------------------------------------
    const config_path PLAYER_THRUST_RATE = { DOM_ENTITY, ENTITY_PLAYER, "thrust-rate" };
    const config_path PLAYER_DRAG_RATE = { DOM_ENTITY, ENTITY_PLAYER, "drag-rate" };
    const config_path PLAYER_ROTATE_RATE = { DOM_ENTITY, ENTITY_PLAYER, DOM_ROTATE_RATE };
    const config_path PLAYER_SIZE = { DOM_ENTITY, ENTITY_PLAYER, DOM_SIZE };
    const config_path PLAYER_MAX_SPEED = { DOM_ENTITY, ENTITY_PLAYER, "max-speed" };
    const config_path PLAYER_IMAGE = { DOM_ENTITY, ENTITY_PLAYER, DOM_IMAGE, "ship"s };
    const config_path PLAYER_PARTICLE = { DOM_ENTITY, ENTITY_PLAYER, DOM_IMAGE, "destroy-particle"s };
    const config_path PLAYER_START_PARTICLE = { DOM_ENTITY, ENTITY_PLAYER, DOM_IMAGE, "start-particle"s };
    const config_path PLAYER_AUDIO_THRUST = { DOM_ENTITY, ENTITY_PLAYER, DOM_AUDIO, "thrust"s };
    const config_path PLAYER_AUDIO_DEATH = { DOM_ENTITY, ENTITY_PLAYER, DOM_AUDIO, "death"s };
    const config_path PLAYER_AUDIO_START = { DOM_ENTITY, ENTITY_PLAYER, DOM_AUDIO, "start"s };

    // --------------------------------------------------------------
    //
    // Entity: sars-cov2 attributes (some of them)
    //
    // --------------------------------------------------------------
    const config_path VIRUS_IMAGE = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_IMAGE, "virus"s };
    const config_path VIRUS_PARTICLE = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_IMAGE, "particle"s };
    const config_path AUDIO_DEATH = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_AUDIO, "death"s };

    const config_path VIRUS_ROTATE_RATE = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_ROTATE_RATE };
    const config_path VIRUS_SPEED = { DOM_ENTITY, ENTITY_SARSCOV2, "speed" };
    const config_path VIRUS_SIZE_MIN = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_SIZE, DOM_MIN };
    const config_path VIRUS_SIZE_MAX = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_SIZE, DOM_MAX };
    const config_path VIRUS_HEALTH_START = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_HEALTH, "start"s };
    const config_path VIRUS_HEALTH_INCREMENTS = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_HEALTH, "increments"s };
    const config_path VIRUS_HEALTH_INCREMENT_TIME = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_HEALTH, "increment-time"s };
    const config_path VIRUS_AGE_MATURITY = { DOM_ENTITY, ENTITY_SARSCOV2, "age-maturity"s };
    const config_path VIRUS_GEST_MIN = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_GESTATION, DOM_MIN };
    const config_path VIRUS_GEST_MEAN = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_GESTATION, DOM_MEAN };
    const config_path VIRUS_GEST_STDEV = { DOM_ENTITY, ENTITY_SARSCOV2, DOM_GESTATION, DOM_STDEV };

    // --------------------------------------------------------------
    //
    // Entity: base-gun attributes
    //
    // --------------------------------------------------------------
    const config_path BASIC_GUN_IMAGE_BULLET = { DOM_ENTITY, ENTITY_WEAPON_BASIC_GUN, DOM_IMAGE, DOM_BULLET };
    const config_path BOMB_IMAGE = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_IMAGE, DOM_BULLET };
    const config_path BASIC_GUN_AUDIO_FIRE = { DOM_ENTITY, ENTITY_WEAPON_BASIC_GUN, DOM_AUDIO, DOM_FIRE };
    const config_path BOMB_AUDIO_FIRE = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_AUDIO, DOM_FIRE };
    const config_path BOMB_AUDIO_EXPLODE = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_AUDIO, DOM_EXPLODE };

    // --------------------------------------------------------------
    //
    // Entity: powerups
    //
    // --------------------------------------------------------------
    const config_path POWERUP_RAPID_FIRE_IMAGE = { DOM_ENTITY, ENTITY_WEAPON_RAPID_FIRE, DOM_POWERUP, DOM_IMAGE };
    const config_path POWERUP_RAPID_FIRE_AUDIO = { DOM_ENTITY, ENTITY_WEAPON_RAPID_FIRE, DOM_POWERUP, DOM_AUDIO };

    const config_path POWERUP_SPREAD_FIRE_IMAGE = { DOM_ENTITY, ENTITY_WEAPON_SPREAD_FIRE, DOM_POWERUP, DOM_IMAGE };
    const config_path POWERUP_SPREAD_FIRE_AUDIO = { DOM_ENTITY, ENTITY_WEAPON_SPREAD_FIRE, DOM_POWERUP, DOM_AUDIO };

    const config_path POWERUP_BOMB_IMAGE = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_POWERUP, DOM_IMAGE };
    const config_path POWERUP_BOMB_AUDIO = { DOM_ENTITY, ENTITY_WEAPON_BOMB, DOM_POWERUP, DOM_AUDIO };

    // --------------------------------------------------------------
    //
    // Level names
    //
    // --------------------------------------------------------------
    const auto DOM_LEVELS = "levels"s;
    const auto TRAINING_1 = "training-1"s;
    const auto TRAINING_2 = "training-2"s;
    const auto TRAINING_3 = "training-3"s;
    const auto TRAINING_4 = "training-4"s;
    const auto TRAINING_5 = "training-5"s;

    const auto PATIENT_1 = "patient-1"s;
    const auto PATIENT_2 = "patient-2"s;
    const auto PATIENT_3 = "patient-3"s;

    const config_path TRAINING_LEVEL1_NAME = { DOM_LEVELS, TRAINING_1, DOM_NAME };
    const config_path TRAINING_LEVEL2_NAME = { DOM_LEVELS, TRAINING_2, DOM_NAME };
    const config_path TRAINING_LEVEL3_NAME = { DOM_LEVELS, TRAINING_3, DOM_NAME };
    const config_path TRAINING_LEVEL4_NAME = { DOM_LEVELS, TRAINING_4, DOM_NAME };
    const config_path TRAINING_LEVEL5_NAME = { DOM_LEVELS, TRAINING_5, DOM_NAME };

    const config_path PATIENT_LEVEL1_NAME = { DOM_LEVELS, PATIENT_1, DOM_NAME };
    const config_path PATIENT_LEVEL2_NAME = { DOM_LEVELS, PATIENT_2, DOM_NAME };
    const config_path PATIENT_LEVEL3_NAME = { DOM_LEVELS, PATIENT_3, DOM_NAME };

} // namespace config
