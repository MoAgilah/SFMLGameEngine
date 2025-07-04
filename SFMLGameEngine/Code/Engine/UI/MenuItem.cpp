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

void MenuItem::Render(IRenderer* renderer)
{
#if defined _DEBUG
	auto window = static_cast<sf::RenderWindow*>(renderer->GetWindow()->GetNativeHandle());
	if (window)
		window->draw(m_cellSpace);
#endif

	if (m_textElement)
		m_textElement->Render(renderer);

	if (m_spriteElement)
		m_spriteElement->Render(renderer);
}

SFText* MenuItem::AddTextElement(SFText* text)
{
	m_textElement = std::shared_ptr<SFText>(text);
	return GetTextElement();
}

SFText* MenuItem::GetTextElement()
{
	if (m_textElement)
		return m_textElement.get();

	return nullptr;
}

SFSprite* MenuItem::AddSpriteElement(SFSprite* spr)
{
	m_spriteElement = std::shared_ptr<SFSprite>(spr);
	return GetSpriteElement();
}

SFSprite* MenuItem::GetSpriteElement()
{
	if (m_spriteElement)
		return m_spriteElement.get();

	return nullptr;
}