#pragma once

#include "../../../Engine/Interfaces/IMenu.h"
#include "../Drawables/SFSprite.h"

class SFMenu : public IMenu
{
public:
	SFMenu(const Point& menuSize, float outlineThickness, const Point& dimensions, const NMenuPositionData& menuPositionData);
	~SFMenu() = default;

	void AddCursor(ISprite* spr, const MenuNav& menuNav);

protected:

	void BuildMenuSpace() override;
	void BuildColumns() override;
	void BuildRows() override;

	void SetActiveTextElement() override;
};