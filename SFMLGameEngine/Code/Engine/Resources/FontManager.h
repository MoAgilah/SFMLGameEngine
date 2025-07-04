#pragma once

#include "../../Utilities/ResourceLoader.h"

class FontManager
{
public:
	FontManager();
	~FontManager() = default;

	sf::Font* GetFont(const std::string& name);
	void AddFonts(const fs::path& path);

private:

	ResourceLoader<sf::Font> m_loader;
};