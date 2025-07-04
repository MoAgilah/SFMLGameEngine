#pragma once

#include "MenuNavigation.h"
#include "../../Platform/SFML/Drawables/SFSprite.h"
#include "../../Utilities/Point.h"
#include <string>
#include <vector>

class MenuCursor
{
public:
	MenuCursor(SFSprite* spr, const MenuNav& menuNav);
	~MenuCursor() = default;

	void Update(float deltaTime);
	void Render(IRenderer* renderer);

	void SetPosition(const Point& pos) { m_cursor->SetPosition(pos); };
	void SetScale(const Point& cellSize);

	MenuNav* GetMenuNav() { return &m_menuNav; }
	SFSprite* GetSprite() { return m_cursor.get(); }

private:

	MenuNav m_menuNav;
	std::shared_ptr<SFSprite> m_cursor;
};