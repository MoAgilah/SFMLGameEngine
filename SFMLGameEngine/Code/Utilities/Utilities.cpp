#include "Utilities.h"

#include "../Engine/Collisions/BoundingVolume.h"
#include <array>
#include <numbers>

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
{
}

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

float Line::SqDistPointSegment(const Point& p) const
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


bool Line::IsPointAboveLine(const Point& pnt) const
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

bool Line::IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) const
{
	// Get relative velocity vector
	Point relativeVelocity = vb - va;

	// Calculate relative movement of the line endpoints
	Point relStartVelocity = relativeVelocity + (start - end);
	Point relEndVelocity = relativeVelocity + (end - start);

	// Calculate the coefficients for the quadratic equation
	float radiusSquared = circle->GetRadius() * circle->GetRadius();

	// From start to circle center
	Point lineToCircleStart = circle->GetCenter() - start;

	float a = pnt::dot(relStartVelocity, relStartVelocity);
	float b = 2 * pnt::dot(relStartVelocity, lineToCircleStart);
	float c = pnt::dot(lineToCircleStart, lineToCircleStart) - radiusSquared;

	// Solve the quadratic equation
	if (!SolveQuadratic(a, b, c, tfirst, tlast))
		return false;  // No intersection

	// Ensure tfirst and tlast are within valid range [0, 1]
	tfirst = std::max(tfirst, 0.0f);
	tlast = std::min(tlast, 1.0f);

	if (tfirst > tlast) return false;

	// Check intersection for line endpoints
	for (float t = tfirst; t <= tlast; t += 0.01f)
	{
		Point currentStart = start + relStartVelocity * t;
		Point currentEnd = end + relEndVelocity * t;
		Point currentCircle = circle->GetCenter() + relativeVelocity * t;

		if (pnt::lengthSquared(currentCircle - currentStart) <= radiusSquared ||
			pnt::lengthSquared(currentCircle - currentEnd) <= radiusSquared) {
			return true;
		}
	}

	return true;
}

float GetXDist(const Point& p1, const Point& p2)
{
	return p2.x - p1.x;
}

float GetYDist(const Point& p1, const Point& p2)
{
	return p2.y - p1.y;
}

bool SolveQuadratic(float a, float b, float c, float& t0, float& t1)
{
	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return false;

	float sqrtDiscriminant = std::sqrt(discriminant);
	t0 = (-b - sqrtDiscriminant) / (2.0f * a);
	t1 = (-b + sqrtDiscriminant) / (2.0f * a);
	return true;
}

FloatRect::FloatRect(float left, float top, float width, float height)
	:m_left(left), m_top(top), m_width(width), m_height(height)
{
	m_min = { m_left, m_top };
	m_max = { m_left + m_width, m_top + m_height };
	m_midPoint = { (m_min.x + m_max.x) / 2.f,
	(m_min.y + m_max.y) / 2.f };
}