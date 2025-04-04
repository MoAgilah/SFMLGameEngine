#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "../Game/Timer.h"
#include "Utilities/Point.h"

enum TextAlignment
{
	None, LeftHand, Center, RightHand
};

enum TextType
{
	Static, Dynamic
};

class Text
{
public:
	Text(const std::string& fontName = "Standard", float fadeTime = 0.75f);
	~Text() = default;

	void InitCountdown(const std::string& text, int startFrom, unsigned int charSize, const Point& pos, sf::Color color = sf::Color::Black, TextAlignment alignment = Center);
	void InitStaticText(const std::string& text, unsigned int charSize, const Point& pos, sf::Color color = sf::Color::Black, TextAlignment alignment = None);
	void InitFlashingText(const std::string& text, unsigned int charSize, const Point& pos, sf::Color color = sf::Color::Black, TextAlignment alignment = None, bool paused = false);

	void Reset(const std::string& text);
	void RestartCountDown();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	bool IsPaused() { return m_paused; }
	bool CountHasEnded() { return m_count == 0; }

	void Pause();
	void Resume();

private:

	void Init(const std::string& text, unsigned int charSize, const Point& pos, sf::Color color, TextAlignment alignment, bool loop, bool paused = false);
	void SetTextPosition(const Point& pos);

	int m_count = 0;
	int m_startFrom = 0;
	bool m_countdown = false;
	std::string m_countdownMsg;
	bool m_loop = true;
	bool m_paused;
	bool m_reduceAlpha = true;
	float m_time = 1;
	float m_maxTime;
	TextAlignment m_alignment;
	Timer m_timer;
	Point m_position;
	sf::Text m_text;
	std::shared_ptr<sf::Shader> m_textShader;
};