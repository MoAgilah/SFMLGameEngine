#pragma once

#include <map>
#include <SFML/Audio.hpp>
#include <string>

enum class SoundID
{
	None = -1,
};

class SoundManager
{
public:
	SoundManager() = default;
	~SoundManager() = default;

	sf::SoundBuffer* GetSound(SoundID id);
	void AddSound(SoundID id);

	sf::Music* GetMusic() { return&m_music; }
	bool SetMusic(std::string music);

private:

	bool m_musicSet = false;
	sf::Music m_music;
	std::map<SoundID, sf::SoundBuffer> m_soundMap;
};