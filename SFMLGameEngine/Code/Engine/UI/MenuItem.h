#pragma once

#include "../../Platform/SFML/Drawables/SFText.h"
#include "../../Platform/SFML/Drawables/SFSprite.h"
#include "../../Utilities/Point.h"
#include <memory>

class MenuItem
{
public:
	MenuItem(const Point& menuSize, float outlineThickness);
	~MenuItem() = default;

	void Update(float deltaTime);
	void Render(IRenderer* renderer);

	int GetMenuSlotNumber() const { return m_menuSlotNumber; }
	void SetMenuSlotNumber(int slotNumber) { m_menuSlotNumber = slotNumber; }

	Point GetPosition() const { return m_cellSpace.getPosition(); }
	void SetPosition(const Point& position) { m_cellSpace.setPosition(position); }

	Point GetOrigin() const { return m_cellSpace.getOrigin(); }
	Point GetSize() const { return m_cellSpace.getSize(); }

	SFText* AddTextElement(SFText* text);
	SFText* GetTextElement();

	SFSprite* AddSpriteElement(SFSprite* spr);
	SFSprite* GetSpriteElement();

private:

	int m_menuSlotNumber;
	sf::RectangleShape m_cellSpace;
	std::shared_ptr<SFText> m_textElement;
	std::shared_ptr<SFSprite> m_spriteElement;
};