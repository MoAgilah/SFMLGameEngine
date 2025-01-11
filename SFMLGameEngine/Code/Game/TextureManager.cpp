#include "TextureManager.h"
#include "Game/Constants.h"

TextureManager::TextureManager()
	: m_loader(GameConstants::TextureFilePaths)
{
}

sf::Texture* TextureManager::GetTexture(const std::string& name)
{
	return m_loader.GetResource(name);
}

void TextureManager::AddTextures(fs::path path)
{
	m_loader.LoadResources(path);
}
