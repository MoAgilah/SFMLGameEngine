#include "IText.h"

NTextConfig::NTextConfig()
	: m_fontName("Standard"), m_charSize(0), m_position(Point()), m_colour(Colour::Black), m_animType(NTextAnimType::Static), m_alignment(NTextAlignment::None)
{}

NTextConfig::NTextConfig(const std::string fontName, unsigned int charSize, const Point& position, NTextAnimType textAnimType, Colour colour, NTextAlignment alignment)
	: m_fontName(fontName), m_charSize(charSize), m_position(position), m_colour(colour), m_animType(textAnimType), m_alignment(alignment)
{}

NTextConfig::NTextConfig(const NTextConfig& config)
	: m_fontName(config.m_fontName), m_charSize(config.m_charSize), m_position(config.m_position), m_colour(config.m_colour), m_animType(config.m_animType), m_alignment(config.m_alignment)
{}

Point NCalculateTextOrigin(const Point& boundsSize, const Point& boundsPos)
{
	return { boundsPos.x + boundsSize.x / 2.f, boundsPos.x + boundsSize.y / 2.f };
}

Point NSetTextPosition(NTextAlignment alignment, const Point& textPos, const Point& boundsSize, const Point& boundsPos)
{
	switch (alignment)
	{
	case NTextAlignment::LeftHand:
		return { textPos.x, textPos.y - boundsSize.y / 2.f };
	case NTextAlignment::Center:
		return { textPos.x - boundsSize.x / 2.f - boundsPos.x,
			textPos.y - boundsSize.y / 2.f };
	case NTextAlignment::RightHand:
		return { textPos.x - boundsSize.x - boundsPos.x,
			textPos.y - boundsSize.y / 2.f };
		break;
	default:
		NCalculateTextOrigin(boundsSize, boundsPos);
		return textPos;
	}
}

IText::IText(const NTextConfig& config)
	: m_config(config)
{
}