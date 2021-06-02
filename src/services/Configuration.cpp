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

#include "services/Configuration.hpp"

#include "services/ConfigurationPath.hpp"

#include <chrono>
#include <cstdint>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

// --------------------------------------------------------------
//
// Why two configuration files and why can the developer one not
// exist?
// For end users, I want an ability that allows various settings
// to be modified, like controls, resolution, etc.  But for the
// parameterized game-play data, I don't want the user to be able
// to modify them.  But, during development, I want it to be easy
// to change the game-play data, especially without having to recompile
// any code.  Therefore, the developer json, if it exists on the
// file system, will be used, but if it doesn't, then the embedded
// data is used.  Someone could figure this out and create their
// own develop json file and get around it, but I'll live with it
// because that means the game is massively popular and someone
// wanted to hack it!
//
// Online tool to convert JSON to a C++ string: https://tools.knowledgewalls.com/jsontostring
//
// --------------------------------------------------------------
bool Configuration::initialize(const std::string_view jsonSettings, const std::string_view jsonDeveloper)
{
    std::string jsonFull;
    // If running with a release json file, have to merge in the hard-coded settings
    if (jsonDeveloper.length() == 0)
    {
        // When updating remember:
        //  1.  Remove the leading {
        //  2.  Add a leading ,
        static const std::string jsonGame = ",\"developer\":{\"main-menu\":true},\"content\":{\"font\":{\"title\":{\"filename\":\"Shojumaru-Regular.ttf\",\"size\":60},\"menu\":{\"filename\":\"Shojumaru-Regular.ttf\",\"size\":40},\"level-select\":{\"filename\":\"Shojumaru-Regular.ttf\",\"title-size\":40,\"item-size\":24},\"game-status\":{\"filename\":\"Shojumaru-Regular.ttf\",\"size\":40},\"credits\":{\"filename\":\"Shojumaru-Regular.ttf\",\"title-size\":22,\"item-size\":36},\"settings\":{\"filename\":\"Shojumaru-Regular.ttf\",\"item-size\":36,\"message-size\":24},\"leaderboards\":{\"filename\":\"Shojumaru-Regular.ttf\",\"selection-size\":36,\"header-size\":32,\"entry-size\":28},\"gameplay\":{\"filename\":\"Shojumaru-Regular.ttf\",\"score-size\":24}},\"audio\":{\"menu\":{\"activate\":\"menu-activate.wav\",\"accept\":\"menu-accept.wav\"}},\"image\":{\"menu-background\":\"menu-background-2.jpg\"}},\"entity\":{\"player\":{\"thrust-rate\":1.0e-10,\"max-speed\":3.0e-5,\"drag-rate\":5.0e-9,\"rotate-rate\":0.00025,\"size\":3,\"image\":{\"ship\":\"playerShip1_blue.png\",\"destroy-particle\":\"virus-particle.png\",\"start-particle\":\"player-start-particle.png\"},\"audio\":{\"thrust\":\"thruster-level3.ogg\",\"death\":\"player-death.ogg\",\"start\":\"player-start.wav\"}},\"sars-cov2\":{\"rotate-rate\":0.02,\"speed\":1.25e-5,\"size\":{\"min\":0.5,\"max\":4},\"health\":{\"start\":4,\"increments\":24,\"increment-time\":1000},\"age-maturity\":20000,\"gestation\":{\"min\":2000,\"mean\":10000,\"stdev\":4000},\"image\":{\"virus\":\"sars-cov-2.png\",\"particle\":\"virus-particle.png\"},\"audio\":{\"death\":\"virus-death.ogg\"}},\"basic-gun\":{\"fire-delay\":200,\"damage\":1,\"lifetime\":2000,\"size\":0.45,\"image\":{\"bullet\":\"antibody.png\"},\"audio\":{\"fire\":\"fire.ogg\"}},\"rapid-fire\":{\"fire-delay\":100,\"damage\":1,\"lifetime\":2000,\"size\":0.45,\"image\":{\"bullet\":\"antibody.png\"},\"audio\":{\"fire\":\"fire.ogg\"},\"powerup\":{\"size\":2,\"lifetime\":15000,\"image\":\"powerup-rapid-fire.png\",\"sprite-count\":6,\"sprite-time\":100,\"audio\":\"powerup.ogg\"}},\"spread-fire\":{\"fire-delay\":200,\"damage\":1,\"lifetime\":2000,\"size\":0.45,\"image\":{\"bullet\":\"antibody.png\"},\"audio\":{\"fire\":\"fire.ogg\"},\"powerup\":{\"size\":2,\"lifetime\":20000,\"image\":\"powerup-spread-fire.png\",\"sprite-count\":6,\"sprite-time\":100,\"audio\":\"powerup.ogg\"}},\"bomb\":{\"fire-delay\":1000,\"damage\":0,\"lifetime\":1000,\"size\":1.5,\"bullets\":{\"count\":40,\"damage\":1,\"size\":0.45,\"lifetime\":2000},\"image\":{\"bullet\":\"bomb.png\"},\"audio\":{\"fire\":\"fire-bomb.ogg\",\"explode\":\"explode-bomb.ogg\"},\"powerup\":{\"size\":2,\"lifetime\":25000,\"image\":\"powerup-bomb.png\",\"sprite-count\":6,\"sprite-time\":100,\"audio\":\"powerup.ogg\"}}},\"levels\":{\"training-1\":{\"name\":\"Familiarization\",\"content\":{\"image\":{\"background\":\"petri-1.png\"},\"music\":{\"background\":\"background-music-2.ogg\"},\"messages\":{\"ready\":\"prepare for training\",\"failure\":\"more training needed\",\"success\":\"training successful\"}},\"settings\":{\"initial-virus-count\":2,\"max-virus-count\":3,\"nano-bot-count\":3,\"min-powerup-time\":0,\"bomb-powerup-time\":0,\"rapid-fire-powerup-time\":0,\"spread-fire-powerup-time\":0,\"leaderboard-max-viruses-killed\":0}},\"training-2\":{\"name\":\"Bomb\",\"content\":{\"image\":{\"background\":\"petri-1.png\"},\"music\":{\"background\":\"background-music-2.ogg\"},\"messages\":{\"ready\":\"look for the bomb weapon\",\"failure\":\"more training needed\",\"success\":\"training successful\"}},\"settings\":{\"initial-virus-count\":3,\"max-virus-count\":5,\"nano-bot-count\":3,\"min-powerup-time\":0,\"bomb-powerup-time\":20000,\"rapid-fire-powerup-time\":0,\"spread-fire-powerup-time\":0,\"leaderboard-max-viruses-killed\":0}},\"training-3\":{\"name\":\"Rapid Fire\",\"content\":{\"image\":{\"background\":\"petri-1.png\"},\"music\":{\"background\":\"background-music-2.ogg\"},\"messages\":{\"ready\":\"look for the rapid fire upgrade\",\"failure\":\"more training needed\",\"success\":\"training successful\"}},\"settings\":{\"initial-virus-count\":3,\"max-virus-count\":6,\"nano-bot-count\":3,\"min-powerup-time\":0,\"bomb-powerup-time\":0,\"rapid-fire-powerup-time\":20000,\"spread-fire-powerup-time\":0,\"leaderboard-max-viruses-killed\":0}},\"training-4\":{\"name\":\"Spread Fire\",\"content\":{\"image\":{\"background\":\"petri-1.png\"},\"music\":{\"background\":\"background-music-2.ogg\"},\"messages\":{\"ready\":\"look for the spread fire upgrade\",\"failure\":\"more training needed\",\"success\":\"training successful\"}},\"settings\":{\"initial-virus-count\":3,\"max-virus-count\":6,\"nano-bot-count\":3,\"min-powerup-time\":0,\"bomb-powerup-time\":0,\"rapid-fire-powerup-time\":0,\"spread-fire-powerup-time\":20000,\"leaderboard-max-viruses-killed\":0}},\"training-5\":{\"name\":\"Final Checkout\",\"content\":{\"image\":{\"background\":\"petri-5.png\"},\"music\":{\"background\":\"background-music-2.ogg\"},\"messages\":{\"ready\":\"prepare for final training\",\"failure\":\"more training needed\",\"success\":\"training successful\"}},\"settings\":{\"initial-virus-count\":4,\"max-virus-count\":8,\"nano-bot-count\":3,\"min-powerup-time\":5000,\"bomb-powerup-time\":30000,\"rapid-fire-powerup-time\":30000,\"spread-fire-powerup-time\":30000,\"leaderboard-max-viruses-killed\":0}},\"patient-1\":{\"name\":\"Newly Infected\",\"content\":{\"image\":{\"background\":\"petri-2.png\"},\"music\":{\"background\":\"background-music-1.ogg\"},\"messages\":{\"ready\":\"prepare for battle\",\"failure\":\"patient in danger of dying\",\"success\":\"Patient has recovered\"}},\"settings\":{\"initial-virus-count\":3,\"max-virus-count\":10,\"nano-bot-count\":3,\"min-powerup-time\":5000,\"bomb-powerup-time\":30000,\"rapid-fire-powerup-time\":30000,\"spread-fire-powerup-time\":30000,\"leaderboard-max-viruses-killed\":20}},\"patient-2\":{\"name\":\"On Ventilator\",\"content\":{\"image\":{\"background\":\"petri-3.png\"},\"music\":{\"background\":\"background-music-1.ogg\"},\"messages\":{\"ready\":\"prepare for battle\",\"failure\":\"patient in danger of dying\",\"success\":\"Patient has recovered\"}},\"settings\":{\"initial-virus-count\":6,\"max-virus-count\":14,\"nano-bot-count\":3,\"min-powerup-time\":7500,\"bomb-powerup-time\":40000,\"rapid-fire-powerup-time\":40000,\"spread-fire-powerup-time\":40000,\"leaderboard-max-viruses-killed\":30}},\"patient-3\":{\"name\":\"Near Death\",\"content\":{\"image\":{\"background\":\"petri-4.png\"},\"music\":{\"background\":\"background-music-1.ogg\"},\"messages\":{\"ready\":\"prepare for battle\",\"failure\":\"patient has died\",\"success\":\"Patient has recovered\"}},\"settings\":{\"initial-virus-count\":8,\"max-virus-count\":18,\"nano-bot-count\":3,\"min-powerup-time\":10000,\"bomb-powerup-time\":50000,\"rapid-fire-powerup-time\":50000,\"spread-fire-powerup-time\":50000,\"leaderboard-max-viruses-killed\":50}}}}";
        std::string_view json1 = jsonSettings.substr(0, jsonSettings.size() - 2);
        jsonFull = std::string(json1) + jsonGame;
    }
    else
    {
        std::string_view json1 = jsonSettings.substr(0, jsonSettings.size() - 2);
        std::string_view json2 = jsonDeveloper.substr(1, jsonDeveloper.size() - 1);
        jsonFull = std::string(json1) + ", " + std::string(json2);
    }

    m_domFull.Parse(jsonFull.data());
    m_domSettings.Parse(jsonSettings.data());

    //
    // Have to call this because the resolution and scale-to-resolution settings are needed in
    // order to set this correctly.
    m_graphics.updateScale();

    return true;
}

// --------------------------------------------------------------
//
// Serialize the configuration to a std::string for persistence.
//
// --------------------------------------------------------------
std::string Configuration::serialize()
{
    //
    // Use pretty writing to make a nicely formatted string for serialization (human readable)
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer, rapidjson::UTF8<>> writer(buffer);
    m_domSettings.Accept(writer);

    return buffer.GetString();
}

// --------------------------------------------------------------
//
// Helper method used to find the member iteration in the rapidJSON document.
//
// --------------------------------------------------------------
auto findValue(rapidjson::Document& dom, const std::vector<std::string>& path)
{
    auto itr = path.begin();
    auto node = dom.FindMember(itr->c_str());
    while (++itr != path.end())
    {
        node = node->value.FindMember(itr->c_str());
    }

    return node;
}

// --------------------------------------------------------------
//
// Various template specializations for get<>ing and set<>ing values
// in the configuration.
//
// --------------------------------------------------------------
template <>
bool Configuration::get(const std::vector<std::string>& path)
{
    return findValue(instance().m_domFull, path)->value.GetBool();
}

template <>
std::string Configuration::get(const std::vector<std::string>& path)
{
    return findValue(instance().m_domFull, path)->value.GetString();
}

template <>
std::uint8_t Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<std::uint8_t>(findValue(instance().m_domFull, path)->value.GetUint());
}

template <>
std::uint16_t Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<std::uint16_t>(findValue(instance().m_domFull, path)->value.GetUint());
}

template <>
std::uint32_t Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<std::uint32_t>(findValue(instance().m_domFull, path)->value.GetUint());
}

template <>
int Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<int>(findValue(instance().m_domFull, path)->value.GetInt());
}

template <>
float Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<float>(findValue(instance().m_domFull, path)->value.GetDouble());
}

template <>
double Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<double>(findValue(instance().m_domFull, path)->value.GetDouble());
}

template <>
std::chrono::milliseconds Configuration::get(const std::vector<std::string>& path)
{
    auto ms = static_cast<std::uint32_t>(findValue(instance().m_domFull, path)->value.GetUint());
    return std::chrono::milliseconds(ms);
}

template <>
void Configuration::set(const std::vector<std::string>& path, std::string value)
{
    // Modify values in both, because when serialization takes place, want the
    // changed values to persist.
    findValue(instance().m_domFull, path)->value.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()), instance().m_domFull.GetAllocator());
    findValue(instance().m_domSettings, path)->value.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()), instance().m_domSettings.GetAllocator());
}

template <>
void Configuration::set(const std::vector<std::string>& path, std::uint16_t value)
{
    findValue(instance().m_domFull, path)->value.SetUint(value);
    findValue(instance().m_domSettings, path)->value.SetUint(value);
}

template <>
void Configuration::set(const std::vector<std::string>& path, std::uint8_t value)
{
    findValue(instance().m_domFull, path)->value.SetUint(value);
    findValue(instance().m_domSettings, path)->value.SetUint(value);
}

template <>
void Configuration::set(const std::vector<std::string>& path, bool value)
{
    findValue(instance().m_domFull, path)->value.SetBool(value);
    findValue(instance().m_domSettings, path)->value.SetBool(value);
}

template <>
void Configuration::set(const std::vector<std::string>& path, float value)
{
    findValue(instance().m_domFull, path)->value.SetDouble(value);
    findValue(instance().m_domSettings, path)->value.SetDouble(value);
}

// --------------------------------------------------------------
//
// When either the resolution or scaling option changes, need to update
// the SFML scaling factor in order to render correctly.
//
// --------------------------------------------------------------
void Configuration::Graphics::updateScale()
{
    static bool uiScaled{ false };

    if (!Configuration::get<bool>(config::GRAPHICS_SCALE_TO_RESOLUTION))
    {
        m_scale = { m_viewCoordinates.width / Configuration::getGraphics().getResolution().width, m_viewCoordinates.height / Configuration::getGraphics().getResolution().height };
    }

    if (!uiScaled)
    {
        auto aspectRatio = Configuration::get<float>(config::GRAPHICS_WIDTH) / Configuration::getGraphics().getResolution().height;
        if (aspectRatio < 1.0)
        {
            m_scaleUI.y *= aspectRatio;
        }
        uiScaled = true;
    }
}
