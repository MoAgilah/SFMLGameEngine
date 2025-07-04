#pragma once

#include "SFDrawables.h"
#include "../../../Engine/Core/Timer.h"
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
	NTextConfig()
		: m_fontName("Standard"), m_charSize(0), m_position(Point()), m_colour(sf::Color::Black), m_animType(NTextAnimType::Static), m_alignment(NTextAlignment::None)
	{
	}

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

	virtual void SetText(const std::string& text);
	virtual void Reset(const std::string& text, std::optional<NTextConfig> config = std::nullopt);

	Point GetSize() override;

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

using NUpdateFunc = std::function<void(float)>;
using NRenderFunc = std::function<void(IRenderer* renderer)>;

class SFAnimatedText : public SFText
{
public:
	SFAnimatedText(const NTextConfig& config);

	void Update(float deltaTime) override;
	void Render(IRenderer* renderer) override;

	void Reset(const std::string& text, std::optional<NTextConfig> config = std::nullopt) override;

	bool GetIsLooping() { return m_looping; }
	void SetIsLooping(bool loop) { m_looping = loop; }

	bool GetIsPaused() { return m_paused; }
	void SetIsPaused(bool pause) { m_paused = pause; }

	void SetMaxCount(int startFrom);
	bool CountHasEnded() const { return m_count == 0; }

	void SetCountDown(const std::string& msg) { m_countdownMsg = msg; }

	void SetUpdateFunc(NUpdateFunc func);
	void SetRenderFunc(NRenderFunc func);

	Timer& GetTimer() { return m_timer; }

private:

	void FadeInAndOutUpdate(float deltaTime);
	void FadeInFadeOutRender(IRenderer* renderer);

private:

	Timer m_timer;
	bool m_paused = false;
	bool m_looping = true;
	bool m_reduceAlpha = true;
	int m_count = 0;
	int m_maxCount = 0;
	NUpdateFunc m_updateFunc;
	NRenderFunc m_renderFunc;
	std::string m_countdownMsg;
	std::shared_ptr<sf::Shader> m_textShader;
};

void InitFlashingText(SFAnimatedText* txtObj, const std::string& text, bool loop = true, std::optional<NTextConfig> config = std::nullopt);
void InitCountdownText(SFAnimatedText* txtObj, int startFrom, const std::string& countDownMessage, std::optional<NTextConfig> config = std::nullopt);
void InitCustomTextAnim(SFAnimatedText* txtObj, const std::string& text, NUpdateFunc updator, NRenderFunc rendaror, const std::string& shaderName = "", std::optional<NTextConfig> config = std::nullopt);