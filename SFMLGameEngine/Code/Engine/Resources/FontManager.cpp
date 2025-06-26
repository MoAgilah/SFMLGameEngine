#include "FontManager.h"

#include "../Core/Constants.h"

FontManager::FontManager()
	: m_loader(GameConstants::FontFilePaths)
{}

sf::Font* FontManager::GetFont(const std::string& name)
{
	return m_loader.GetResource(name);
}

void FontManager::AddFonts(const fs::path& path)
{
	m_loader.LoadResources(path);
}