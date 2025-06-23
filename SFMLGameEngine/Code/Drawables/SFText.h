#pragma once

#include "SFDrawables.h"
#include <SFML/Graphics.hpp>
#include <string>

enum class NTextAnimType
{
	Unassigned, Static, Flashing, Countdown, Custom
};

enum class NTextAlignment
{
	None, LeftHand, Center, RightHand
};

struct NTextConfig
{
	NTextConfig(const std::string fontName, unsigned int charSize, const Point& position, NTextAnimType textAnimType, sf::Color colour = sf::Color::Black, NTextAlignment alignment = NTextAlignment::Center)
		: m_fontName(fontName), m_charSize(charSize), m_position(position), m_colour(colour), m_animType(textAnimType), m_alignment(alignment)
	{}

	NTextConfig(const NTextConfig& config)
		: m_fontName(config.m_fontName), m_charSize(config.m_charSize), m_position(config.m_position), m_colour(config.m_colour), m_animType(config.m_animType), m_alignment(config.m_alignment)
	{}

	std::string m_fontName;
	unsigned int m_charSize;
	Point m_position;
	sf::Color m_colour;
	NTextAnimType m_animType;
	NTextAlignment m_alignment;
};

Point NCalculateTextOrigin(const sf::FloatRect& bounds);

Point NSetTextPosition(NTextAlignment alignment, const Point& pos, const sf::FloatRect& bounds);

class SFText : public SFDrawables<sf::Text>
{
public:
	SFText(const NTextConfig& config);

	void Update(float deltaTime);

	virtual void SetText(const std::string& text);
	virtual void Reset(const std::string& text, std::optional<NTextConfig> config = std::nullopt);

	Point GetSize();

	unsigned int GetCharSize() { return m_drawable->getCharacterSize(); }
	void SetCharSize(unsigned int charSize) { m_drawable->setCharacterSize(charSize); }

	sf::Color GetOutlineColour() { return m_drawable->getOutlineColor(); }
	void SetOutlineColour(const sf::Color& colour) { m_drawable->setOutlineColor(colour); }

	sf::Color GetFillColour() { return m_drawable->getFillColor(); }
	void SetFillColour(const sf::Color& colour) { m_drawable->setFillColor(colour); }

	void ResetOutlineColour() { SetOutlineColour(m_config.m_colour); }

	bool IsAnimated() { return m_config.m_animType > NTextAnimType::Static; }

private:

	virtual void Init();

protected:

	NTextConfig m_config;
};