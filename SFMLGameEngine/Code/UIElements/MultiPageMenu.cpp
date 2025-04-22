#include "MultiPageMenu.h"

MultiPageMenu::MultiPageMenu(const std::string& ID, Menu* menu)
{
	AddMenu(ID, menu);
	TransitionMenu(ID);
}

void MultiPageMenu::ProcessInput()
{
	if (m_currentMenu)
		m_currentMenu->ProcessInput();
}

void MultiPageMenu::Update(float deltaTime)
{
	if (m_currentMenu)
		m_currentMenu->Update(deltaTime);
}

void MultiPageMenu::Render(sf::RenderWindow& window)
{
	if (m_currentMenu)
		m_currentMenu->Render(window);
}

void MultiPageMenu::AddMenu(const std::string& ID, Menu* menu)
{
	m_menus.emplace(ID, menu);
}

Menu* MultiPageMenu::GetMenu(const std::string& ID)
{
	auto it = m_menus.find(ID);
	if (it != m_menus.end())
		return (*it).second.get();

	return nullptr;
}

void MultiPageMenu::TransitionMenu(const std::string& ID)
{
	if (m_currentMenu)
		m_currentMenu->Pause();

	m_currentMenu = GetMenu(ID);
}