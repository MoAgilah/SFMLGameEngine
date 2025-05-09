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

void MenuCursor::SetScale(const Point& cellSize)
{
	// Compute scale factors
	Point scaleXY = cellSize / m_cursor->GetSize();

	// Choose the smaller scale to preserve aspect ratio
	float scale = std::min(scaleXY.x, scaleXY.y);

	m_cursor->SetScale({ scale,scale });
}
