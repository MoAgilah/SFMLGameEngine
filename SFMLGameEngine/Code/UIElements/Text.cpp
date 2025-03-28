#include "Menu.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include <format>
#include <iostream>

void CalculateTextOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2.0f,
		bounds.top + bounds.height / 2.0f);
}

Text::Text(const std::string fontName, float fadeTime)
	: m_paused(false), m_maxTime(fadeTime), m_timer(m_maxTime)
{
	m_text.setFont(*GameManager::Get()->GetFontMgr().GetFont(fontName));

	m_textShader.reset(GameManager::Get()->GetShaderMgr().GetShader("FadeInOutShader"));
}

void Text::Init(const std::string text, unsigned int charSize, const sf::Vector2f pos, sf::Color color, TextAlignment alignment, bool loop, bool paused)
{
	m_loop = loop;
	m_countdown = false;

	if (paused)
	{
		Pause();
	}
	else
	{
		Resume();
	}

	m_text.setCharacterSize(charSize);
	m_text.setString(text);
	m_text.setOutlineThickness(charSize / 10.f);
	m_text.setOutlineColor(color);


	sf::FloatRect textBounds = m_text.getLocalBounds();

	switch (alignment)
	{
	case LeftHand:
		m_text.setPosition(10, GameConstants::ScreenDim.y / 2 - textBounds.height / 2);
		break;
	case Center:
		m_text.setPosition((GameConstants::ScreenDim.x - textBounds.width) / 2 - textBounds.left,
			GameConstants::ScreenDim.y / 2 - textBounds.height / 2);
		break;
	case RightHand:
		m_text.setPosition(GameConstants::ScreenDim.x - textBounds.width - 10 - textBounds.left,
			GameConstants::ScreenDim.y / 2 - textBounds.height / 2);
		break;
	default:
		m_text.setPosition(pos);
		CalculateTextOrigin(m_text);

		break;
	}
}

void Text::InitCountdown(int startFrom, unsigned int charSize, const sf::Vector2f pos, sf::Color color, TextAlignment alignment)
{
	m_alignment = alignment;
	m_count = m_startFrom = startFrom;
	Init(std::to_string(m_count), charSize, pos, color, m_alignment, false);
	m_countdown = true;
}

void Text::InitStaticText(const std::string text, unsigned int charSize, const sf::Vector2f pos, sf::Color color, TextAlignment alignment)
{
	m_alignment = alignment;
	Init(text, charSize, pos, color, m_alignment, true, true);
}

void Text::InitFlashingText(const std::string text, unsigned int charSize, const sf::Vector2f pos, sf::Color color, TextAlignment alignment, bool paused)
{
	m_alignment = alignment;
	Init(text, charSize, pos, color, m_alignment, true, paused);
}

void Text::Reset(const std::string text)
{
	m_text.setString(text);

	sf::FloatRect textBounds = m_text.getLocalBounds();

	switch (m_alignment)
	{
	case LeftHand:
		m_text.setPosition(10, GameConstants::ScreenDim.y / 2 - textBounds.height / 2);
		break;
	case Center:
		m_text.setPosition((GameConstants::ScreenDim.x - textBounds.width) / 2 - textBounds.left,
			GameConstants::ScreenDim.y / 2 - textBounds.height / 2);
		break;
	case RightHand:
		m_text.setPosition(GameConstants::ScreenDim.x - textBounds.width - 10 - textBounds.left,
			GameConstants::ScreenDim.y / 2 - textBounds.height / 2);
		break;
	default:
		CalculateTextOrigin(m_text);

		break;
	}

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
	}
	else
	{
		float val = m_timer.GetTime() / m_maxTime;

		if (m_reduceAlpha)
		{
			m_timer.Update(deltaTime);
			if (m_timer.CheckEnd())
			{
				m_reduceAlpha = false;
				if (val < 0)
					val = 0;
			}
		}
		else
		{
			if (m_loop)
			{
				m_timer.Update(-deltaTime);
				if (m_timer.GetTime() >= m_maxTime)
				{
					m_reduceAlpha = true;
					m_timer.ResetTime();
				}
			}
			else
			{
				if (m_countdown)
				{
					if (!CountHasEnded())
					{
						m_count--;
						m_timer.ResetTime();
						Reset(std::to_string(m_count));
					}
					else
					{
						m_paused = true;
					}
				}
				else
				{
					m_paused = true;
				}
			}
		}

		m_time = val;
	}
}

void Text::Render(sf::RenderWindow& window)
{
	m_textShader->setUniform("time", m_time);
	window.draw(m_text, m_textShader.get());
}

void Text::Pause()
{
	m_paused = true;
}

void Text::Resume()
{
	m_paused = false;
}
