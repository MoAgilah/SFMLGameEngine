#pragma once

#include "Object.h"
#include "../../Engine/Core/Timer.h"
#include <SFML/Graphics.hpp>

class Player;

class Enemy : public DynamicObject
{
public:
	Enemy(const std::string& texID, std::unique_ptr<BoundingVolume> volume, int maxLives = 1);
	Enemy(const std::string& texID, const AnimationData& animData, std::unique_ptr<BoundingVolume> volume, int maxLives = 1);
	~Enemy() override = default;

	bool GetActive() const final { return GetIsAlive() && Object::GetActive(); }

	void Update(float deltaTime) override;

	void Reset() override;

	bool GetAirbourne() const { return m_airbourne; }
	void SetAirbourne(bool air) { m_airbourne = air; }

	bool GetIsAlive() const { return m_isAlive; }
	bool HasLifes() const { return m_numLives > 0; }
	virtual void DecrementLife();
	virtual void Die() = 0;

	bool GetInvulnerabe() const { return m_invulnerable; }
	void SetInvulnerability(bool val) { m_invulnerable = val; }

	Timer* GetAirTimer() { return &m_airTimer; }
	void SetAirTime(float time);
	void SetTimeLeftActive(float time);

private:

	virtual void Animate(float deltaTime) = 0;

	int m_maxLives;
	int m_numLives = 1;
	bool m_isAlive = true;
	bool m_airbourne = false;
	bool m_invulnerable = false;
	bool m_resetAllowed = false;
	Timer m_airTimer;
	Timer m_resetTimer;
	Timer m_activationTimer;
};
