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

template <typename PlatformCapsule>
class NBoundingCapsule;

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

	NBoundingCapsule(float radius, const Line& segment)
		: IBoundingVolume(NVolumeType::Capsule)
		, IBoundingCapsule()
		, NBoundingVolume<PlatformShape>(NVolumeType::Capsule)
	{
		this->m_shape = std::make_shared<PlatformShape>();
		Reset(m_radius, pnt::distance(segment.start, segment.end), m_angle);
		Update(segment.GetMidPoint());
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

	void* GetNativeShape() override { return NBoundingVolume<PlatformShape>::GetNativeShape(); }

	Point GetCenter() const override { return NBoundingVolume<PlatformShape>::GetCenter(); }
	void SetCenter(const Point& c) override { NBoundingVolume<PlatformShape>::SetCenter(c); }

	Point GetPosition() const override { return NBoundingVolume<PlatformShape>::GetPosition(); }
	void SetPosition(const Point& p) override { NBoundingVolume<PlatformShape>::SetPosition(p); }

	Point GetOrigin() const override { return NBoundingVolume<PlatformShape>::GetOrigin(); }
	void SetOrigin(const Point& o) override { NBoundingVolume<PlatformShape>::SetOrigin(o); }

	Point GetScale() const override { return NBoundingVolume<PlatformShape>::GetScale(); }
	void SetScale(const Point& scale) override
	{
		NBoundingVolume<PlatformShape>::SetScale(scale);
		if (this->m_shape)
			Reset(m_radius, m_length, m_angle);
	}

	bool Intersects(const Point& pnt) const override
	{
		auto clsPnt = m_segment.ClosestPointOnLineSegment(pnt);

		NBoundingCircle<SFCircle> circle(m_radius, clsPnt);

		return circle.Intersects(pnt);
	}

	bool Intersects(IBoundingVolume* other) override
	{
		return this->Intersects(other);
	}

	bool IntersectsMoving(IBoundingVolume* other, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		return this->IntersectsMoving(other, va, vb, tfirst, tlast);
	}

	Point GetSeparationVector(IBoundingVolume* other) override
	{
		return this->GetSeparationVector(other);
	}
	Point GetPoint(NSide side) override
	{
		auto center = NBoundingVolume<PlatformShape>::GetCenter();
		switch (side) {
		case NSide::Top:    return this->GetSegment().start;
		case NSide::Bottom: return this->GetSegment().end;
		case NSide::Left:   return center - Point(this->GetLength() * 0.5f, 0);
		case NSide::Right:  return center + Point(this->GetLength() * 0.5f, 0);
		}
		return center;
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

		float radSq = this->GetRadius() * this->GetRadius();

		// Check if the distances are less than or equal to the capsule's radius squared
		if (distStart <= radSq || distEnd <= radSq)
			return true;

		float closestPointStartDistSq = pnt::lengthSquared((closestToStart - m_segment.start));
		float closestPointEndDistSq = pnt::lengthSquared((closestToEnd - m_segment.end));

		return closestPointStartDistSq <= radSq || closestPointEndDistSq <= radSq;
	}

	bool Intersects(IBoundingCircle* circle) override
	{
		float r = circle->GetRadius() + this->GetRadius();

		float dist2 = m_segment.SqDistPointSegment(circle->GetCenter());

		return dist2 <= r * r;
	}

	bool Intersects(IBoundingCapsule* capsule) override
	{
		float combinedRadiusSquared = (this->GetRadius() + capsule->GetRadius()) * (this->GetRadius() + capsule->GetRadius());

		// Compute the shortest distance squared between the two line segments
		float distanceSquared = std::min({
			capsule->GetSegment().SqDistPointSegment(this->GetSegment().start),
			capsule->GetSegment().SqDistPointSegment(this->GetSegment().end),
			GetSegment().SqDistPointSegment(capsule->GetSegment().start),
			GetSegment().SqDistPointSegment(capsule->GetSegment().end)
			});

		// Check if the distance is within the combined radii
		return distanceSquared <= combinedRadiusSquared;
	}

	bool IntersectsMoving(IBoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		NBoundingCircle<PlatformCircle> circle(m_radius, m_segment.start);
		if (static_cast<IBoundingVolume*>(&circle)->IntersectsMoving(static_cast<IBoundingVolume*>(box), va, vb, tfirst, tlast))
			return true;

		circle.Update(m_segment.end);
		if (static_cast<IBoundingVolume*>(&circle)->IntersectsMoving(static_cast<IBoundingVolume*>(box), va, vb, tfirst, tlast))
			return true;

		NBoundingBox<PlatformBox> capBox(this);
		return static_cast<IBoundingVolume*>(&capBox)->IntersectsMoving(static_cast<IBoundingVolume*>(box), va, vb, tfirst, tlast);
	}

	bool IntersectsMoving(IBoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		// check the capsule spherical ends
		NBoundingCircle<PlatformCircle> capCircle(circle->GetRadius(), m_segment.start);
		if (static_cast<IBoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<IBoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		capCircle.Update(m_segment.end);

		if (static_cast<IBoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<IBoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		capCircle.Update(m_segment.GetMidPoint());

		if (static_cast<IBoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<IBoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		return m_segment.IntersectsMoving(circle, va, vb, tfirst, tlast);
	}

	bool IntersectsMoving(IBoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		NBoundingCircle<PlatformCircle> endpoint1(m_radius, m_segment.start);
		NBoundingCircle<PlatformCircle> endpoint2(m_radius, m_segment.end);

		NBoundingCircle<PlatformCircle> otherEndpoint1(m_radius, m_segment.start);
		NBoundingCircle<PlatformCircle> otherEndpoint2(m_radius, m_segment.end);

		if (static_cast<IBoundingVolume*>(&endpoint1)->IntersectsMoving(static_cast<IBoundingVolume*>(&otherEndpoint2), va, vb, tfirst, tlast))
			return true;

		if (static_cast<IBoundingVolume*>(&endpoint2)->IntersectsMoving(static_cast<IBoundingVolume*>(&otherEndpoint1), va, vb, tfirst, tlast))
			return true;

		if (static_cast<IBoundingVolume*>(&endpoint2)->IntersectsMoving(static_cast<IBoundingVolume*>(&otherEndpoint2), va, vb, tfirst, tlast))
			return true;

		if (static_cast<IBoundingVolume*>(&endpoint1)->IntersectsMoving(static_cast<IBoundingVolume*>(&otherEndpoint1), va, vb, tfirst, tlast))
			return true;

		NBoundingBox<PlatformBox> box1 = NBoundingBox<PlatformBox>(this);
		NBoundingBox<PlatformBox> box2 = NBoundingBox<PlatformBox>(static_cast<NBoundingCapsule<PlatformShape>*>(capsule));

		return static_cast<IBoundingVolume*>(&box1)->IntersectsMoving(static_cast<IBoundingVolume*>(&box2), va, vb, tfirst, tlast);
	}

	Point GetSeparationVector(IBoundingBox* box) override
	{
		return box->GetSeparationVector(static_cast<IBoundingVolume*>(this));
	}

	Point GetSeparationVector(IBoundingCircle* circle) override
	{
		return circle->GetSeparationVector(static_cast<IBoundingVolume*>(this));
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
