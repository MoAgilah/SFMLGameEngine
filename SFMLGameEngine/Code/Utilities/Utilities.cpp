#include "Utilities.h"
#include <array>
#include <numbers>
#include "../Collisions/BoundingVolume.h"

namespace obj
{
	bool IsPlayerObject(const std::string& texID)
	{
		return std::find(playerObjects.begin(), playerObjects.end(), texID) != playerObjects.end();
	}

	bool IsBoxObject(const std::string& texID)
	{
		return std::find(boxObjects.begin(), boxObjects.end(), texID) != boxObjects.end();
	}

	bool IsEnemyObject(const std::string& texID)
	{
		return std::find(enemyObjects.begin(), enemyObjects.end(), texID) != enemyObjects.end();
	}

	bool IsCollectableObject(const std::string& texID)
	{
		return std::find(collectableObjects.begin(), collectableObjects.end(), texID) != collectableObjects.end();
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

	float e = pnt::dot(ps, es);

	// Handle cases where c projects outside ab
	if (e <= 0.0f)
		return pnt::dot(ps, ps);

	float f = pnt::dot(es, es);
	if (e >= f)
		return pnt::dot(pe, pe);

	// Handle cases where c projects onto ab
	return pnt::dot(ps, ps) - e * e / f;
}

Point Line::ClosestPointOnLineSegment(const Point& pnt) const
{
	Point segmentVector = end - start;
	Point pointVector = pnt - start;

	float segmentLengthSquared = pnt::lengthSquared(segmentVector);
	if (segmentLengthSquared == 0.0f)
	{
		// The line segment is just a single point
		return start;
	}

	// Compute projection factor t
	float t = pnt::dot(pointVector, segmentVector) / segmentLengthSquared;

	// Clamp t to [0, 1] to ensure the closest point is within the segment
	t = std::clamp(t, 0.0f, 1.0f);

	// Compute the closest point on the segment
	return { start.x + t * segmentVector.x, start.y + t * segmentVector.y };
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