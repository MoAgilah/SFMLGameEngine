#pragma once

#include <SFML/Graphics.hpp>

class BoundingCircle;
class BoundingCapsule;

class Point
{
public:
	Point();
	Point(float x, float y);
	Point(const sf::Vector2f& vec);
	~Point() = default;

	float& operator[] (std::size_t index);
	const float& operator[] (std::size_t index) const;
	friend Point operator-(const Point& a, const Point& b);
	friend Point operator+(const Point& a, const Point& b);
	friend Point operator*(const Point& a, float scalar);
	friend Point operator*(float scalar, const Point& a);
	friend Point operator/(const Point& a, const Point& b);

	friend Point& operator+=(Point& a, const Point& b);
	friend Point& operator-=(Point& a, const Point& b);
	friend Point& operator*=(Point& a, float scalar);
	friend Point& operator/=(Point& a, const Point& b);

	Point& operator=(const Point& other);
	friend bool operator==(const Point& a, const Point& b);
	friend bool operator!=(const Point& a, const Point& b);

	operator sf::Vector2f() const;

	Point Clamp(const Point& p1, const Point& p2) const;

	bool MovingPointIntersects(BoundingCircle* circle, const Point& v, float& tfirst, float& tlast);
	bool MovingPointIntersects(BoundingCapsule* circle, const Point& v, float& tfirst, float& tlast);

	float x;
	float y;
};

namespace pnt
{
	float lengthSquared(const Point& p);
	float dot(const Point& p1, const Point& p2);
	float distance(const Point& p1, const Point& p2);
	float length(const Point& p);
	Point Normalize(const Point& p);
	bool IsMovingTowards(Point p1, Point p2, Point v1, Point v2);
	bool IsBetween(const Point& p, const Point& a, const Point& b);
}