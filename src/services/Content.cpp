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

#include "services/Content.hpp"

#include <filesystem>
#include <iostream>

static const std::string CONTENT_PATH{ "assets" };
static const std::string CONTENT_FONT_PATH{ "fonts" };
static const std::string CONTENT_IMAGE_PATH{ "images" };
static const std::string CONTENT_AUDIO_PATH{ "audio" };
static const std::string CONTENT_MUSIC_PATH{ "music" };

// --------------------------------------------------------------
//
// Specialization on sf::Font for loading a font
//
// --------------------------------------------------------------
template <>
void Content::load<sf::Font>(std::string key, std::string filename, std::function<void(std::string)> onComplete, std::function<void(std::string)> onError)
{
    instance().m_tasks.enqueue({ Task::Type::Font, key, filename, onComplete, onError });
    instance().m_eventTasks.notify_one();
}

// --------------------------------------------------------------
//
// Specialization on sf::Texture for loading textures (for adding to sprites for rendering)
//
// --------------------------------------------------------------
template <>
void Content::load<sf::Texture>(std::string key, std::string filename, std::function<void(std::string)> onComplete, std::function<void(std::string)> onError)
{
    instance().m_tasks.enqueue({ Task::Type::Texture, key, filename, onComplete, onError });
    instance().m_eventTasks.notify_one();
}

// --------------------------------------------------------------
//
// Specialization on sf::SoundBuffer for loading an audio clip
//
// --------------------------------------------------------------
template <>
void Content::load<sf::SoundBuffer>(std::string key, std::string filename, std::function<void(std::string)> onComplete, std::function<void(std::string)> onError)
{
    instance().m_tasks.enqueue({ Task::Type::Audio, key, filename, onComplete, onError });
    instance().m_eventTasks.notify_one();
}

// --------------------------------------------------------------
//
// Specialization on sf::Music for getting a music file ready for streaming
//
// --------------------------------------------------------------
template <>
void Content::load<sf::Music>(std::string key, std::string filename, std::function<void(std::string)> onComplete, std::function<void(std::string)> onError)
{
    instance().m_tasks.enqueue({ Task::Type::Music, key, filename, onComplete, onError });
    instance().m_eventTasks.notify_one();
}

// --------------------------------------------------------------
//
// Specialization on sf::Font for obtaining a font
//
// --------------------------------------------------------------
template <>
std::shared_ptr<sf::Font> Content::get(std::string key)
{
    return instance().m_fonts[key];
}

template <>
bool Content::has<sf::Font>(std::string key)
{
    return instance().m_fonts.find(key) != instance().m_fonts.end();
}

// --------------------------------------------------------------
//
// Specialization on sf::Image for obtaining an image
//
// --------------------------------------------------------------
template <>
std::shared_ptr<sf::Texture> Content::get(std::string key)
{
    return instance().m_textures[key];
}

template <>
bool Content::has<sf::Texture>(std::string key)
{
    return instance().m_textures.find(key) != instance().m_textures.end();
}

// --------------------------------------------------------------
//
// Specialization on sf::SoundBuffer for obtaining an audio clip
//
// --------------------------------------------------------------
template <>
std::shared_ptr<sf::SoundBuffer> Content::get(std::string key)
{
    return instance().m_audio[key];
}

template <>
std::shared_ptr<sf::Sound> Content::get(std::string key)
{
    return instance().m_sound[key];
}

// Only need has<sf::SoundBuffer>, because it is good for both sf::Sound and sf::SoundBuffer
template <>
bool Content::has<sf::SoundBuffer>(std::string key)
{
    return instance().m_audio.find(key) != instance().m_audio.end();
}

// --------------------------------------------------------------
//
// Specialization on sf::Music for obtaining a Music streaming object
//
// --------------------------------------------------------------
template <>
std::shared_ptr<sf::Music> Content::get(std::string key)
{
    return instance().m_music[key];
}

template <>
bool Content::has<sf::Music>(std::string key)
{
    return instance().m_music.find(key) != instance().m_music.end();
}

// --------------------------------------------------------------
//
// Call this one time at program startup.  This gets the worker
// thread up and running.
//
// --------------------------------------------------------------
void Content::initialize()
{
    m_thread = std::make_unique<std::thread>(&Content::run, this);
}

// --------------------------------------------------------------
//
// Call this one time as the program is shutting down.  This gets
// the worker thread gracefully terminated.
//
// --------------------------------------------------------------
void Content::terminate()
{
    m_done = true;
    m_eventTasks.notify_one();
    m_thread->join();

    m_fonts.clear();
    m_textures.clear();
    m_audio.clear();
    m_music.clear();
    m_sound.clear();
}

// --------------------------------------------------------------
//
// Specialization on sf::Font for loading a font
//
// --------------------------------------------------------------
template <>
bool Content::loadImpl<sf::Font>(Task& task)
{
    std::filesystem::path path(CONTENT_PATH);
    path /= CONTENT_FONT_PATH;
    path /= task.filename;

    auto font = std::make_shared<sf::Font>();
    if (!font->loadFromFile(path.string()))
    {
        return false;
    }

    instance().m_fonts[task.key] = font;

    return true;
}

// --------------------------------------------------------------
//
// Specialization on sf::Texture for loading textures (for adding to sprites for rendering)
//
// --------------------------------------------------------------
template <>
bool Content::loadImpl<sf::Texture>(Task& task)
{
    std::filesystem::path path(CONTENT_PATH);
    path /= CONTENT_IMAGE_PATH;
    path /= task.filename;

    auto image = std::make_shared<sf::Texture>();
    if (!image->loadFromFile(path.string()))
    {
        return false;
    }

    instance().m_textures[task.key] = image;

    return true;
}

// --------------------------------------------------------------
//
// Specialization on sf::SoundBuffer for loading an audio clip
//
// --------------------------------------------------------------
template <>
bool Content::loadImpl<sf::SoundBuffer>(Task& task)
{
    std::filesystem::path path(CONTENT_PATH);
    path /= CONTENT_AUDIO_PATH;
    path /= task.filename;

    auto audio = std::make_shared<sf::SoundBuffer>();
    if (!audio->loadFromFile(path.string()))
    {
        return false;
    }

    instance().m_audio[task.key] = audio;
    // Create the matching sf::Sound that can be used to directly play the sound if desired
    instance().m_sound[task.key] = std::make_shared<sf::Sound>();
    instance().m_sound[task.key]->setBuffer(*audio);

    return true;
}

// --------------------------------------------------------------
//
// Specialization on sf::Music for creating a music object
//
// --------------------------------------------------------------
template <>
bool Content::loadImpl<sf::Music>(Task& task)
{
    std::filesystem::path path(CONTENT_PATH);
    path /= CONTENT_MUSIC_PATH;
    path /= task.filename;

    auto audio = std::make_shared<sf::Music>();
    if (!audio->openFromFile(path.string()))
    {
        return false;
    }

    instance().m_music[task.key] = audio;

    return true;
}

// --------------------------------------------------------------
//
// This is the worker thread.  It pulls tasks from the queue and
// completes them.  If there are no tasks, it goes into an efficient
// wait state until a new task is added.
//
// Note: This method must come AFTER the template specializations for
//       the loadImpl method.  If it doesn't, the compiler tries to
//       create its own instantiations and then when it sees the specializations
//       it has a little fit.
//
// --------------------------------------------------------------
void Content::run()
{
    while (!m_done)
    {
        m_processing = true;
        auto task = m_tasks.dequeue();
        if (task.has_value())
        {
            bool success{ false };
            switch (task->type)
            {
                case Task::Type::Font:
                    success = loadImpl<sf::Font>(*task);
                    break;
                case Task::Type::Texture:
                    success = loadImpl<sf::Texture>(*task);
                    break;
                case Task::Type::Audio:
                    success = loadImpl<sf::SoundBuffer>(*task);
                    break;
                case Task::Type::Music:
                    success = loadImpl<sf::Music>(*task);
                    break;
            }
            if (success)
            {
                std::cout << "finished loading: " << task->key << std::endl;
            }
            else
            {
                m_contentError = true;
                std::cout << "error in loading: " << task->filename << std::endl;
            }

            if (success && task->onComplete != nullptr)
            {
                task->onComplete(task->key);
            }
            else if (!success && task->onError != nullptr)
            {
                task->onError(task->filename);
            }
        }
        else
        {
            m_processing = false;
            std::unique_lock<std::mutex> lock(m_mutexTasks);
            m_eventTasks.wait(lock);
        }
    }
}