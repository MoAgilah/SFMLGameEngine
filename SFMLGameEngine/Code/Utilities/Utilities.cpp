#include "Utilities.h"
#include <array>
#include <numbers>
#include "../Collisions/BoundingVolume.h"

bool IsPlayerObject(const std::string& texID)
{
	return texID == "";
}

bool IsBoxObject(const std::string& texID)
{
	return texID == "";
}

std::vector<std::string> enemyObject =
{
};

bool IsEnemyObject(const std::string& texID)
{
	return std::find(enemyObject.begin(), enemyObject.end(), texID) != enemyObject.end();
}

std::vector<std::string> collectableObject =
{
};

bool IsCollectableObject(const std::string& texID)
{
	return std::find(collectableObject.begin(), collectableObject.end(), texID) != collectableObject.end();
}

namespace
{
	float Dot(const Point& a, const Point& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float SqDistPointSegment(const Point& a, const Point& b, const Point& c)
	{
		Point ab = b - a;
		Point ac = c - a;
		Point bc = c - b;

		float e = Dot(ac, ab);

		// Handle cases where c projects outside ab
		if (e <= 0.0f)
			return Dot(ac, ac);

		float f = Dot(ab, ab);
		if (e >= f)
			return Dot(bc, bc);

		// Handle cases where c projects onto ab
		return Dot(ac, ac) - e * e / f;
	}
}

Line::Line(const Point& start, const Point& end)
	: start(start), end(end)
{
}

Line::Line(const sf::ConvexShape& tri, int bgn, int end)
	: Line(tri.getPoint(bgn), tri.getPoint(end))
{}

Point Line::GetMidPoint() const
{
	float x = (start.x + end.x) / 2.f;
	float y = (start.y + end.y) / 2.f;

	return Point(x, y);
}

float Line::CalculateAngle() const
{
	float angleInDegrees = atan2(DistY(), DistX()) * (180.f / std::numbers::pi_v<float>);

	return angleInDegrees;
}

float Line::SqDistPointSegment(const Point& p)
{
	Point es = end - start;
	Point ps = p - start;
	Point pe = p - end;

	float e = Dot(ps, es);

	// Handle cases where c projects outside ab
	if (e <= 0.0f)
		return Dot(ps, ps);

	float f = Dot(es, es);
	if (e >= f)
		return Dot(pe, pe);

	// Handle cases where c projects onto ab
	return Dot(ps, ps) - e * e / f;
}

Point Line::ClosestPointOnLineSegment(const Point& pnt) const
{
	Point AB = end - start;
	Point AP = pnt - start;

	float abLengthSquared = pnt::lengthSquared(AB);
	if (abLengthSquared == 0.0)
	{
		// A and B are the same point
		return start;
	}

	// Projection factor (t)
	float t = pnt::dot(AP, AB) / abLengthSquared;

	// Clamp t to [0, 1] to stay within the segment
	t = std::max(0.0f, std::min(1.0f, t));

	// Compute the closest point
	return { start.x + t * AB.x, start.y + t * AB.y };
}

bool Line::IsPointAboveLine(const Point & pnt) const
{
	float s = (end.x - start.x) * (pnt.y - start.y) - (end.y - start.y) * (pnt.x - start.x);
	return s > 0;
}

bool Line::IntersectsPoint(const Point& pnt) const
{
	// get distance from the point to the two ends of the line
	float d1 = pnt::distance(pnt, end);
	float d2 = pnt::distance(pnt, start);

	float lineLen = pnt::distance(start, end);

	// a little buffer zone that will give collision
	float buffer = 0.1f;    // higher # = less accurate

	// if the two distances are equal to the line's
	// length, the point is on the line!
	return d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer;
}

float GetXDist(const Point& p1, const Point& p2)
{
	return p2.x - p1.x;
}

float GetYDist(const Point& p1, const Point& p2)
{
	return p2.y - p1.y;
}