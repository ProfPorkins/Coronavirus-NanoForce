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

namespace content
{
    using namespace std::string_literals;

    const auto KEY_FONT_TITLE = "font/title"s;
    const auto KEY_FONT_MENU = "font/menu"s;
    const auto KEY_FONT_CREDITS = "font/credits"s;
    const auto KEY_FONT_GAME_STATUS = "font/game-status"s;
    const auto KEY_FONT_SETTINGS = "font/settings"s;
    const auto KEY_FONT_LEVEL_SELECT = "font/level-select";
    const auto KEY_FONT_GAMEPLAY = "font/gameplay";

    const auto KEY_IMAGE_MENU_BACKGROUND = "image/menu-background"s;

    const auto KEY_MENU_ACTIVATE = "audio/menu-activate"s;
    const auto KEY_MENU_ACCEPT = "audio/menu-accept"s;

    const auto KEY_IMAGE_PLAYER = "image/player"s;
    const auto KEY_IMAGE_BASIC_GUN_BULLET = "image/bullet"s;
    const auto KEY_IMAGE_BOMB = "image/bomb"s;
    const auto KEY_IMAGE_SARSCOV2 = "image/sars-cov2"s;
    const auto KEY_IMAGE_SARSCOV2_PARTICLE = "image/sars-cov2-particle"s;
    const auto KEY_IMAGE_PLAYER_PARTICLE = "image/player-particle"s;
    const auto KEY_IMAGE_POWERUP_RAPID_FIRE = "image/powerup-rapid-fire"s;
    const auto KEY_IMAGE_POWERUP_SPREAD_FIRE = "image/powerup-spread-fire"s;
    const auto KEY_IMAGE_POWERUP_BOMB = "image/powerup-bomb"s;
    const auto KEY_IMAGE_PLAYER_START_PARTICLE = "image/player-start-particle"s;

    const auto KEY_AUDIO_VIRUS_DEATH = "audio/virus-death"s;
    const auto KEY_AUDIO_PLAYER_DEATH = "audio/player-death"s;
    const auto KEY_AUDIO_PLAYER_START = "audio/player-start"s;
    const auto KEY_AUDIO_THRUST = "audio/thrust"s;
    const auto KEY_AUDIO_BASIC_GUN_FIRE = "audio/fire"s;
    const auto KEY_AUDIO_RAPID_GUN_FIRE = "audio/fire"s;
    const auto KEY_AUDIO_SPREAD_GUN_FIRE = "audio/fire"s;
    const auto KEY_AUDIO_BOMB_FIRE = "audio/bomb-fire"s;
    const auto KEY_AUDIO_BOMB_EXPLODE = "audio/bomb-explode"s;
    const auto KEY_AUDIO_RAPID_GUN_POWERUP = "audio/powerup-rapid-fire"s;
    const auto KEY_AUDIO_SPREAD_GUN_POWERUP = "audio/powerup-spread-fire"s;
    const auto KEY_AUDIO_BOMB_POWERUP = "audio/powerup-bomb"s;
} // namespace content
