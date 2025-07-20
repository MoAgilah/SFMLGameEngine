#pragma once

#include "SFDrawables.h"
#include <SFML/Graphics.hpp>
#include <string>

class SFSprite : public SFDrawables<sf::Sprite>
{
public:
	SFSprite() = default;
	SFSprite(const std::string& texId);

	std::string_view GetTexID() const { return m_texID; }

	void SetTexture(const std::string& texId);

	void Update(float dt);

	sf::Sprite* GetSprite() { return this->GetPrimaryDrawableAs<sf::Sprite>(); }

	void Move(float x, float y);
	void Move(const Point& mov);

	sf::Vector2u GetTextureSize() const { return this->GetPrimaryDrawableAs<sf::Sprite>()->getTexture().getSize(); }
	void SetTextureRect(const sf::IntRect& rect) { this->GetPrimaryDrawableAs<sf::Sprite>()->setTextureRect(rect); }

private:

	std::string m_texID;
};

struct NRange
{
	int m_current = 0;
	int m_max;
};

class SFAnimatedSprite : public SFSprite
{
public:
	SFAnimatedSprite(const std::string& texId, int rows, int columns, float framesPerSec, bool symmetrical, float animationSpeed);
	SFAnimatedSprite(const std::string& texId, float framesPerSec, bool symmetrical, float animationSpeed);

	void Update(float dt) override;

	Point GetSize() override;

	sf::Vector2u GetFrameSize() const { return m_frameSize; }
	void SetFrameSize(const sf::Vector2u& size, int currentFrame = 1, int currentAnim = 1);

	void ChangeAnim(int animNum);
	int GetCurrentAnim() const { return m_animation.m_current; }

	void SetFrames(const std::vector<int>& numFrames);
	void SetFrameData(int rows, int columns, const std::vector<int>& numFrames);

	bool PlayedNumTimes(int val) const { return m_animCycles == val; }
	bool PlayedOnce() const { return m_animCycles > 0; }

	void SetShouldLoop(bool loop) { m_loop = loop; }

	void UpdateAnimSpeed(float animSpd);

private:

	NRange m_frame;
	NRange m_animation;
	bool m_symmetrical;
	bool m_loop = true;
	int m_frameTime = 0;
	int m_animCycles = 0;
	float m_animSpeed = 0;
	float m_currentTime = 0;
	float m_framesPerSecond = 0;
	sf::Vector2u m_frameSize;
	std::vector<int> m_numFrames;
};