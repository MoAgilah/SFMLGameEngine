#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "Point.h"

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

struct Line
{
	Line() = default;
	Line(const Point& start, const Point& end);
	Line(const sf::ConvexShape& tri, int bgn, int end);

	Point GetMidPoint() const;
	float CalculateAngle() const;

	float SqDistPointSegment(const Point& p);
	Point ClosestPointOnLineSegment(const Point& pnt) const;

	bool IsPointAboveLine(const Point& pnt) const;
	bool IntersectsPoint(const Point& pnt) const;

	float DistX() const { return end.x - start.x; }
	float DistY() const { return end.y - start.y; }

	Point start;
	Point end;
};

float GetXDist(const Point& p1, const Point& p2);
float GetYDist(const Point& p1, const Point& p2);