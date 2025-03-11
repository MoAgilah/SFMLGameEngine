#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "../Game/Timer.h"

enum TextAlignment
{
	None, LeftHand, Center, RightHand
};

class FlashingText
{
public:
	FlashingText(const std::string fontName = "Standard", float fadeTime = 0.75f);
	~FlashingText() = default;

	void Init(const std::string text, unsigned int charSize, const sf::Vector2f pos, TextAlignment alignment = None, bool loop = true);
	void InitCountdown(int startFrom, unsigned int charSize, const sf::Vector2f pos);

	void Reset(const std::string text);
	void RestartCountDown();

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	bool IsPaused() { return m_paused; }
	bool CountHasEnded() { return m_count == 0; }

	void Pause();
	void Resume();

private:

	int m_count = 0;
	int m_startFrom = 0;
	bool m_countdown = false;
	bool m_loop = true;
	bool m_paused;
	bool m_reduceAlpha = true;
	float m_maxTime;
	TextAlignment m_alignment;
	Timer m_timer;
	sf::Text m_text;
	std::shared_ptr<sf::Shader> m_textShader;
};