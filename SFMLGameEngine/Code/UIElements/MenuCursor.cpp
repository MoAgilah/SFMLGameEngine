#include "MenuCursor.h"

MenuCursor::MenuCursor(Sprite* spr, const MenuNav& menuNav)
	: m_menuNav(menuNav)
{
	m_cursor = std::shared_ptr<Sprite>(spr);
}

void MenuCursor::Update(float deltaTime)
{
	m_menuNav.HandleNavigation();

	if (m_cursor)
		m_cursor->Update(deltaTime);
}

void MenuCursor::Render(sf::RenderWindow& window)
{
	if (m_cursor)
		m_cursor->Render(window);
}