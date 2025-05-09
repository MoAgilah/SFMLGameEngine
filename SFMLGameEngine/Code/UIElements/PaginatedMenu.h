#pragma once

#include "Menu.h"
#include <memory>
#include <vector>

class PaginatedMenu
{
public:
	PaginatedMenu() = default;
	~PaginatedMenu() = default;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void SetCurrentMenuNumber(int menuNumer);
	int GetCurrentMenuNumber() { return m_currentMenuNum; }

	Menu* AddMenu(Menu* menu);
	Menu* GetMenuByNumber(int menuNumer);
	Menu* GetCurrentMenu() { return GetMenuByNumber(m_currentMenuNum); }

private:

	unsigned int m_currentMenuNum = 0;
	unsigned int m_previousMenuNum = -1;
	std::vector<std::shared_ptr<Menu>> m_menuPages;
};