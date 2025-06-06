#include "Sprite.h"

#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include <format>
#include <iostream>

Sprite::Sprite(const std::string& id)
{
	SetTexture(id);
}

void Sprite::SetTexture(const std::string& texId)
{
	m_texID = texId;

	try
	{
		m_sprite = std::make_unique<sf::Sprite>(*GameManager::Get()->GetTextureMgr().GetTexture(m_texID));
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what() << std::endl;
	}

	SetScale(GameConstants::Scale);
	SetOrigin(Point(m_sprite->getTexture().getSize()) * 0.5f);
}

void Sprite::Update(float dt)
{
	// does nothing
}

void Sprite::Render(sf::RenderWindow& window) const
{
	const sf::Sprite* sprite = m_sprite.get();
	if (sprite) {
		window.draw(*sprite);
	}
}

Point Sprite::GetSize() const
{
	auto bounds = m_sprite->getGlobalBounds();
	return Point(bounds.size);
}

void Sprite::SetScale(const Point& scale)
{
	m_scale = scale;
	m_sprite->setScale(scale);
}

void Sprite::SetFrameSize(const sf::Vector2u& size, int currentFrame, int currentAnim)
{
	m_frameSize = size;

	//set first frame to display
	int left = static_cast<int>(currentFrame * m_frameSize.x);
	int top = static_cast<int>(currentAnim * m_frameSize.y);
	int width = static_cast<int>(m_frameSize.x);
	int height = static_cast<int>(m_frameSize.y);

	SetTextureRect(sf::IntRect(sf::Vector2i(left, top),
		sf::Vector2i(width, height)));
	SetOrigin({ (float)m_frameSize.x / 2.f, (float)m_frameSize.y / 2.f });
}

AnimatedSprite::AnimatedSprite(const std::string& texId, int rows, int columns, float framesPerSec, bool symmetrical, float animSpeed)
	: Sprite(texId), m_animSpeed(animSpeed), m_framesPerSecond(framesPerSec / 1000.0f), m_symmetrical(symmetrical)
{
	//set single frame size
	SetFrameSize(sf::Vector2u(GetTextureSize().x / columns, GetTextureSize().y / rows), m_frame.m_current, m_animation.m_current);
}

AnimatedSprite::AnimatedSprite(const std::string& texId, float framesPerSec, bool symmetrical, float animSpeed)
	: Sprite(texId), m_animSpeed(animSpeed), m_framesPerSecond(framesPerSec / 1000.0f), m_symmetrical(symmetrical)
{
}

void AnimatedSprite::Update(float dt)
{
	if (!m_loop)
	{
		if (m_frame.m_current == m_numFrames[m_animation.m_current])
			return;
	}

	m_currentTime += m_animSpeed * dt;

	if (m_currentTime >= m_framesPerSecond)
	{
		m_currentTime = 0.0f;

		++m_frame.m_current;

		if (m_symmetrical)
		{
			if (m_frame.m_current >= m_frame.m_max)
			{
				m_frame.m_current = 0;
			}
		}
		else
		{
			if (m_frame.m_current >= m_numFrames[m_animation.m_current])
			{
				if (m_loop)
					m_frame.m_current = 0;
				else
					--m_frame.m_current;
				++m_animCycles;
			}
		}

		int left = static_cast<int>(m_frame.m_current * GetFrameSize().x);
		int top = static_cast<int>(m_animation.m_current * GetFrameSize().y);
		int width = static_cast<int>(GetFrameSize().x);
		int height = static_cast<int>(GetFrameSize().y);

		SetTextureRect(sf::IntRect(sf::Vector2i(left, top),
			sf::Vector2i(width, height)));
	}
}

Point AnimatedSprite::GetSize() const
{
	auto frameSize = GetFrameSize();
	return Point((float)frameSize.x, (float)frameSize.y);
}

void  AnimatedSprite::ChangeAnim(int animNum)
{
	m_animCycles = 0;
	m_animation.m_current = animNum;
}

void AnimatedSprite::SetFrames(const std::vector<int>& numFrames)
{
	m_numFrames.assign(numFrames.begin(), numFrames.end());
	m_animation.m_max = m_numFrames[m_animation.m_current];
}

void AnimatedSprite::SetFrameData(int rows, int columns, const std::vector<int>& numFrames)
{
	//set single frame size
	SetFrameSize(sf::Vector2u(GetTextureSize().x / columns, GetTextureSize().y / rows), m_frame.m_current, m_animation.m_current);

	m_numFrames = numFrames;
	m_animation.m_max = m_numFrames[m_animation.m_current];
}

void AnimatedSprite::UpdateAnimSpeed(float animSpd)
{
	if (m_animSpeed != animSpd)
		m_animSpeed = animSpd;
}