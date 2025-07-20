#pragma once

#include "../../Utilities/Colour.h"
#include "../../Utilities/Point.h"
#include "../../Utilities/Utilities.h"

class IShape
{
public:
	virtual ~IShape() = default;

	const Point& GetCenter() { return m_center; }
	void SetCenter(const Point& center) { m_center = center; }

	virtual void SetRotation(float rotation) = 0;

	virtual Colour GetFillColour() = 0;
	virtual void SetFillColour(const Colour& col) = 0;

	virtual Colour GetOutlineColour() = 0;
	virtual void SetOutlineColour(const Colour& col) = 0;

	virtual float GetOutlineThickness() = 0;
	virtual void SetOutlineThickness(float scale) = 0;

protected:

	Point m_center;
};

class ICircleShape
{
public:
	virtual ~ICircleShape() = default;
	virtual float GetRadius() = 0;
	virtual void SetRadius(float radius) = 0;
};

class ICapsuleShape : public IShape
{
public:
	virtual ~ICapsuleShape() = default;
	virtual float GetRadius() const = 0;
	virtual void SetRadius(float radius) = 0;

	virtual float GetLength() const = 0;
	virtual void SetLength(float length) = 0;

	virtual float GetAngle() const = 0;
	virtual void SetAngle(float angle) = 0;

	virtual const Line& GetSegment() const = 0;
};
