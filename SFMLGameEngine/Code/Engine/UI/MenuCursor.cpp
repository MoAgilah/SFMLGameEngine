#include "MenuCursor.h"

MenuCursor::MenuCursor(SFSprite* spr, const MenuNav& menuNav)
	: m_menuNav(menuNav)
{
	m_cursor = std::shared_ptr<SFSprite>(spr);
}

void MenuCursor::Update(float deltaTime)
{
	m_menuNav.HandleNavigation();

	if (m_cursor)
		m_cursor->Update(deltaTime);
}

void MenuCursor::Render(IRenderer* renderer)
{
	if (m_cursor)
		m_cursor->Render(renderer);
}

void MenuCursor::SetScale(const Point& cellSize)
{
	// Compute scale factors
	Point scaleXY = cellSize / m_cursor->GetSize();

	// Choose the smaller scale to preserve aspect ratio
	float scale = std::min(scaleXY.x, scaleXY.y);

	m_cursor->SetScale({ scale,scale });
}
