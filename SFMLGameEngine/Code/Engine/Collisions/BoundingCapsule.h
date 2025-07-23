#pragma once

#include "CollisionManager.h"
#include "../Interfaces/IBoundingVolume.h"
#include "../../Utilities/Traits.h"
#include <numbers>

// Forward declarations
template <typename PlatformBox>
class NBoundingBox;

template <typename PlatformCircle>
class NBoundingCircle;

template <typename PlatformShape>
class NBoundingCapsule : public IBoundingCapsule, public NBoundingVolume<PlatformShape>
{
public:
	using PlatformCircle = typename CapsuleTraits<PlatformShape>::CircleType;
	using PlatformBox = typename CapsuleTraits<PlatformShape>::BoxType;

	NBoundingCapsule(float radius, float length, float angle, const Point& pos)
		: IBoundingVolume(NVolumeType::Capsule)
		, IBoundingCapsule()
		, NBoundingVolume<PlatformShape>(NVolumeType::Capsule)
	{
		this->m_shape = std::make_shared<PlatformShape>();
		Reset(radius, length, angle);
		Update(pos);
	}

	void Reset(float radius, float length, float angle)
	{
		if (this->m_shape)
			this->m_shape->Reset(radius, length, angle);
	}

	void Update(const Point& pos) override
	{
		if (this->m_shape)
			this->m_shape->Update(pos);
	}

	void Render(IRenderer* renderer) override
	{
		if (this->m_shape)
			this->m_shape->Render(renderer);
	}

	bool Intersects(const Point& pnt) const override
	{
		auto clsPnt = m_segment.ClosestPointOnLineSegment(pnt);

		NBoundingCircle<PlatformCircle> circle(m_radius, clsPnt);

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

	Point GetPoint(NSide side) override
	{
		switch (side) {
		case NSide::Top:    return m_segment.start;
		case NSide::Bottom: return m_segment.end;
		case NSide::Left:   return this->m_center - Point(m_length * 0.5f, 0);
		case NSide::Right:  return this->m_center + Point(m_length * 0.5f, 0);
		}
		return this->m_center;
	}

	void SetScale(const Point& scale) override
	{
		if (this->m_shape)
			this->m_shape->SetScale(scale);

		Reset(m_radius, m_length, m_angle);
	}

protected:

	bool Intersects(IBoundingBox* box) override
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

	bool Intersects(IBoundingCircle* circle) override
	{
		float r = circle->GetRadius() + m_radius;

		float dist2 = m_segment.SqDistPointSegment(circle->GetCenter());

		return dist2 <= r * r;
	}

	bool Intersects(IBoundingCapsule* capsule) override
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

	bool IntersectsMoving(IBoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		NBoundingCircle<PlatformCircle> circle(m_radius, m_segment.start);
		if (circle.IntersectsMoving(box, va, vb, tfirst, tlast))
			return true;

		circle.Update(m_segment.end);
		if (circle.IntersectsMoving(box, va, vb, tfirst, tlast))
			return true;

		NBoundingBox<PlatformBox> capBox(this);
		return capBox.IntersectsMoving(box, va, vb, tfirst, tlast);
	}

	bool IntersectsMoving(IBoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		// check the capsule spherical ends
		NBoundingCircle<PlatformCircle> capCircle(circle->GetRadius(), m_segment.start);
		if (capCircle.IntersectsMoving(circle, va, vb, tfirst, tlast))
			return true;

		capCircle.Update(m_segment.end);

		if (capCircle.IntersectsMoving(circle, va, vb, tfirst, tlast))
			return true;

		capCircle.Update(m_segment.GetMidPoint());

		if (capCircle.IntersectsMoving(circle, va, vb, tfirst, tlast))
			return true;

		return m_segment.IntersectsMoving(circle, va, vb, tfirst, tlast);
		return false;
	}

	bool IntersectsMoving(IBoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		NBoundingCircle<PlatformCircle> endpoint1(m_radius, m_segment.start);
		NBoundingCircle<PlatformCircle> endpoint2(m_radius, m_segment.end);

		NBoundingCircle<PlatformCircle> otherEndpoint1(m_radius, m_segment.start);
		NBoundingCircle<PlatformCircle> otherEndpoint2(m_radius, m_segment.end);

		if (endpoint1.IntersectsMoving(&otherEndpoint2, va, vb, tfirst, tlast))
			return true;

		if (endpoint2.IntersectsMoving(&otherEndpoint1, va, vb, tfirst, tlast))
			return true;

		if (endpoint2.IntersectsMoving(&otherEndpoint2, va, vb, tfirst, tlast))
			return true;

		if (endpoint1.IntersectsMoving(&otherEndpoint1, va, vb, tfirst, tlast))
			return true;

		NBoundingBox<PlatformBox> box1 = NBoundingBox<PlatformBox>(this);
		NBoundingBox<PlatformBox> box2 = NBoundingBox<PlatformBox>(capsule);

		return box1.IntersectsMoving(&box2, va, vb, tfirst, tlast);
	}

	Point GetSeparationVector(IBoundingBox* other) override
	{
		return other->GetSeparationVector(this);
	}

	Point GetSeparationVector(IBoundingCircle* other) override
	{
		return other->GetSeparationVector(this);
	}

	Point GetSeparationVector(IBoundingCapsule* other) override
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

		return Point();
	}
};
