#include "SoundManager.h"
#include "Constants.h"

SoundManager::SoundManager()
	: m_soundLoader(GameConstants::SoundFilePaths),
	m_musicLoader(GameConstants::MusicFilePaths)
{
}

sf::SoundBuffer* SoundManager::GetSound(const std::string& name)
{
	return m_soundLoader.GetResource(name);
}

sf::Music* SoundManager::GetMusic(const std::string& name)
{
	return m_musicLoader.GetResource(name);
}

void SoundManager::AddSounds(fs::path path)
{
	m_soundLoader.LoadResources(path);
}

void SoundManager::AddMusic(fs::path path)
{
	m_musicLoader.LoadResources(path);
}
