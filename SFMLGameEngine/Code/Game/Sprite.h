#pragma once

#include "../Engine/Resources/TextureManager.h"
#include "../Utilities/Point.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <span>
#include <string>
#include <vector>

class Sprite
{
public:
	Sprite() = default;
	explicit Sprite(const std::string& texId);
	virtual ~Sprite() = default;

	std::string_view GetTexID() const { return m_texID; }

	void SetTexture(const std::string& texId);

	virtual void Update(float dt);
	void Render(sf::RenderTarget& window) const;

	sf::Sprite* GetSprite() { return m_sprite.get(); }

	void Move(float x, float y) { m_sprite->move({ x, y }); }

	Point GetPosition() const { return m_sprite->getPosition(); }
	void SetPosition(const Point& pos) { m_sprite->setPosition(pos); }

	virtual Point GetSize() const;

	Point GetOrigin() const { return m_sprite->getOrigin(); }
	void SetOrigin(const Point& pos) { m_sprite->setOrigin(pos); }

	sf::Vector2u GetTextureSize() const { return m_sprite->getTexture().getSize(); }
	void SetTextureRect(const sf::IntRect& rect) { m_sprite->setTextureRect(rect); }

	Point GetScale() const { return m_scale; }
	void SetScale(const Point& scale);

	sf::Vector2u GetFrameSize() const { return m_frameSize; }
	void SetFrameSize(const sf::Vector2u& size, int currentFrame = 1, int currentAnim = 1);

private:

	Point m_scale;
	std::string m_texID;
	std::unique_ptr<sf::Sprite> m_sprite;
	sf::Vector2u m_frameSize;
};

struct Range
{
	int m_current = 0;
	int m_max;
};

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(const std::string& texId, int rows, int columns, float framesPerSec, bool symmetrical, float animationSpeed);
	AnimatedSprite(const std::string& texId, float framesPerSec, bool symmetrical, float animationSpeed);
	~AnimatedSprite() final = default;

	void Update(float dt) override;

	Point GetSize() const;

	void ChangeAnim(int animNum);
	int GetCurrentAnim() const { return m_animation.m_current; }

	void SetFrames(const std::vector<int>& numFrames);
	void SetFrameData(int rows, int columns, const std::vector<int>& numFrames);

	bool PlayedNumTimes(int val) const { return m_animCycles == val; }
	bool PlayedOnce() const { return m_animCycles > 0; }

	void SetShouldLoop(bool loop) { m_loop = loop; }

	void UpdateAnimSpeed(float animSpd);

private:

	Range m_frame;
	Range m_animation;
	bool m_symmetrical;
	bool m_loop = true;
	int m_frameTime = 0;
	int m_animCycles = 0;
	float m_animSpeed = 0;
	float m_currentTime = 0;
	float m_framesPerSecond = 0;
	std::vector<int> m_numFrames;
};