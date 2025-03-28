#pragma once

#include "Text.h"

class Menu
{
public:
	Menu(void (*func)(int), TextType textType, const std::string text, unsigned int charSize, unsigned int marginSize, const sf::Vector2f& pos, sf::Color color = sf::Color::Black);
	~Menu() = default;

	void AddMenuItem(const std::string text, bool hasPassiveColor = false, sf::Color color = sf::Color::Black);

	void ProcessInput();
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

private:

	bool m_menuMoved = false;
	bool m_canGoDown = true;
	bool m_canGoUp = true;
	int m_menuPosition = 0;
	int m_maxMenuPosition = 0;
	unsigned int m_charSize;
	unsigned int m_marginSize;
	void (*m_actionFunc)(int);
	sf::Vector2f m_position;
	TextType m_textType;
	sf::Color m_ActiveColour;
	std::vector<Text> m_menuItems;
};