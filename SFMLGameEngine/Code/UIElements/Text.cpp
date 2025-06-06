#include "Text.h"

#include "../Game/GameManager.h"

Point CalculateTextOrigin(const sf::FloatRect& bounds)
{
	return { bounds.position.x + bounds.size.x / 2.f, bounds.position.x + bounds.size.y / 2.f };
}

Point SetTextPosition(TextAlignment alignment, const Point& pos, const sf::FloatRect& bounds)
{
	switch (alignment)
	{
	case LeftHand:
		return { pos.x, pos.y - bounds.size.y / 2.f };
	case Center:
		return { pos.x - bounds.size.x / 2.f - bounds.position.x,
			pos.y - bounds.size.y / 2.f };
	case RightHand:
		return { pos.x - bounds.size.x - bounds.position.x,
			pos.y - bounds.size.y / 2.f };
		break;
	default:
		CalculateTextOrigin(bounds);
		return pos;
	}
}

Text::Text(const TextConfig& config)
	: m_config(config), m_text(*GameManager::Get()->GetFontMgr().GetFont(config.m_fontName))
{
}

void Text::Update(float deltaTime)
{
	// does nothing sf::text is static
}

void Text::Render(sf::RenderWindow& window)
{
	window.draw(m_text);
}

void Text::Reset(const std::string& text)
{
	m_text.setString(text);
	m_text.setPosition(SetTextPosition(m_config.m_alignment, m_config.m_position, m_text.getLocalBounds()));
}

void Text::SetText(const std::string& text, std::optional<TextConfig> config)
{
	if (config)
		m_config = *config;

	m_text.setCharacterSize(m_config.m_charSize);
	m_text.setString(text);
	m_text.setOutlineThickness(m_config.m_charSize / 10.f);
	m_text.setOutlineColor(m_config.m_colour);

	m_text.setPosition(SetTextPosition(m_config.m_alignment, m_config.m_position, m_text.getLocalBounds()));
}

void Text::SetPosition(const Point& pos)
{
	m_config.m_position = pos;
}

Point Text::GetPosition()
{
	return m_text.getPosition();
}

Point Text::GetSize()
{
	sf::FloatRect bounds = m_text.getLocalBounds();
	return Point(bounds.size.x, bounds.size.y);
}

void Text::SetCharSize(unsigned int charSize)
{
	m_text.setCharacterSize(charSize);
}

void Text::SetTextAlignment(TextAlignment alignment)
{
	m_config.m_alignment = alignment;
}

void Text::SetOutlineColour(const sf::Color& colour)
{
	m_text.setOutlineColor(colour);
}

void Text::SetFillColour(const sf::Color& colour)
{
	m_text.setFillColor(colour);
}

AnimatedText::AnimatedText(const TextConfig& config)
	: Text(config), m_timer(m_maxTime), m_time(1.f)
{
	switch (m_config.m_animType)
	{
	case Flashing:
	case Countdown:
		m_textShader.reset(GameManager::Get()->GetShaderMgr().GetShader("FadeInOutShader"));
		break;
	}
}

void AnimatedText::Update(float deltaTime)
{
	switch (m_config.m_animType)
	{
	case Flashing:
	case Countdown:
		FadeInAndOutUpdate(deltaTime);
		break;
	case Custom:
		if (m_customUpdate)
			m_customUpdate(deltaTime);
		break;
	}
}

void AnimatedText::Render(sf::RenderWindow& window)
{
	switch (m_config.m_animType)
	{
	case Flashing:
	case Countdown:
		FadeInFadeOutRender(window);
		break;
	case Custom:
		if (m_customRender)
			m_customRender(window);
		break;
	}
}

void AnimatedText::Reset(const std::string& text)
{
	Text::Reset(text);
	m_timer.ResetTime();
	m_reduceAlpha = true;
	m_paused = false;
}

void AnimatedText::InitFlashingText(const std::string& text, bool loop, std::optional<TextConfig> config)
{
	if (config)
		m_config = *config;

	m_loop = loop;
	SetText(text, m_config);
}

void AnimatedText::InitCountdownText(int startFrom, const std::string& countDownMessage, std::optional<TextConfig> config)
{
	if (config)
		m_config = *config;

	m_countdownMsg = countDownMessage;

	m_count = m_maxCount = startFrom;
	m_loop = false;
	SetText(std::to_string(m_maxCount), m_config);
}

void AnimatedText::InitCustomTextAnim(const std::string& text, UpdateFunc updator, RenderFunc rendaror, const std::string& shaderName, std::optional<TextConfig> config)
{
	if (!shaderName.empty())
		m_textShader.reset(GameManager::Get()->GetShaderMgr().GetShader(shaderName));

	if (config)
		m_config = *config;

	m_customUpdate = updator;
	m_customRender = rendaror;

	SetText(text, m_config);
}

void AnimatedText::FadeInAndOutUpdate(float deltaTime)
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
			if (!CountHasEnded())
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

void AnimatedText::FadeInFadeOutRender(sf::RenderWindow& window)
{
	m_textShader->setUniform("time", m_time);
	window.draw(m_text, m_textShader.get());
}