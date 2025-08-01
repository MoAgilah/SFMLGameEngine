#pragma once

#include "Point.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace obj
{
	static std::vector<std::string> playerObjects;
	static std::vector<std::string> boxObjects;
	static std::vector<std::string> enemyObjects;
	static std::vector<std::string> collectableObjects;

	bool IsPlayerObject(const std::string& texID);
	bool IsBoxObject(const std::string& texID);
	bool IsEnemyObject(const std::string& texID);
	bool IsCollectableObject(const std::string& texID);
}

class IBoundingCircle;

struct Line
{
	Line() = default;
	Line(const Point& start, const Point& end);
	Line(const sf::ConvexShape& tri, int bgn, int end);

	Point GetMidPoint() const;
	float CalculateAngle() const;

	float SqDistPointSegment(const Point& p) const;
	Point ClosestPointOnLineSegment(const Point& pnt) const;

	bool IsPointAboveLine(const Point& pnt) const;
	bool IntersectsPoint(const Point& pnt) const;

	bool IntersectsMoving(IBoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) const;

	float DistX() const { return end.x - start.x; }
	float DistY() const { return end.y - start.y; }

	Point start;
	Point end;
};

float GetXDist(const Point& p1, const Point& p2);
float GetYDist(const Point& p1, const Point& p2);

// Solves a*t^2 + b*t + c = 0.
// Returns false if there are no real roots; otherwise, returns the roots in t0 and t1.
bool SolveQuadratic(float a, float b, float c, float& t0, float& t1);

struct FloatRect
{
	float m_left;
	float m_top;
	float m_width;
	float m_height;

	FloatRect(float left, float top, float width, float height);

	Point Min() const { return m_min; }
	Point Max() const { return m_max; }
	Point MidPoint() const { return m_midPoint; }

private:

	Point m_min;
	Point m_max;
	Point m_midPoint;
};