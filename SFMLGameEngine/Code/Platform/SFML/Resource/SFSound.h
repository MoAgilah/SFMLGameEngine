#pragma once

#include "../../../Engine/Interfaces/ISound.h"
#include "../../../Utilities/Traits.h"
#include <SFML/Audio.hpp>

class SFSound : public ISound
{
public:
    SFSound()
        : m_buffer()              // create buffer first
        , m_sound(m_buffer)       // construct sound with the buffer
    {}
    virtual ~SFSound() = default;

    // Load a music stream from a file
    bool LoadFromFile(const std::string& filepath) override
    {
        if (!m_buffer.loadFromFile(filepath))
            return false;

        m_sound.setBuffer(m_buffer);
        return true;
    }

    // Playback control
    void Play() override
    {
        m_sound.play();
    }

    void Pause() override
    {
        m_sound.pause();
    }

    void Stop() override
    {
        m_sound.stop();
    }

    // Looping
    void SetLoop(bool loop) override
    {
        m_sound.setLooping(loop);
    }

    sf::Sound& GetNativeSound() { return m_sound; }

private:

    sf::SoundBuffer m_buffer;
    sf::Sound m_sound;
};

template<>
struct SoundTrait<SFSound>
{
    using SoundType = SFSound;

    static std::unique_ptr<ISound> Create()
    {
        return std::make_unique<SFSound>();
    }
};
