#include "SFMenuCursor.h"

SFMenuCursor::SFMenuCursor(SFSprite* sprite, const MenuNav& menuNav)
	: IMenuCursor(std::move(sprite), menuNav)
{}

void SFMenuCursor::SetPosition(const Point& pos)
{
	static_cast<SFSprite*>(m_cursor.get())->SetPosition(pos);
}

void SFMenuCursor::SetScale(const Point& cellSize)
{
	auto spr = static_cast<SFSprite*>(m_cursor.get());

	Point scaleXY = cellSize / spr->GetSize();

	float scale = std::min(scaleXY.x, scaleXY.y);

	spr->SetScale({ scale, scale });
}
