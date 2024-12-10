#include "SoundManager.h"
#include <iostream>
#include <format>
#include <stdexcept>

std::string TexIDToString(SoundID id)
{
	switch (id)
	{
	default: return "";
	}
}

sf::SoundBuffer* SoundManager::GetSound(SoundID id)
{
	auto it = m_soundMap.find(id);
	if (it == m_soundMap.end())
	{
		throw std::invalid_argument(std::format("Texture {} not recognised", (int)id));
		return nullptr;
	}

	return &m_soundMap[id];
}

void SoundManager::AddSound(SoundID id)
{
	sf::SoundBuffer sound;

	if (!sound.loadFromFile(std::format("../Resources/Sounds/{}.wav", TexIDToString(id))))
	{
		std::cout << std::format("failed to load ../Resources/Sprites/{}.png", TexIDToString(id)) << std::endl;
	}

	m_soundMap.try_emplace(id, sound);
}

bool SoundManager::SetMusic(std::string music)
{
	if (m_musicSet)
	{
		if (m_music.getStatus() == sf::SoundSource::Status::Playing)
		{
			m_music.pause();
		}
	}

	if (!m_music.openFromFile(music))
	{
		std::cout << std::format("failed to load ../Resources/Sprites/{}", music) << std::endl;

		m_musicSet = false;

		return false;
	}

	m_musicSet = true;

	return true;
}
