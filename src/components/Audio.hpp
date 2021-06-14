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

#include "Component.hpp"
#include "services/Content.hpp"

#include <SFML/Audio/Sound.hpp>
#include <string>

namespace components
{
    class Audio : public Component
    {
      public:
        Audio(std::string audioKey, bool load = false) :
            m_audioKey(audioKey)
        {
            //
            // Some audio (actually only one in this code) isn't fire and forget.  Sometimes
            // there is a need to start/stop a particular sound.
            if (load)
            {
                m_sound.setBuffer(*Content::get<sf::SoundBuffer>(audioKey));
            }
        }

        auto getKey() { return m_audioKey; }
        void start() { m_sound.play(); }
        void stop() { m_sound.stop(); }

      private:
        std::string m_audioKey;
        sf::Sound m_sound;
    };
} // namespace components
