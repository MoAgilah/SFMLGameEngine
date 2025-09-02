#pragma once

#include "../../../Engine/Interfaces/IMenuItem.h"
#include "../Drawables/SFText.h"
#include "../Drawables/SFSprite.h"
#include "../../../Utilities/Point.h"

class SFMenuItem : public IMenuItem
{
public:
	SFMenuItem(const Point& menuSize, float outlineThickness);
	~SFMenuItem() = default;

	Point GetPosition() const override;
	void SetPosition(const Point& position) override;

	Point GetOrigin() const override;
	Point GetSize() const override;

	IText* AddTextElement(std::shared_ptr<IText>  text) override;
	ISprite* AddSpriteElement(std::shared_ptr<ISprite>  spr) override;
};