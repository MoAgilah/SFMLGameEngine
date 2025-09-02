#pragma once

#include "../../../Engine/Interfaces/IMenuCursor.h"
#include "../../../Engine/UI/MenuNavigation.h"
#include "../Drawables/SFSprite.h"


class SFMenuCursor : public IMenuCursor
{
public:
	SFMenuCursor(SFSprite* sprite, const MenuNav& menuNav);
	~SFMenuCursor() = default;

	void SetPosition(const Point& pos);
	void SetScale(const Point& cellSize);
};