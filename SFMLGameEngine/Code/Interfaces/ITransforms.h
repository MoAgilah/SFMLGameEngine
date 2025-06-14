#pragma once

#include "../Utilities/Point.h"

class ITransforms
{
public:
	virtual ~ITransforms() = default;

	virtual void SetPosition(const Point& pos) = 0;
	virtual Point GetPosition() = 0;

	virtual void SetScale(const Point& scl) = 0;
	virtual Point GetScale() = 0;

	virtual Point GetOrigin() = 0;
	virtual void SetOrigin(const Point& pos) = 0;

	virtual Point GetSize() = 0;

protected:

	Point m_scale;
};

class IMoveable
{
public:
	virtual ~IMoveable() = default;

	virtual void SetVelocity(const Point& vel) { m_velocity = vel; }
	virtual Point GetVelocity() { return m_velocity; }

private:

	Point m_velocity;
};