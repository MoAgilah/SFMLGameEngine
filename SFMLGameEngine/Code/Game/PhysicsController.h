#pragma once

#include "../Utilities/Point.h"
#include <utility>
#include <vector>

enum XVelocity
{
	XVelNone
};

enum YVelocity
{
	YVelNone
};

enum PhysicsType
{
	PhysicsTypeNone
};

struct Velocity
{
	float min;
	float max;
};

class PhysicsController
{
public:
	PhysicsController();
	~PhysicsController() = default;

	void Update(bool direction, const Point& currVelocity);

	PhysicsType GetPhysicsType() const { return (PhysicsType)m_currType; }
	XVelocity GetXVelocityType() const { return (XVelocity)m_currX; }

	float GetXAcceleration() const { return m_currAccelerations.x; }
	float GetYAcceleration() const { return m_currAccelerations.y; }

	float GetMaxXVelocity() const { return m_maxVelocity.first.max; }
	float GetMaxYVelocity() const { return m_maxVelocity.second; }

	float GetAirTime() const { return m_airTime; }
private:

	int m_currX;
	int m_currY;
	float m_airTime;
	PhysicsType m_currType;
	Point m_currAccelerations;
	float m_groundAcceleration;
	std::vector<float> m_aerialTimer;
	std::vector<float> m_aerialVelocities;
	std::vector<float> m_slopedAccelerations;
	std::vector<float> m_aerialAccelerations;
	std::pair<Velocity, float> m_maxVelocity;
	std::vector<Velocity> m_groundVelocities;
	std::vector<Velocity> m_slopedVelocities;
};