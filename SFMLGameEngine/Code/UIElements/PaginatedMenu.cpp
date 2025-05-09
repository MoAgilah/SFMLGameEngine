#include "PaginatedMenu.h"

void PaginatedMenu::Update(float deltaTime)
{
	if (m_menuPages.empty())
		return;

	m_menuPages[m_currentMenuNum]->Update(deltaTime);
}

void PaginatedMenu::Render(sf::RenderWindow& window)
{
	if (m_menuPages.empty())
		return;

	m_menuPages[m_currentMenuNum]->Render(window);
}

void PaginatedMenu::SetCurrentMenuNumber(int menuNumer)
{
	if (menuNumer < m_menuPages.size())
		m_currentMenuNum = menuNumer;
}

Menu* PaginatedMenu::AddMenu(Menu* menu)
{
	m_menuPages.push_back(std::shared_ptr<Menu>(menu));

	return GetMenuByNumber((int)(m_menuPages.size() - 1));
}

Menu* PaginatedMenu::GetMenuByNumber(int menuNumber)
{
	if (m_menuPages.empty())
		return nullptr;

	if (menuNumber < m_menuPages.size())
		return m_menuPages[menuNumber].get();

	return nullptr;
}
