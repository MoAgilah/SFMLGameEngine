#include "SFMenuItem.h"

#include "../Drawables/SFShape.h"

SFMenuItem::SFMenuItem(const Point& menuSize, float outlineThickness)
{
	m_cellSpace = std::make_shared<SFRect>(menuSize, Point());
	auto rect = dynamic_cast<SFRect*>(m_cellSpace.get());
	if (rect)
	{
		rect->SetOrigin(Point(rect->GetSize()) / 2.f);
		rect->SetOutlineThickness(outlineThickness);
		rect->SetOutlineColour(sf::Color::Green);
	}
}

Point SFMenuItem::GetPosition() const
{
	return static_cast<SFRect*>(m_cellSpace.get())->GetPosition();
}

void SFMenuItem::SetPosition(const Point& position)
{
	static_cast<SFRect*>(m_cellSpace.get())->SetPosition(position);
}

Point SFMenuItem::GetOrigin() const
{
	return static_cast<SFRect*>(m_cellSpace.get())->GetOrigin();
}

Point SFMenuItem::GetSize() const
{
	return static_cast<SFRect*>(m_cellSpace.get())->GetSize();
}

IText* SFMenuItem::AddTextElement(std::shared_ptr<IText> text)
{
	m_textElement = std::move(text);     // stores as shared_ptr<IText>
	return m_textElement.get();
}

ISprite* SFMenuItem::AddSpriteElement(std::shared_ptr<ISprite> spr)
{
	m_spriteElement = std::move(spr);
	return m_spriteElement.get();
}