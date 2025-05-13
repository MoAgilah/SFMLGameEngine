#include "PhysicsController.h"

#include "Constants.h"
#include<format>
#include<iostream>

PhysicsController::PhysicsController()
	: m_currX(XVelocity::XVelNone), m_currY(YVelocity::YVelNone), m_currType(PhysicsType::PhysicsTypeNone)
{
	m_groundVelocities.push_back({ GameConstants::ObjectSpeed * GameConstants::Scale.x });

	m_slopedVelocities.push_back({ GameConstants::ObjectSpeed * GameConstants::Scale.x });

	m_aerialVelocities = { GameConstants::ObjectSpeed * GameConstants::Scale.x };
	m_aerialTimer = { 0.5f };
	m_airTime = m_aerialTimer[m_currType];

	m_maxVelocity = { m_groundVelocities[m_currX], m_aerialVelocities[m_currY] };

	m_groundAcceleration = 0.0546875f;
	m_slopedAccelerations = { 0.125f };
	m_aerialAccelerations = { 0.0625f };

	m_currAccelerations.x = m_groundAcceleration;
	m_currAccelerations.y = m_aerialAccelerations[m_currY];
}

void PhysicsController::Update(bool direction, const Point& currVelocity)
{
}
