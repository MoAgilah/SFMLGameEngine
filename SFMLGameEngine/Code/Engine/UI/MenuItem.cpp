#include "MenuItem.h"

MenuItem::MenuItem(const Point& menuSize, float outlineThickness)
	: m_menuSlotNumber(-1), m_cellSpace(menuSize), m_textElement(nullptr), m_spriteElement(nullptr)
{
	m_cellSpace.setOrigin(Point(m_cellSpace.getSize()) / 2.f);
	m_cellSpace.setOutlineThickness(outlineThickness);
	m_cellSpace.setOutlineColor(sf::Color::Green);
}

void MenuItem::Update(float deltaTime)
{
	if (m_textElement)
		m_textElement->Update(deltaTime);

	if (m_spriteElement)
		m_spriteElement->Update(deltaTime);
}

void MenuItem::Render(sf::RenderWindow& window)
{
#if defined _DEBUG
	DebugRender(window);
#endif

	if (m_textElement)
		m_textElement->Render(window);

	if (m_spriteElement)
		m_spriteElement->Render(window);
}

Text* MenuItem::AddTextElement(Text* text)
{
	m_textElement = std::shared_ptr<Text>(text);
	return GetTextElement();
}

Text* MenuItem::GetTextElement()
{
	if (m_textElement)
		return m_textElement.get();

	return nullptr;
}

Sprite* MenuItem::AddSpriteElement(Sprite* spr)
{
	m_spriteElement = std::shared_ptr<Sprite>(spr);
	return GetSpriteElement();
}

Sprite* MenuItem::GetSpriteElement()
{
	if (m_spriteElement)
		return m_spriteElement.get();

	return nullptr;
}

void MenuItem::DebugRender(sf::RenderWindow& window)
{
	window.draw(m_cellSpace);
}
