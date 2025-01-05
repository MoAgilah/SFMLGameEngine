#include "FontManager.h"

FontManager::FontManager()
{
	m_standardFont.loadFromFile("../SFMLGameEngine/Resources/Fonts/arial.ttf");
}

void FontManager::AddFont(std::string path)
{
	sf::Font font;
	font.loadFromFile(path);
	m_fonts.push_back(font);
}
