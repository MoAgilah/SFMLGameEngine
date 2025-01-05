#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class FontManager
{
public:
	FontManager();
	~FontManager() = default;

	void AddFont(std::string path);


	const sf::Font& GetStandardFont() { return m_standardFont; }
	const sf::Font& GetMenuFont() { return m_standardFont; }

private:

	sf::Font m_standardFont;
	std::vector<sf::Font> m_fonts;
};