#pragma once

#include "SFDrawables.h"
#include "../../../Engine/Interfaces/IText.h"
#include "../../../Engine/Core/Timer.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class SFText : public SFDrawables<sf::Text>, public IText
{
public:
	SFText() {}
	SFText(const NTextConfig& config);

	void Update(float deltaTime) override;
	void Render(IRenderer* renderer) override;

	virtual void SetText(const std::string& text);
	virtual void Reset(const std::string& text, std::optional<NTextConfig> config = std::nullopt);

	Point GetSize() override;

	unsigned int GetCharSize() override { return this->GetPrimaryDrawableAs<sf::Text>()->getCharacterSize(); }
	void SetCharSize(unsigned int charSize) override { this->GetPrimaryDrawableAs<sf::Text>()->setCharacterSize(charSize); }

	Colour GetOutlineColour() override { return this->GetPrimaryDrawableAs<sf::Text>()->getOutlineColor(); }
	void SetOutlineColour(const Colour& colour) override { this->GetPrimaryDrawableAs<sf::Text>()->setOutlineColor(colour); }

	Colour GetFillColour() override { return this->GetPrimaryDrawableAs<sf::Text>()->getFillColor(); }
	void SetFillColour(const Colour& colour) override { this->GetPrimaryDrawableAs<sf::Text>()->setFillColor(colour); }

	float GetOutlineThickness() override { return this->GetPrimaryDrawableAs<sf::Text>()->getOutlineThickness(); }
	void SetOutlineThickness(float thickness) override { this->GetPrimaryDrawableAs<sf::Text>()->setOutlineThickness(thickness); }

	void ResetOutlineColour() { SetOutlineColour(m_config.m_colour); }

private:

	void Init() override;
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
	sf::Shader* m_textShader;
};

void InitFlashingText(SFAnimatedText* txtObj, const std::string& text, bool loop = true, std::optional<NTextConfig> config = std::nullopt);
void InitCountdownText(SFAnimatedText* txtObj, int startFrom, const std::string& countDownMessage, std::optional<NTextConfig> config = std::nullopt);
void InitCustomTextAnim(SFAnimatedText* txtObj, const std::string& text, NUpdateFunc updator, NRenderFunc rendaror, const std::string& shaderName = "", std::optional<NTextConfig> config = std::nullopt);