#include "Configuration.hpp"

#include <cstdint>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

bool Configuration::initialize(const std::string& json)
{
    auto valid = parseFromJSON(json);
    //
    // Have to call this because the resolu tiuon and scale-to-resolution settings are needed in
    // order to set this correctly.
    m_graphics.updateScale();

    return valid;
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
    m_dom.Accept(writer);

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
    return findValue(m_dom, path)->value.GetBool();
}

template <>
std::string Configuration::get(const std::vector<std::string>& path)
{
    return findValue(m_dom, path)->value.GetString();
}

template <>
std::uint8_t Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<std::uint8_t>(findValue(m_dom, path)->value.GetUint());
}

template <>
std::uint16_t Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<std::uint16_t>(findValue(m_dom, path)->value.GetUint());
}

template <>
float Configuration::get(const std::vector<std::string>& path)
{
    return static_cast<float>(findValue(m_dom, path)->value.GetDouble());
}

template <>
void Configuration::set(const std::vector<std::string>& path, std::string value)
{
    findValue(m_dom, path)->value.SetString(value.c_str(), static_cast<rapidjson::SizeType>(value.size()), m_dom.GetAllocator());
}

template <>
void Configuration::set(const std::vector<std::string>& path, std::uint16_t value)
{
    findValue(m_dom, path)->value.SetUint(value);
}

template <>
void Configuration::set(const std::vector<std::string>& path, std::uint8_t value)
{
    findValue(m_dom, path)->value.SetUint(value);
}

template <>
void Configuration::set(const std::vector<std::string>& path, bool value)
{
    findValue(m_dom, path)->value.SetBool(value);
}

template <>
void Configuration::set(const std::vector<std::string>& path, float value)
{
    findValue(m_dom, path)->value.SetDouble(value);
}

// --------------------------------------------------------------
//
// Parse the JSON text into meaningful configuration settings exposed
// from the Configuration class.
//
// --------------------------------------------------------------
bool Configuration::parseFromJSON(const std::string& json)
{
    m_dom.Parse(json.c_str());

    return true;
}

// --------------------------------------------------------------
//
// When either the resolution or scaling option changes, need to update
// the SFML scaling factor in order to render correctly.
//
// --------------------------------------------------------------
void Configuration::Graphics::updateScale()
{
    if (!Configuration::i().get<bool>(config::GRAPHICS_SCALE_TO_RESOLUTION))
    {
        m_scale = { m_viewCoordinates.x / Configuration::i().get<std::uint16_t>(config::GRAPHICS_WIDTH), m_viewCoordinates.y / Configuration::i().get<std::uint16_t>(config::GRAPHICS_HEIGHT) };
    }
}

