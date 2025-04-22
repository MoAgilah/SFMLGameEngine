#pragma once

#include "Menu.h"
#include <map>
#include <memory>
#include <string>

class MultiPageMenu
{
public:
	MultiPageMenu(const std::string& ID, Menu* menu);
	~MultiPageMenu() = default;

	void ProcessInput();
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void AddMenu(const std::string& ID, Menu* menu);
	Menu* GetMenu(const std::string& ID);

	void TransitionMenu(const std::string& ID);

private:

	Menu* m_currentMenu = nullptr;
	std::map<std::string, std::unique_ptr<Menu>> m_menus;
};