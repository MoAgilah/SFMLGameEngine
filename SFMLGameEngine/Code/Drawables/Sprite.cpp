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
		std::cerr << "Texture Error: " << e.what() << std::endl;
	}

	SetScale(GameConstants::Scale);
	SetOrigin(Point(m_sprite->getTexture().getSize()) * 0.5f);
}

void Sprite::Update(float dt)
{
	// does nothing
}

void Sprite::Render(sf::RenderTarget& window) const
{
	if (m_sprite)
		window.draw(*m_sprite);
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

	int left = currentFrame * size.x;
	int top = currentAnim * size.y;

	m_sprite->setTextureRect({ {left, top}, {static_cast<int>(size.x), static_cast<int>(size.y)} });
	SetOrigin(Point(size) * 0.5f);
}

AnimatedSprite::AnimatedSprite(const std::string& texId, int rows, int columns, float framesPerSec, bool symmetrical, float animSpeed)
	: Sprite(texId), m_animSpeed(animSpeed), m_framesPerSecond(framesPerSec / 1000.0f), m_symmetrical(symmetrical)
{
	auto texSize = GetTextureSize();
	SetFrameSize({ texSize.x / static_cast<unsigned>(columns), texSize.y / static_cast<unsigned>(rows) });
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
				m_frame.m_current = 0;
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

		int left = m_frame.m_current * GetFrameSize().x;
		int top = m_animation.m_current * GetFrameSize().y;
		SetTextureRect({ {left, top}, {static_cast<int>(GetFrameSize().x), static_cast<int>(GetFrameSize().y)} });
	}
}

Point AnimatedSprite::GetSize() const
{
	return GetFrameSize();
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
	auto texSize = GetTextureSize();
	SetFrameSize({ texSize.x / static_cast<unsigned>(columns), texSize.y / static_cast<unsigned>(rows) });
	SetFrames(numFrames);
}

void AnimatedSprite::UpdateAnimSpeed(float animSpd)
{
	if (m_animSpeed != animSpd)
		m_animSpeed = animSpd;
}