#include "Menu.h"
#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include <format>
#include <iostream>

FlashingText::FlashingText(const std::string fontName, float fadeTime)
	: m_paused(false), m_maxTime(fadeTime), m_timer(m_maxTime)
{
	m_text.setFont(*GameManager::Get()->GetFontMgr().GetFont(fontName));

	m_textShader.reset(GameManager::Get()->GetShaderMgr().GetShader("FadeInOutShader"));
}

void FlashingText::Init(const std::string text, unsigned int charSize, const sf::Vector2f pos, bool loop)
{
	m_loop = loop;

	m_text.setCharacterSize(charSize);
	m_text.setString(text);
	m_text.setOutlineThickness(charSize / 10.f);
	m_text.setOutlineColor(sf::Color::Black);

	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin((textRect.left + textRect.width) / 2.0f,
		(textRect.top + textRect.height) / 2.0f);

	m_text.setPosition(pos);
}

void FlashingText::Reset(const std::string text)
{
	m_text.setString(text);

	sf::FloatRect textRect = m_text.getLocalBounds();
	if (text == "1")
	{
		textRect.left - 5;
		textRect.width = 50;
	}

	m_text.setOrigin((textRect.left + textRect.width) / 2.0f,
		(textRect.top + textRect.height) / 2.0f);

	m_timer.ResetTime();
	m_reduceAlpha = true;
	m_paused = false;
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
				m_paused = true;
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
