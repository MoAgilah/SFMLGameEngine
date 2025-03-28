#pragma once

#include "Text.h"
#include <functional>

class Menu
{
public:
	Menu(std::function<void(int)> func, std::string m_fontName, TextType textType, const std::string text, unsigned int charSize, unsigned int marginSize, const sf::Vector2f& pos, sf::Color color = sf::Color::Black);
	~Menu() = default;

	void AddMenuItem(const std::string text);

	void ProcessInput();
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void SetPassiveColour(sf::Color color);
private:

	Text CreateMenuItem(const std::string& text, sf::Color color);
	void HandleNavigation();
	void HandleDirection(bool isPressed, bool& canMove, int direction);
	void ClampMenuPosition();

	bool m_initial = true;
	bool m_menuMoved = false;
	bool m_canGoDown = true;
	bool m_canGoUp = true;
	int m_menuPosition = 0;
	unsigned int m_charSize;
	unsigned int m_marginSize;
	std::function<void(int)> m_actionFunc;
	sf::Vector2f m_position;
	std::string m_fontName;
	TextType m_textType;
	sf::Color m_activeColour;
	bool m_hasPassiveColor = false;
	sf::Color m_passiveColour;
	std::vector<Text> m_menuItems;
};
