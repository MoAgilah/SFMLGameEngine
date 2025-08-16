#pragma once

#include "../../../Engine/Interfaces/IMusic.h"
#include "../../../Utilities/Traits.h"
#include <SFML/Audio.hpp>

class SFMusic : public IMusic
{
public:
    virtual ~SFMusic() = default;

    // Load a music stream from a file
    bool LoadFromFile(const std::string& filepath) override
    {
        return m_music.openFromFile(filepath);
    }

    // Playback control
    void Play() override
    {
        m_music.play();
    }

    void Pause() override
    {
        m_music.pause();
    }

    void Stop() override
    {
        m_music.stop();
    }

    // Looping
    void SetLoop(bool loop) override
    {
        m_music.setLooping(loop);
    }

    sf::Music& GetNativeMusic() { return m_music; }

private:

    sf::Music m_music;
};

template<>
struct MusicTrait<SFMusic>
{
    using MusicType = SFMusic;

    static std::unique_ptr<IMusic> Create()
    {
        return std::make_unique<SFMusic>();
    }
};
