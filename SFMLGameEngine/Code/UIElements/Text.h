#pragma once

#include "../Game/Timer.h"
#include "../Utilities/Point.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>
#include <string>

enum TextAlignment
{
	NNone, NLeftHand, NCenter, NRightHand
};

struct TextConfig
{
	unsigned int m_charSize;
	Point m_position;
	sf::Color m_colour;
	TextAlignment m_alignment;

	TextConfig()
		: m_charSize(0), m_position(Point()), m_colour(sf::Color::Black), m_alignment(NNone)
	{
	}

	TextConfig(unsigned int charSize, const Point& position, sf::Color colour = sf::Color::Black, TextAlignment alignment = TextAlignment::NNone)
		: m_charSize(charSize), m_position(position), m_colour(colour), m_alignment(alignment)
	{}

	TextConfig(const TextConfig& config)
		: m_charSize(config.m_charSize), m_position(config.m_position), m_colour(config.m_colour), m_alignment(config.m_alignment)
	{}
};

enum TextAnimType
{
	Static, Flashing, Countdown, Custom
};

class Text
{
public:
	Text(const std::string& fontName, TextConfig config);
	~Text() = default;

	virtual void Update(float deltaTime);
	virtual void Render(sf::RenderWindow& window);

	virtual void Reset(const std::string& text);

	virtual void SetText(const std::string& text, std::optional<TextConfig> config = std::nullopt);

	void SetPosition(const Point& pos);
	Point GetPosition();

	void SetCharSize(unsigned int charSize);
	void SetTextAlignment(TextAlignment alignment);
	void SetOutlineColour(const sf::Color& colour);
	void SetFillColour(const sf::Color& colour);

protected:

	TextAnimType m_animType = Static;
	TextConfig m_config;
	sf::Text m_text;

private:

	void SetTextPosition(const Point& pos);
};

using UpdateFunc = std::function<void(float)>;
using RenderFunc = std::function<void(sf::RenderWindow&)>;

class AnimatedText : public Text
{
public:
	AnimatedText(const std::string& fontName, TextConfig config, TextAnimType animType);
	~AnimatedText() = default;

	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

	void Reset(const std::string& text) override;

	void InitFlashingText(const std::string& text, bool loop = true, std::optional<TextConfig> config = std::nullopt);
	void InitCountdownText(int startFrom, const std::string& countDownMessage, std::optional<TextConfig> config = std::nullopt);
	void InitCustomTextAnim(const std::string& text, UpdateFunc updator, RenderFunc rendaror, const std::string& shaderName = "", std::optional<TextConfig> config = std::nullopt);

	void Pause() { m_paused = true; }
	void Resume() { m_paused = false; }
	bool IsPaused() { return m_paused; }

	bool CountHasEnded() const { return m_count == 0; }

private:

	void FadeInAndOutUpdate(float deltaTime);
	void FadeInFadeOutRender(sf::RenderWindow& window);

	UpdateFunc m_customUpdate;
	RenderFunc m_customRender;

	bool m_loop = true;
	bool m_paused = false;
	bool m_reduceAlpha = true;
	float m_time = 0.f;
	float m_maxTime = 1.0f;
	std::string m_countdownMsg;
	int m_count = 0;
	int m_maxCount = 0;
	Timer m_timer;
	std::shared_ptr<sf::Shader> m_textShader;
};