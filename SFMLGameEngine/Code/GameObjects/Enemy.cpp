#include "Enemy.h"

#include "../Game/GameManager.h"

Enemy::Enemy(const std::string& texID, const Point& boxSize, int maxLives)
	: DynamicObject(texID, boxSize), m_numLives(maxLives), m_maxLives(m_numLives),
	m_airTimer(0), m_resetTimer(0), m_activationTimer(0)
{
}

Enemy::Enemy(const std::string& texID, const AnimationData& animData, const Point& boxSize, int maxLives)
	: DynamicObject(texID, animData, boxSize),
	m_numLives(maxLives), m_maxLives(m_numLives), m_airTimer(0), m_resetTimer(0), m_activationTimer(0)
{
}

void Enemy::Update(float deltaTime)
{
	if (GetActive())
	{
		Animate(deltaTime);

		if (!HasLifes())
		{
			m_activationTimer.Update(deltaTime);
			if (m_activationTimer.CheckEnd())
				m_isAlive = false;
		}
	}
	else
	{
		if (!m_resetAllowed)
		{
			m_resetTimer.SetTime(1);
			m_resetAllowed = true;
		}

		if (m_resetAllowed)
		{
			m_resetTimer.Update(deltaTime);
			if (m_resetTimer.CheckEnd())
			{
				if (GameManager::Get()->GetCamera().CheckVerticalBounds(GetBoundingBox()))
					Reset();
			}
		}
	}
}

void Enemy::Reset()
{
	DynamicObject::Reset();

	m_isAlive = true;
	m_numLives = m_maxLives;
	m_resetAllowed = false;
	m_invulnerable = false;

	m_resetTimer.ResetTime();
	m_activationTimer.ResetTime();
}

void Enemy::DecrementLife()
{
	if (--m_numLives == 0)
		Die();
}

void Enemy::SetAirTime(float time)
{
	m_airTimer.SetTime(time);
}

void Enemy::SetTimeLeftActive(float time)
{
	m_activationTimer.SetTime(time);
}