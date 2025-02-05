#include "Point.h"
#include <stdexcept>
#include <SFML/Graphics.hpp>

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
	Point v = v2 - v1;
	Point d = p2 - p1;

	return (d.x * v.x + d.y * v.y) < 0;
}
