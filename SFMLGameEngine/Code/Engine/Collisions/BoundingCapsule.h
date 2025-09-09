#pragma once

#include "BoundingBox.h"
#include "BoundingCircle.h"
#include "NCollisionManager.h"
#include "../Interfaces/IShape.h"
#include "../Interfaces/IBoundingVolume.h"
#include "../../Utilities/Traits.h"
#include <numbers>
#include <algorithm>

template <typename PlatformCapsule>
class NBoundingCapsule : public IBoundingCapsule, public NBoundingVolume<PlatformCapsule>
{
public:
	using PlatformCircle = typename CapsuleTraits<PlatformCapsule>::CircleType;
	using PlatformBox = typename CapsuleTraits<PlatformCapsule>::BoxType;

	NBoundingCapsule(float radius, float length, float angle, const Point& pos)
		: IBoundingVolume(NVolumeType::Capsule)
		, IBoundingCapsule()
		, NBoundingVolume<PlatformCapsule>(NVolumeType::Capsule)
	{
		this->m_shape = std::make_shared<PlatformCapsule>();
		Reset(radius, length, angle);
		Update(pos);
	}

	NBoundingCapsule(float radius, const Line& segment)
		: IBoundingVolume(NVolumeType::Capsule)
		, IBoundingCapsule()
		, NBoundingVolume<PlatformCapsule>(NVolumeType::Capsule)
	{
		this->m_shape = std::make_shared<PlatformCapsule>();
		Reset(radius, pnt::distance(segment.start, segment.end), segment.CalculateAngle());
		Update(segment.GetMidPoint());
	}

	NBoundingBox<PlatformBox> ToBoundingBox() const
	{
		NBoundingBox<PlatformBox> out;
		const float r = GetRadius();
		const auto& seg = GetSegment();

		const float minX = std::min(seg.start.x, seg.end.x) - r;
		const float maxX = std::max(seg.start.x, seg.end.x) + r;
		const float minY = std::min(seg.start.y, seg.end.y) - r;
		const float maxY = std::max(seg.start.y, seg.end.y) + r;

		out.Reset({ maxX - minX, maxY - minY });
		out.Update(seg.GetMidPoint());
		return out;
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

	void* GetNativeShape() override { return NBoundingVolume<PlatformCapsule>::GetNativeShape(); }

	Point GetCenter() const override { return NBoundingVolume<PlatformCapsule>::GetCenter(); }
	void SetCenter(const Point& c) override { NBoundingVolume<PlatformCapsule>::SetCenter(c); }

	Point GetPosition() const override { return NBoundingVolume<PlatformCapsule>::GetPosition(); }
	void SetPosition(const Point& p) override { NBoundingVolume<PlatformCapsule>::SetPosition(p); }

	Point GetOrigin() const override { return NBoundingVolume<PlatformCapsule>::GetOrigin(); }
	void SetOrigin(const Point& o) override { NBoundingVolume<PlatformCapsule>::SetOrigin(o); }

	Point GetScale() const override { return NBoundingVolume<PlatformCapsule>::GetScale(); }
	void SetScale(const Point& scale) override
	{
		NBoundingVolume<PlatformCapsule>::SetScale(scale);
		if (this->m_shape)
			Reset(GetRadius(), GetLength(), GetAngle());
	}

	float GetRadius() const override
	{
		if (this->m_shape)
			return this->m_shape->GetRadius();

		return 0.f;
	}

	float GetLength() const override
	{
		if (this->m_shape)
			return this->m_shape->GetLength();

		return 0.f;
	}

	float GetAngle()  const override
	{
		if (this->m_shape)
			return this->m_shape->GetAngle();

		return 0.f;
	}

	const Line& GetSegment() const override
	{
		if (this->m_shape)
			return this->m_shape->GetSegment();

		return Line();
	}

	bool Intersects(const Point& pnt) const override
	{
		auto clsPnt = GetSegment().ClosestPointOnLineSegment(pnt);

		NBoundingCircle<PlatformCircle> circle{ GetRadius(), clsPnt };

		return circle.Intersects(pnt);
	}

	Point GetPoint(NSide side) override
	{
		auto center = NBoundingVolume<PlatformCapsule>::GetCenter();
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
		Point closestToStart = GetSegment().start.Clamp(boxMin, boxMax);
		Point closestToEnd = GetSegment().end.Clamp(boxMin, boxMax);

		float distStart = GetSegment().SqDistPointSegment(closestToStart);
		float distEnd = GetSegment().SqDistPointSegment(closestToEnd);

		float radSq = this->GetRadius() * this->GetRadius();

		// Check if the distances are less than or equal to the capsule's radius squared
		if (distStart <= radSq || distEnd <= radSq)
			return true;

		float closestPointStartDistSq = pnt::lengthSquared((closestToStart - GetSegment().start));
		float closestPointEndDistSq = pnt::lengthSquared((closestToEnd - GetSegment().end));

		return closestPointStartDistSq <= radSq || closestPointEndDistSq <= radSq;
	}

	bool Intersects(IBoundingCircle* circle) override
	{
		float r = circle->GetRadius() + this->GetRadius();

		float dist2 = GetSegment().SqDistPointSegment(circle->GetCenter());

		return dist2 <= r * r;
	}

	bool Intersects(IBoundingCapsule* capsule) override
	{
		float combinedRadiusSquared = (this->GetRadius() + capsule->GetRadius()) * (this->GetRadius() + capsule->GetRadius());

		// Compute the shortest distance squared between the two line segments
		float a = capsule->GetSegment().SqDistPointSegment(this->GetSegment().start);
		float b = capsule->GetSegment().SqDistPointSegment(this->GetSegment().end);
		float c = GetSegment().SqDistPointSegment(capsule->GetSegment().start);
		float d = GetSegment().SqDistPointSegment(capsule->GetSegment().end);

		float distanceSquared = std::min<float>(std::min<float>(a, b), std::min<float>(c, d));

		// Check if the distance is within the combined radii
		return distanceSquared <= combinedRadiusSquared;
	}

	bool IntersectsMoving(IBoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		NBoundingCircle<PlatformCircle> circle{ GetRadius(), GetSegment().start };
		if (static_cast<IBoundingVolume*>(&circle)->IntersectsMoving(static_cast<IBoundingVolume*>(box), va, vb, tfirst, tlast))
			return true;

		circle.Update(GetSegment().end);

		if (static_cast<IBoundingVolume*>(&circle)->IntersectsMoving(static_cast<IBoundingVolume*>(box), va, vb, tfirst, tlast))
			return true;

		auto capBox = this->ToBoundingBox();
		return static_cast<IBoundingVolume*>(&capBox)->IntersectsMoving(static_cast<IBoundingVolume*>(box), va, vb, tfirst, tlast);
	}

	bool IntersectsMoving(IBoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		// check the capsule spherical ends
		NBoundingCircle<PlatformCircle> capCircle{ circle->GetRadius(), GetSegment().start };

		IBoundingVolume* cc = &capCircle;

		if (cc->IntersectsMoving(static_cast<IBoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		capCircle.Update(GetSegment().end);

		if (cc->IntersectsMoving(static_cast<IBoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		capCircle.Update(GetSegment().GetMidPoint());

		if (cc->IntersectsMoving(static_cast<IBoundingVolume*>(circle), va, vb, tfirst, tlast))
			return true;

		return GetSegment().IntersectsMoving(circle, va, vb, tfirst, tlast);
	}

	bool IntersectsMoving(IBoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		NBoundingCircle<PlatformCircle> endpoint1{ GetRadius(), GetSegment().start };
		NBoundingCircle<PlatformCircle> endpoint2{ GetRadius(), GetSegment().end };

		NBoundingCircle<PlatformCircle> otherEndpoint1{ capsule->GetRadius(), capsule->GetSegment().start };
		NBoundingCircle<PlatformCircle> otherEndpoint2{ capsule->GetRadius(), capsule->GetSegment().end };

		IBoundingVolume* e1 = &endpoint1;
		IBoundingVolume* e2 = &endpoint2;
		IBoundingVolume* o1 = &otherEndpoint1;
		IBoundingVolume* o2 = &otherEndpoint2;

		if (e1->IntersectsMoving(o2, va, vb, tfirst, tlast))
			return true;

		if (e2->IntersectsMoving(o1, va, vb, tfirst, tlast))
			return true;

		if (e2->IntersectsMoving(o2, va, vb, tfirst, tlast))
			return true;

		if (e1->IntersectsMoving(o1, va, vb, tfirst, tlast))
			return true;

		NBoundingBox<PlatformBox> box1 = this->ToBoundingBox();
		NBoundingBox<PlatformBox> box2 = static_cast<NBoundingCapsule<PlatformCapsule>*>(capsule)->ToBoundingBox();

		IBoundingVolume* b1 = &box1;
		IBoundingVolume* b2 = &box2;

		return b1->IntersectsMoving(b2, va, vb, tfirst, tlast);
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
		Point closest1 = GetSegment().ClosestPointOnLineSegment(other->GetSegment().start);
		Point closest2 = other->GetSegment().ClosestPointOnLineSegment(closest1);
		closest1 = GetSegment().ClosestPointOnLineSegment(closest2);

		Point displacement = closest2 - closest1;
		float distance = pnt::length(displacement);
		float radiusSum = GetRadius() + other->GetRadius();
		float penetrationDepth = radiusSum - distance;

		if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
			return pnt::Normalize(displacement) * (penetrationDepth + NCollisionManager::BUFFER);

		if (distance <= std::numeric_limits<float>::epsilon())
			return { 0.f, (other->GetPosition().y > this->GetPosition().y ? 1.f : -1.f) * (radiusSum + NCollisionManager::BUFFER) };

		return Point();
	}
};