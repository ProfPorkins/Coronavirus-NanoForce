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

#include "SoundPlayer.hpp"

#include "Content.hpp"
#include "ContentKey.hpp"

// --------------------------------------------------------------
//
// Call this one time at program startup.  This gets the worker
// thread up and running.
//
// --------------------------------------------------------------
void SoundPlayer::initialize()
{
    m_thread = std::make_unique<std::thread>(&SoundPlayer::run, this);
    //
    // Create a queue of 100 sf::Sound object that we'll cycle through.
    while (m_sounds.size() < 100)
    {
        m_sounds.enqueue(std::make_shared<sf::Sound>());
    }
}

// --------------------------------------------------------------
//
// Call this one time as the program is shutting down.  This gets
// the worker thread gracefully terminated.
//
// --------------------------------------------------------------
void SoundPlayer::terminate()
{
    m_done = true;
    m_eventTasks.notify_one();
    m_thread->join();
}

// --------------------------------------------------------------
//
// Public method to allow client code to initiate a sound.
//
// --------------------------------------------------------------
void SoundPlayer::play(const std::string& key, float volume)
{
    instance().m_tasks.enqueue({ key, volume });
    instance().m_eventTasks.notify_one();
}

// --------------------------------------------------------------
//
// This is the worker thread.  It pulls tasks from the queue and
// completes them.  If there are no tasks, it goes into an efficient
// wait state until a new task is added.
//
// --------------------------------------------------------------
void SoundPlayer::run()
{
    while (!m_done)
    {
        auto task = m_tasks.dequeue();
        if (task.has_value())
        {
            auto sound = m_sounds.dequeue().value();
            sound->setBuffer(*Content::get<sf::SoundBuffer>(task.value().key));
            sound->setVolume(task.value().volume);
            sound->play();
            m_sounds.enqueue(sound);
        }
        else
        {
            std::unique_lock<std::mutex> lock(m_mutexTasks);
            m_eventTasks.wait(lock);
        }
    }
}