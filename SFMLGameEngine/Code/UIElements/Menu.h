#pragma once

#include "Text.h"
#include <functional>
#include <map>

class Menu
{
public:
	Menu(std::function<void(int)> func, std::string fontName, TextAnimType textType, const std::string text, unsigned int charSize, unsigned int marginSize, const Point& pos, sf::Color color = sf::Color::Black);
	~Menu() = default;

	void AddMenuItem(const std::string text);

	void ProcessInput();
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void SetPassiveColour(sf::Color color);

private:

	Text* CreateMenuItem(const std::string& text, sf::Color color);
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
	Point m_position;
	std::string m_fontName;
	TextAnimType m_textType;
	sf::Color m_activeColour;
	bool m_hasPassiveColor = false;
	sf::Color m_passiveColour;
	std::vector<std::unique_ptr<Text>> m_menuItems;
};
