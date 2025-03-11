#include "Menu.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include <format>
#include <iostream>

void CalculateTextOrigin(sf::Text& text)
{
	sf::FloatRect textRect = text.getLocalBounds();
	auto len = text.getString().getSize();

	text.setOrigin(((textRect.left + text.getCharacterSize()) * len) / 2.0f,
		(textRect.top + textRect.height) / 2.0f);
}

FlashingText::FlashingText(const std::string fontName, float fadeTime)
	: m_paused(false), m_maxTime(fadeTime), m_timer(m_maxTime)
{
	m_text.setFont(*GameManager::Get()->GetFontMgr().GetFont(fontName));

	m_textShader.reset(GameManager::Get()->GetShaderMgr().GetShader("FadeInOutShader"));
}

void FlashingText::Init(const std::string text, unsigned int charSize, const sf::Vector2f pos, TextAlignment alignment, bool loop)
{
	m_loop = loop;
	if (m_countdown)
		m_countdown = false;

	m_text.setCharacterSize(charSize);
	m_text.setString(text);
	m_text.setOutlineThickness(charSize / 10.f);
	m_text.setOutlineColor(sf::Color::Black);


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

void FlashingText::InitCountdown(int startFrom, unsigned int charSize, const sf::Vector2f pos)
{
	m_alignment = TextAlignment::Center;
	m_count = m_startFrom = startFrom;
	Init(std::to_string(m_count), charSize, pos, TextAlignment::Center, false);
	m_countdown = true;
}

void FlashingText::Reset(const std::string text)
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

void FlashingText::RestartCountDown()
{
	m_count = m_startFrom;
	Reset(std::to_string(m_count));
}

void FlashingText::Update(float deltaTime)
{
	if (m_paused)
	{
		m_textShader->setUniform("time", m_loop ? 1.f : 0.f);
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

		m_textShader->setUniform("time", val);
	}
}

void FlashingText::Render(sf::RenderWindow& window)
{
	window.draw(m_text, m_textShader.get());
}

void FlashingText::Pause()
{
	m_paused = true;
}

void FlashingText::Resume()
{
	m_paused = false;
}
