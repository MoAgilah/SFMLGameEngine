#pragma once

#include "../Interfaces/IBoundingVolume.h"
#include "CollisionManager.h"
#include <numbers>

// Function to calculate the four corners of a rotated rectangle
void NCalculateRotatedRectangleCorners(Point corners[4], const Point& centre, const Point& size, float angle)
{
	// Convert the angle from degrees to radians
	float radians = angle * std::numbers::pi_v<float> / 180.0f;

	// Precompute sine and cosine of the angle
	float cosTheta = cos(radians);
	float sinTheta = sin(radians);

	// Half dimensions
	float halfWidth = size.x / 2.0f;
	float halfHeight = size.y / 2.0f;

	// Relative corners before rotation
	Point relativeCorners[4] =
	{
		{ -halfWidth, -halfHeight }, // Bottom-left
		{  halfWidth, -halfHeight }, // Bottom-right
		{  halfWidth,  halfHeight }, // Top-right
		{ -halfWidth,  halfHeight }  // Top-left
	};

	// Compute rotated corners
	for (int i = 0; i < 4; ++i)
	{
		corners[i].x = centre.x + relativeCorners[i].x * cosTheta - relativeCorners[i].y * sinTheta;
		corners[i].y = centre.y + relativeCorners[i].x * sinTheta + relativeCorners[i].y * cosTheta;
	}
}

template<typename IDrawable>
class NBoundingCapsule : public NBoundingVolume<IDrawable>
{
public:
	NBoundingCapsule(float radius, float length, float angle, const Point& pos)
		: NBoundingVolume<IDrawable>(NVolumeType::Capsule, std::make_unique<IDrawable>()),
		m_radius(radius), m_angle(angle)
	{
		Reset(m_radius, length, m_angle);

		Update(pos);
	}

	void Reset(float radius, float length, float angle)
	{
		const auto scale = this->GetScale();
		m_length = length * scale.y;
		m_radius = radius * scale.x;

		/*auto rect = GetDrawable();
		if (rect)
		{
			rect->setSize(Point(radius * 2.f, length));
			rect->setOrigin({ radius, length / 2.f });
			rect->setRotation(sf::degrees(angle));
		}

		m_circle1.setRadius(radius);
		m_circle1.setOrigin({ radius, radius });

		m_circle2.setRadius(radius);
		m_circle2.setOrigin({ radius, radius });*/
	}

	void Update(const Point& pos) override
	{
		this->SetPosition(pos);
		this->m_center = this->GetPosition();

		Point corners[4];
		Point size;
		//auto size = GetRect()->getSize();
		auto scale = this->GetScale();
		size.x *= scale.x;
		size.y *= scale.y;

		NCalculateRotatedRectangleCorners(corners, this->GetCenter(), size, m_angle);

		// Calculate the midpoint of the top side
		m_circle1.setPosition(Line(corners[3], corners[2]).GetMidPoint());
		// Calculate the midpoint of the top side
		m_circle2.setPosition(Line(corners[1], corners[0]).GetMidPoint());

		m_segment.start = m_circle1.getPosition();
		m_segment.end = m_circle2.getPosition();
	}

	bool Intersects(const Point& pnt) const override
	{
		auto clsPnt = m_segment.ClosestPointOnLineSegment(pnt);

		BoundingCircle circle(m_radius, clsPnt);

		return circle.Intersects(pnt);
	}

	bool Intersects(BoundingVolume* other)
	{
		return other->Intersects(this);
	}

	bool IntersectsMoving(BoundingVolume* other, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		return other->IntersectsMoving(this, va, vb, tfirst, tlast);
	}

	Point GetSeparationVector(BoundingVolume* other) override
	{
		return other->GetSeparationVector(this);
	}

	Point GetPoint(NSide side)
	{
		return Point();
	}

	float GetLength() const { return m_length; }
	float GetRadius() const { return m_radius; }
	Line& GetSegment() { return m_segment; }

	void SetScale(const Point& scale) override
	{
		BoundingVolume::SetScale(scale);
		m_circle1.setScale(scale);
		m_circle2.setScale(scale);
		//Reset(m_circle1.getRadius(), GetRect()->getSize().y, m_angle);
	}

protected:

	bool Intersects(BoundingBox* box) override
	{
		// Compute the box's min and max corners
		Point boxMin = box->GetMin();
		Point boxMax = box->GetMax();

		// Check the line segment (capsule core) against the box
		Point closestToStart = m_segment.start.Clamp(boxMin, boxMax);
		Point closestToEnd = m_segment.end.Clamp(boxMin, boxMax);

		float distStart = m_segment.SqDistPointSegment(closestToStart);
		float distEnd = m_segment.SqDistPointSegment(closestToEnd);

		float radSq = m_radius * m_radius;

		// Check if the distances are less than or equal to the capsule's radius squared
		if (distStart <= radSq || distEnd <= radSq)
			return true;

		float closestPointStartDistSq = pnt::lengthSquared((closestToStart - m_segment.start));
		float closestPointEndDistSq = pnt::lengthSquared((closestToEnd - m_segment.end));

		return closestPointStartDistSq <= radSq || closestPointEndDistSq <= radSq;
	}

	bool Intersects(BoundingCircle* circle) override
	{
		float r = circle->GetRadius() + m_radius;

		float dist2 = m_segment.SqDistPointSegment(circle->GetCenter());

		return dist2 <= r * r;
	}

	bool Intersects(BoundingCapsule* capsule) override
	{
		float combinedRadiusSquared = (m_radius + capsule->m_radius) * (m_radius + capsule->m_radius);

		// Compute the shortest distance squared between the two line segments
		float distanceSquared = std::min({
			capsule->m_segment.SqDistPointSegment(m_segment.start),
			capsule->m_segment.SqDistPointSegment(m_segment.end),
			m_segment.SqDistPointSegment(capsule->m_segment.start),
			m_segment.SqDistPointSegment(capsule->m_segment.end)
			});

		// Check if the distance is within the combined radii
		return distanceSquared <= combinedRadiusSquared;
	}

	bool IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		BoundingCircle circle(m_radius, m_segment.start);
		if (circle.IntersectsMoving(static_cast<BoundingVolume*>(box), va, vb, tfirst, tlast))
			return true;

		circle.Update(m_segment.end);
		if (circle.IntersectsMoving(static_cast<BoundingVolume*>(box), va, vb, tfirst, tlast))
			return true;

		BoundingBox capBox(this);
		return capBox.IntersectsMoving(static_cast<BoundingVolume*>(box), va, vb, tfirst, tlast);
	}

	bool IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		// check the capsule spherical ends
		BoundingCircle capCircle(circle->GetRadius(), m_segment.start);
		if (static_cast<BoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<BoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		capCircle.Update(m_segment.end);

		if (static_cast<BoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<BoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		capCircle.Update(m_segment.GetMidPoint());

		if (static_cast<BoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<BoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		return m_segment.IntersectsMoving(circle, va, vb, tfirst, tlast);
	}

	bool IntersectsMoving(BoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		BoundingCircle endpoint1(m_radius, m_segment.start);
		BoundingCircle endpoint2(m_radius, m_segment.end);

		BoundingCircle otherEndpoint1(m_radius, m_segment.start);
		BoundingCircle otherEndpoint2(m_radius, m_segment.end);

		if (endpoint1.IntersectsMoving(static_cast<BoundingVolume*>(&otherEndpoint2), va, vb, tfirst, tlast))
			return true;

		if (endpoint2.IntersectsMoving(static_cast<BoundingVolume*>(&otherEndpoint1), va, vb, tfirst, tlast))
			return true;

		if (endpoint2.IntersectsMoving(static_cast<BoundingVolume*>(&otherEndpoint2), va, vb, tfirst, tlast))
			return true;

		if (endpoint1.IntersectsMoving(static_cast<BoundingVolume*>(&otherEndpoint1), va, vb, tfirst, tlast))
			return true;

		BoundingBox box1 = BoundingBox(this);
		BoundingBox box2 = BoundingBox(capsule);

		return box1.IntersectsMoving(static_cast<BoundingVolume*>(&box2), va, vb, tfirst, tlast);
	}

	Point GetSeparationVector(BoundingBox* other) override
	{
		return other->GetSeparationVector(static_cast<BoundingVolume*>(this));
	}

	Point GetSeparationVector(BoundingCircle* other) override
	{
		return other->GetSeparationVector(static_cast<BoundingVolume*>(this));
	}

	Point GetSeparationVector(BoundingCapsule* other) override
	{
		Point closest1 = m_segment.ClosestPointOnLineSegment(other->GetSegment().start);
		Point closest2 = other->GetSegment().ClosestPointOnLineSegment(closest1);
		closest1 = m_segment.ClosestPointOnLineSegment(closest2);

		Point displacement = closest2 - closest1;
		float distance = pnt::length(displacement);
		float radiusSum = GetRadius() + other->GetRadius();
		float penetrationDepth = radiusSum - distance;

		if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
			return pnt::Normalize(displacement) * (penetrationDepth + CollisionManager::BUFFER);

		if (distance <= std::numeric_limits<float>::epsilon())
			return { 0.f, (other->GetPosition().y > this->GetPosition().y ? 1.f : -1.f) * (radiusSum + CollisionManager::BUFFER) };

		return { 0.f, 0.f };
	}

private:
	float m_angle;
	float m_radius;
	float m_length;
	Line m_segment;
	sf::CircleShape m_circle1;
	sf::CircleShape m_circle2;
};
