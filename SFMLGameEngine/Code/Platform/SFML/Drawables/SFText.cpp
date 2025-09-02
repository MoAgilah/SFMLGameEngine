#include "SFText.h"

#include "../Resource/SFFont.h"
#include "../Resource/SFShader.h"
#include "../../../Engine/Core/NGameManager.h"

SFText::SFText(const NTextConfig& config)
	: IText(config)
{
	auto font = dynamic_cast<SFFont*>(NGameManager::Get()->GetFontMgr().GetFont(m_config.m_fontName));

	if (font)
		SetDrawable(std::make_shared<sf::Text>(font->GetNativeFont()));
	else
		std::cerr << "Font Error: Could not find font '" << m_config.m_fontName << "'\n";

	SetCharSize(m_config.m_charSize);
	SetOutlineThickness(m_config.m_charSize / 10.f);
	SetOutlineColour(m_config.m_colour);
}

void SFText::Update(float deltaTime)
{
	SFDrawables<sf::Text>::Update(deltaTime);
}

void SFText::Render(IRenderer* renderer)
{
	SFDrawables<sf::Text>::Render(renderer);
}

void SFText::SetText(const std::string& text)
{
	auto txtObj = this->GetPrimaryDrawableAs<sf::Text>();
	if (txtObj)
	{
		txtObj->setString(text);
		auto bounds = txtObj->getLocalBounds();
		SetPosition(NSetTextPosition(m_config.m_alignment, m_config.m_position, bounds.size, bounds.position));
	}
}

void SFText::Reset(const std::string& text, std::optional<NTextConfig> config)
{
	if (config)
	{
		m_config = *config;
		Init();
	}

	SetText(text);
}

Point SFText::GetSize()
{
	sf::FloatRect bounds = this->GetPrimaryDrawableAs<sf::Text>()->getLocalBounds();
	return Point(bounds.size.x, bounds.size.y);
}

void SFText::Init()
{
	SetCharSize(m_config.m_charSize);
	SetOutlineThickness(m_config.m_charSize / 10.f);
	SetOutlineColour(m_config.m_colour);
}

SFAnimatedText::SFAnimatedText(const NTextConfig& config)
	: SFText(config), m_timer(1.f), m_textShader(nullptr)
{
	switch (m_config.m_animType)
	{
	case NTextAnimType::Flashing:
	case NTextAnimType::Countdown:
	{
		auto shader = dynamic_cast<SFShader*>(NGameManager::Get()->GetShaderMgr().GetShader("FadeInOutShader"));
		if (shader)
			m_textShader = &shader->GetNativeShader();
		else
			std::cerr << "Shader Error: 'FadeInOutShader' not found\n";
	}
		break;
	}
}

void SFAnimatedText::Update(float deltaTime)
{
	switch (m_config.m_animType)
	{
	case NTextAnimType::Flashing:
	case NTextAnimType::Countdown:
		FadeInAndOutUpdate(deltaTime);
		break;
	case NTextAnimType::Custom:
		if (m_updateFunc)
			m_updateFunc(deltaTime);
		break;
	}
}

void SFAnimatedText::Render(IRenderer* renderer)
{
	switch (m_config.m_animType)
	{
	case NTextAnimType::Flashing:
	case NTextAnimType::Countdown:
		FadeInFadeOutRender(renderer);
		break;
	case NTextAnimType::Custom:
		if (m_renderFunc)
			m_renderFunc(renderer);
		break;
	}
}

void SFAnimatedText::Reset(const std::string& text, std::optional<NTextConfig> config)
{
	if (config)
		m_config = *config;

	SetText(text);
}

void SFAnimatedText::SetMaxCount(int startFrom)
{
	m_count = m_maxCount = startFrom;
}

void SFAnimatedText::SetUpdateFunc(NUpdateFunc func)
{
	m_updateFunc = func;
}

void SFAnimatedText::SetRenderFunc(NRenderFunc func)
{
	m_renderFunc = func;
}

void SFAnimatedText::FadeInAndOutUpdate(float deltaTime)
{
	if (m_paused)
	{
		m_timer.SetCurrTime(m_looping ? 1.f : 0.f);
		return;
	}

	float time;

	if (m_reduceAlpha)
	{
		m_timer.Update(deltaTime);
		time = m_timer.GetCurrTime() / m_timer.GetMaxTime();

		if (m_timer.CheckEnd())
		{
			m_reduceAlpha = false;
			if (time < 0.f)
				m_timer.SetCurrTime(0.f);
		}
	}
	else
	{
		if (m_looping)
		{
			m_timer.Update(-deltaTime);
			time = m_timer.GetCurrTime() / m_timer.GetMaxTime();

			if (m_timer.GetCurrTime() >= m_timer.GetMaxTime())
			{
				m_reduceAlpha = true;
				m_timer.RestartTimer();
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

void SFAnimatedText::FadeInFadeOutRender(IRenderer* renderer)
{
	if (!renderer || !this->GetPrimaryDrawableAs<sf::Text>() || !m_textShader)
		return;

	m_textShader->setUniform("time", m_timer.GetCurrTime());

	if (auto* windowHandle = renderer->GetWindow())
	{
		auto* sfWindow = static_cast<sf::RenderWindow*>(windowHandle->GetNativeHandle());
		if (sfWindow)
			sfWindow->draw(*this->GetPrimaryDrawableAs<sf::Text>(), m_textShader);
	}
}

void InitFlashingText(SFAnimatedText* txtObj, const std::string& text, bool loop, std::optional<NTextConfig> config)
{
	if (txtObj)
	{
		txtObj->SetIsLooping(loop);
		txtObj->Reset(text, config);
	}
}

void InitCountdownText(SFAnimatedText* txtObj, int startFrom, const std::string& countDownMessage, std::optional<NTextConfig> config)
{
	if (txtObj)
	{
		txtObj->SetMaxCount(startFrom);
		txtObj->SetIsLooping(false);
		txtObj->SetCountDown(countDownMessage);
		txtObj->Reset(std::to_string(startFrom), config);
	}
}

void InitCustomTextAnim(SFAnimatedText* txtObj, const std::string& text, NUpdateFunc updator, NRenderFunc rendaror, const std::string& shaderName, std::optional<NTextConfig> config)
{
	if (txtObj)
	{
		txtObj->SetUpdateFunc(updator);
		txtObj->SetRenderFunc(rendaror);
		txtObj->Reset(text, config);
	}
}