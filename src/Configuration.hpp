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

#include "misc/math.hpp"

#include <rapidjson/document.h>
#include <string>
#include <vector>

// --------------------------------------------------------------
//
// Client Configuration settings
//
// Note: This is a Singleton
//
// --------------------------------------------------------------
class Configuration
{
  public:
    Configuration(const Configuration&) = delete;
    Configuration(Configuration&&) = delete;
    Configuration& operator=(const Configuration&) = delete;
    Configuration& operator=(Configuration&&) = delete;

    static auto& instance()
    {
        static Configuration instance;
        return instance;
    }

    bool initialize(const std::string_view jsonSettings, const std::string_view jsonDeveloper);
    std::string serialize();

    template <typename T>
    static T get(const std::vector<std::string>& path);

    template <typename T>
    static void set(const std::vector<std::string>& path, T value);

    //
    // Additional Graphics configuration settings
    //
    class Graphics
    {
      public:
        auto getScale() { return m_scale; }     // Gameplay may or may not scale to the resolution, depends on configuraton
        auto getScaleUI() { return m_scaleUI; } // UI always scales to the resolution
        auto getViewCoordinates() { return m_viewCoordinates; }
        auto getResolution() { return m_resolution; }
        auto getBpp() { return m_bpp; }

        void setViewCoordinates(math::Dimension2f coordinates)
        {
            m_viewCoordinates = coordinates;
            updateScale();
        }
        void setResolution(math::Dimension2u resolution) { m_resolution = resolution; }
        void setBpp(unsigned int bpp) { m_bpp = bpp; }

      private:
        friend Configuration;

        math::Vector2f m_scale{ 0.1f, 0.1f };
        math::Vector2f m_scaleUI{ m_scale };
        math::Dimension2f m_viewCoordinates{ 100.0f, 100.0f };
        math::Dimension2u m_resolution{ 640, 480 };
        unsigned int m_bpp{ 24 };

        void updateScale();
    };

    static Graphics& getGraphics() { return instance().m_graphics; }

  private:
    Configuration() {}

    rapidjson::Document m_domFull;
    rapidjson::Document m_domSettings;

    Graphics m_graphics;
};
