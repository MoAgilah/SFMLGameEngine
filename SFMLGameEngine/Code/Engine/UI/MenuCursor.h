#pragma once

#include "MenuNavigation.h"
#include "../../Game/Sprite.h"
#include "../../Utilities/Point.h"
#include <string>
#include <vector>

class MenuCursor
{
public:
	MenuCursor(Sprite* spr, const MenuNav& menuNav);
	~MenuCursor() = default;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void SetPosition(const Point& pos) { m_cursor->SetPosition(pos); };
	void SetScale(const Point& cellSize);

	MenuNav* GetMenuNav() { return &m_menuNav; }
	Sprite* GetSprite() { return m_cursor.get(); }

private:

	MenuNav m_menuNav;
	std::shared_ptr<Sprite> m_cursor;
};