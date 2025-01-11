#pragma once

#include "../Utilities/ResourceLoader.h"

class SoundManager
{
public:
	SoundManager();
	~SoundManager() = default;

	sf::SoundBuffer* GetSound(const std::string& name);
	sf::Music* GetMusic(const std::string& name);

private:

	ResourceLoader<sf::SoundBuffer> m_soundLoader;
	ResourceLoader<sf::Music> m_musicLoader;
};