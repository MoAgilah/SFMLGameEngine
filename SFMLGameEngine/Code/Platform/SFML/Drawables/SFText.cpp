#include "SFText.h"

Point NCalculateTextOrigin(const sf::FloatRect& bounds)
{
	return { bounds.position.x + bounds.size.x / 2.f, bounds.position.x + bounds.size.y / 2.f };
}

Point NSetTextPosition(NTextAlignment alignment, const Point& pos, const sf::FloatRect& bounds)
{
	switch (alignment)
	{
	case NTextAlignment::LeftHand:
		return { pos.x, pos.y - bounds.size.y / 2.f };
	case NTextAlignment::Center:
		return { pos.x - bounds.size.x / 2.f - bounds.position.x,
			pos.y - bounds.size.y / 2.f };
	case NTextAlignment::RightHand:
		return { pos.x - bounds.size.x - bounds.position.x,
			pos.y - bounds.size.y / 2.f };
		break;
	default:
		NCalculateTextOrigin(bounds);
		return pos;
	}
}

SFText::SFText(const NTextConfig& config)
	: m_config(config)
{
	m_drawable->setCharacterSize(m_config.m_charSize);
	m_drawable->setOutlineThickness(m_config.m_charSize / 10.f);
	m_drawable->setOutlineColor(m_config.m_colour);
}

void SFText::Update(float dt)
{
	// does nothing
}

void SFText::SetText(const std::string& text)
{
	m_drawable->setString(text);
	m_drawable->setPosition(NSetTextPosition(m_config.m_alignment, m_config.m_position, m_drawable->getLocalBounds()));
}

void SFText::Reset(const std::string& text, std::optional<NTextConfig> config)
{
	if (config)
	{
		m_config = *config;
		Init();
	}

	SetText(text);
}

void SFText::Init()
{
	m_drawable->setCharacterSize(m_config.m_charSize);
	m_drawable->setOutlineThickness(m_config.m_charSize / 10.f);
	m_drawable->setOutlineColor(m_config.m_colour);
}