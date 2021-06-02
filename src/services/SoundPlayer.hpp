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

#include "misc/ConcurrentQueue.hpp"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

// --------------------------------------------------------------
//
// This is used throughout the game to play sounds.  It creates
// the sf::Sound object that are responsible for the individual
// sounds.  Music is not handled by this class, because there
// aren't any serious issues related to playing music that need
// something more complex like this.
//
// I struggled to come up with a good name for it.  I didn't want
// to call it a "system", because there are other kinds of systems.
// I also didn't want to call it a "manager", because it seems
// like everything is called a manager.  In the end, I went with
// SoundPlayer, not great, but workable.
//
// Note: This is a Singleton
//
// --------------------------------------------------------------
class SoundPlayer
{
  public:
    SoundPlayer(const SoundPlayer&) = delete;
    SoundPlayer(SoundPlayer&&) = delete;
    SoundPlayer& operator=(const SoundPlayer&) = delete;
    SoundPlayer& operator=(SoundPlayer&&) = delete;

    void initialize();
    void terminate();

    static auto& instance()
    {
        static SoundPlayer instance;
        return instance;
    }

    static void play(const std::string& key, float volume = 100.0f);

  private:
    SoundPlayer() {}

    class Task
    {
      public:
        Task(std::string key, float volume) :
            key(key),
            volume(volume)
        {
        }

        std::string key;
        float volume;
    };

    // Storing shared_ptr because if you copy sf::Sound it stops playing, need to keep
    // the original sf::Sound object around all the time.  Could change it to unique_ptr
    // with a bunch of std::move I suppose, to improve efficiency.
    ConcurrentQueue<std::shared_ptr<sf::Sound>> m_sounds;

    bool m_done{ false };
    std::unique_ptr<std::thread> m_thread;
    ConcurrentQueue<Task> m_tasks;
    std::condition_variable m_eventTasks;
    std::mutex m_mutexTasks;

    void run();
};
