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
		m_sprite.setTexture(*GameManager::Get()->GetTextureMgr().GetTexture(m_texID), true);
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what() << std::endl;
	}

	SetScale(GameConstants::Scale);
	SetOrigin({ (float)m_sprite.getTexture()->getSize().x * 0.5f, (float)m_sprite.getTexture()->getSize().y * 0.5f });
}

void Sprite::Update(float dt)
{
	// does nothing
}

Point Sprite::GetSize() const
{
	auto bounds = m_sprite.getGlobalBounds();
	return Point(bounds.width, bounds.height);
}

void Sprite::SetScale(const Point& scale)
{
	m_scale = scale;
	m_sprite.setScale(scale);
}

void Sprite::SetFrameSize(const sf::Vector2u& size, int currentFrame, int currentAnim)
{
	m_frameSize = size;

	//set first frame to display
	SetTextureRect(sf::IntRect(currentFrame * m_frameSize.x, currentAnim * m_frameSize.y, m_frameSize.x, m_frameSize.y));
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

		//set new frame
		SetTextureRect(sf::IntRect(m_frame.m_current * GetFrameSize().x, m_animation.m_current * GetFrameSize().y, GetFrameSize().x, GetFrameSize().y));
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