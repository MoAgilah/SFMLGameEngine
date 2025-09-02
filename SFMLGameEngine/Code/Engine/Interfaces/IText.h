#pragma once

#include "IRenderer.h"
#include "../../Utilities/Colour.h"
#include "../../Utilities/Point.h"
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
	NTextConfig();

	NTextConfig(const std::string fontName, unsigned int charSize, const Point& position, NTextAnimType textAnimType, Colour colour = Colour::Black, NTextAlignment alignment = NTextAlignment::Center);

	NTextConfig(const NTextConfig& config);

	std::string m_fontName;
	unsigned int m_charSize;
	Point m_position;
	Colour m_colour;
	NTextAnimType m_animType;
	NTextAlignment m_alignment;
};

Point NCalculateTextOrigin(const Point& boundsSize, const Point& boundsPos);

Point NSetTextPosition(NTextAlignment alignment, const Point& textPos, const Point& boundsSize, const Point& boundsPos);

class IText
{
public:
	IText(){}
	IText(const NTextConfig& config);
	virtual ~IText() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(IRenderer* renderer) = 0;

	virtual unsigned int GetCharSize() = 0;
	virtual void SetCharSize(unsigned int charSize) = 0;

	virtual Colour GetOutlineColour() = 0;
	virtual void SetOutlineColour(const Colour& colour) = 0;

	virtual Colour GetFillColour() = 0;
	virtual void SetFillColour(const Colour& colour) = 0;

	virtual float GetOutlineThickness() = 0;
	virtual void SetOutlineThickness(float thickness) = 0;

	bool IsAnimated() { return m_config.m_animType > NTextAnimType::Static; }

private:

	virtual void Init() = 0;

protected:

	NTextConfig m_config;
};