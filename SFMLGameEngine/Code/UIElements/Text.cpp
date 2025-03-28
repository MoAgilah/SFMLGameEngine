#include "Menu.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include <format>
#include <iostream>

void CalculateTextOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

Text::Text(const std::string fontName, float fadeTime)
	: m_paused(false), m_maxTime(fadeTime), m_timer(m_maxTime), m_time(1.f)
{
	m_text.setFont(*GameManager::Get()->GetFontMgr().GetFont(fontName));
	m_textShader.reset(GameManager::Get()->GetShaderMgr().GetShader("FadeInOutShader"));
}

void Text::Init(const std::string text, unsigned int charSize, const sf::Vector2f pos, sf::Color color, TextAlignment alignment, bool loop, bool paused)
{
	m_loop = loop;
	m_countdown = false;
	m_position = pos;
	m_alignment = alignment;

	if (paused)
		Pause();
	else
		Resume();

	m_text.setCharacterSize(charSize);
	m_text.setString(text);
	m_text.setOutlineThickness(charSize / 10.f);
	m_text.setOutlineColor(color);

	SetTextPosition(pos);
}

void Text::SetTextPosition(const sf::Vector2f& pos)
{
	sf::FloatRect bounds = m_text.getLocalBounds();

	switch (m_alignment) {
	case LeftHand:
		m_text.setPosition(10.f, GameConstants::ScreenDim.y / 2.f - bounds.height / 2.f);
		break;
	case Center:
		m_text.setPosition((GameConstants::ScreenDim.x - bounds.width) / 2.f - bounds.left,
							GameConstants::ScreenDim.y / 2.f - bounds.height / 2.f);
		break;
	case RightHand:
		m_text.setPosition(GameConstants::ScreenDim.x - bounds.width - 10.f - bounds.left,
							GameConstants::ScreenDim.y / 2.f - bounds.height / 2.f);
		break;
	default:
		m_text.setPosition(pos);
		CalculateTextOrigin(m_text);
		break;
	}
}

void Text::Reset(const std::string text)
{
	m_text.setString(text);
	SetTextPosition(m_position);
	m_timer.ResetTime();
	m_reduceAlpha = true;
	m_paused = false;
}

void Text::RestartCountDown()
{
	m_count = m_startFrom;
	Reset(std::to_string(m_count));
}

void Text::Update(float deltaTime)
{
	if (m_paused)
	{
		m_time = m_loop ? 1.f : 0.f;
		return;
	}

	if (m_reduceAlpha)
	{
		m_timer.Update(deltaTime);
		m_time = m_timer.GetTime() / m_maxTime;

		if (m_timer.CheckEnd())
		{
			m_reduceAlpha = false;
			if (m_time < 0.f)
				m_time = 0.f;
		}
	}
	else
	{
		if (m_loop)
		{
			m_timer.Update(-deltaTime);
			m_time = m_timer.GetTime() / m_maxTime;

			if (m_timer.GetTime() >= m_maxTime)
			{
				m_reduceAlpha = true;
				m_timer.ResetTime();
			}
		}
		else
		{
			if (m_countdown && !CountHasEnded())
			{
				--m_count;
				if (m_count != 0)
				{
					Reset(std::to_string(m_count));
				}
				else
				{
					Reset(m_countdownMsg);
				}
			}
			else
			{
				m_paused = true;
			}
		}
	}
}

void Text::Render(sf::RenderWindow& window)
{
	m_textShader->setUniform("time", m_time);
	window.draw(m_text, m_textShader.get());
}

void Text::InitCountdown(const std::string text, int startFrom, unsigned int charSize, const sf::Vector2f pos, sf::Color color, TextAlignment alignment)
{
	m_countdownMsg = text;
	m_count = m_startFrom = startFrom;
	Init(std::to_string(m_count), charSize, pos, color, alignment, false);
	m_countdown = true;
}

void Text::InitStaticText(const std::string text, unsigned int charSize, const sf::Vector2f pos, sf::Color color, TextAlignment alignment)
{
	Init(text, charSize, pos, color, alignment, true, true);
}

void Text::InitFlashingText(const std::string text, unsigned int charSize, const sf::Vector2f pos, sf::Color color, TextAlignment alignment, bool paused)
{
	Init(text, charSize, pos, color, alignment, true, paused);
}

void Text::Pause()
{
	m_paused = true;
}

void Text::Resume()
{
	m_paused = false;
}
