#pragma once

#include "Text.h"
#include "../../Game/Sprite.h"
#include "../../Utilities/Point.h"
#include <SFML/Graphics.hpp>
#include <memory>

class MenuItem
{
public:
	MenuItem(const Point& menuSize, float outlineThickness);
	~MenuItem() = default;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	int GetMenuSlotNumber() const { return m_menuSlotNumber; }
	void SetMenuSlotNumber(int slotNumber) { m_menuSlotNumber = slotNumber; }

	Point GetPosition() const { return m_cellSpace.getPosition(); }
	void SetPosition(const Point& position) { m_cellSpace.setPosition(position); }

	Point GetOrigin() const { return m_cellSpace.getOrigin(); }
	Point GetSize() const { return m_cellSpace.getSize(); }

	Text* AddTextElement(Text* text);
	Text* GetTextElement();

	Sprite* AddSpriteElement(Sprite* spr);
	Sprite* GetSpriteElement();

private:

	void DebugRender(sf::RenderWindow& window);

	int m_menuSlotNumber;
	sf::RectangleShape m_cellSpace;
	std::shared_ptr<Text> m_textElement;
	std::shared_ptr<Sprite> m_spriteElement;
};