#include "Point.h"
#include <stdexcept>
#include "../Code/Collisions/BoundingVolume.h"


Point::Point()
	: x(0), y(0)
{
}

Point::Point(float x, float y)
	: x(x), y(y)
{
}

Point::Point(const sf::Vector2f& vec)
	: x(vec.x), y(vec.y)
{
}

float& Point::operator[](std::size_t index)
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Point subscript out of range");
	}
}

const float& Point::operator[](std::size_t index) const
{
	switch (index)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Point subscript out of range");
	}
}

Point operator-(const Point& a, const Point& b)
{
	return Point(a.x - b.x, a.y - b.y);
}

Point operator+(const Point& a, const Point& b)
{
	return Point(a.x + b.x, a.y + b.y);
}

Point operator*(const Point& a, float scalar)
{
	return Point(a.x * scalar, a.y * scalar);
}

Point operator*(float scalar, const Point& a)
{
	return Point(a.x * scalar, a.y * scalar);
}

Point operator/(const Point& a, const Point& b)
{
	return Point(a.x / b.x, a.y / b.y);
}

Point& operator+=(Point& a, const Point& b)
{
	a.x += b.x;
	a.y += b.y;

	return a;
}

Point& operator-=(Point& a, const Point& b)
{
	a.x -= b.x;
	a.y -= b.y;

	return a;
}

Point& operator*=(Point& a, float scalar)
{
	a.x *= scalar;
	a.y *= scalar;

	return a;
}

Point& operator/=(Point& a, const Point& b)
{
	a.x /= b.x;
	a.y /= b.y;

	return a;
}

Point& Point::operator=(const Point& other)
{
	this->x = other.x;
	this->y = other.y;
	return *this;
}

bool operator==(const Point& a, const Point& b)
{
	return a.x == b.x && a.y == b.y;
}

bool operator!=(const Point& a, const Point& b)
{
	return a.x != b.x || a.y != b.y;
}

Point::operator sf::Vector2f() const
{
	return sf::Vector2f(x, y);
}

Point Point::Clamp(const Point& p1, const Point& p2) const
{
	return { std::max(p1.x, std::min(p2.x, x)), std::max(p1.y, std::min(p2.y, y)) };
}

bool Point::MovingPointIntersects(BoundingCircle* circle, const Point& v, float& tfirst, float& tlast)
{
	// Compute the vector from the sphere center to the starting point.
	Point m = (*this) - circle->GetCenter();

	float radius = circle->GetRadius();

	// Coefficients for quadratic: |m + v*t|^2 = radius^2.
	float a = pnt::dot(v, v);
	float b = 2.0f * pnt::dot(v, m);
	float c = pnt::dot(m, m) - radius * radius;

	if (!SolveQuadratic(a, b, c, tfirst, tlast))
		return false;

	return true;
}

bool Point::MovingPointIntersects(BoundingCapsule* cap, const Point& v, float& tfirst, float& tlast)
{
	Line seg = cap->GetSegment();
	// Compute the normalized axis direction of the segment.
	Point d = seg.end - seg.start;
	d = pnt::Normalize(d);

	// Compute the vector from one end of the segment to the starting point.
	Point delta = (*this) - seg.start;

	// Remove the component parallel to the segment’s axis.
	Point delta_perp = delta - d * pnt::dot(delta, d);
	Point v_perp = v - d * pnt::dot(v, d);

	float radius = cap->GetRadius();

	// Now, the collision condition becomes: |delta_perp + v_perp * t|^2 = radius^2.
	float a = pnt::dot(v_perp, v_perp);
	float b = 2.0f * pnt::dot(v_perp, delta_perp);
	float c = pnt::dot(delta_perp, delta_perp) - radius * radius;

	if (!SolveQuadratic(a, b, c, tfirst, tlast))
		return false;

	return true;
}

float pnt::lengthSquared(const Point& p)
{
	return p.x * p.x + p.y * p.y;
}

float pnt::dot(const Point& p1, const Point& p2)
{
	return p1.x * p2.x + p1.y * p2.y;
}

float pnt::distance(const Point& p1, const Point& p2)
{
	return std::sqrtf(std::powf((p2.x - p1.x), 2) + std::powf((p2.y - p1.y), 2));
}

float pnt::length(const Point& p)
{
	return std::sqrtf(dot(p,p));
}

Point pnt::Normalize(const Point& p)
{
	auto len = length(p);
	return Point(p.x / len, p.y / len);
}

bool pnt::IsMovingTowards(Point p1, Point p2, Point v1, Point v2)
{
	Point relativePosition = p2 - p1;
	Point relativeVelocity = v2 - v1;

	float res = pnt::dot(relativePosition, relativeVelocity);

	// If the dot product is negative, they are moving towards each other
	return res < 0;
}

bool pnt::IsBetween(const Point& p, const Point& a, const Point& b)
{
	// p is between a and b if the dot product of (p - a) and (p - b) is non-positive.
	return dot(p - a, p - b) <= 0.0f;
}