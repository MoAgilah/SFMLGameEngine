#pragma once

#include "../Interfaces/IBoundingVolume.h"

template<typename TDrawable>
class NBoundingBox : public NBoundingVolume<TDrawable>
{
public:
	NBoundingBox(const Point& size, const Point& pos)
		: NBoundingVolume<TDrawable>(NVolumeType::Box, std::make_unique<TDrawable>(size)),
		m_size(size)
	{
		Reset(size);
		this->Update(pos);
	}

	void Reset(const Point& size)
	{
		auto* rect = this->GetDrawable();
		if (rect)
		{
			rect->SetSize(size);
			auto scale = this->GetScale();
			m_extents = { size.x * 0.5f * scale.x, size.y * 0.5f * scale.y };
			this->SetOrigin(size * 0.5f);
		}
	}

	void Update(const Point& pos) override
	{
		this->SetPosition(pos);
		this->SetCenter(this->GetPosition());
		m_min = this->GetCenter() - m_extents;
		m_max = this->GetCenter() + m_extents;
	}

	float SqDistPoint(const Point& p)
	{
		float sqDist = 0.0f;

		for (size_t i = 0; i < 2; i++)
		{
			float v = p[i];
			if (v < m_min[i]) sqDist += (m_min[i] - v) * (m_min[i] - v);
			if (v > m_max[i]) sqDist += (v - m_max[i]) * (v - m_max[i]);
		}

		return sqDist;
	}

	bool Intersects(const Point& p) const override
	{
		if (pnt.x >= m_min.x &&
			pnt.x <= m_max.x &&
			pnt.y >= m_min.y &&
			pnt.y <= m_max.y)
			return true;

		return false;
	}

	bool Intersects(IBoundingVolume* other) override
	{
		return other->Intersects(this);
	}

	bool IntersectsMoving(IBoundingVolume* other, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		return other->IntersectsMoving(this, va, vb, tfirst, tlast);
	}

	Point GetSeparationVector(IBoundingVolume* other) override
	{
		return other->GetSeparationVector(this);
	}

	const Point& GetMin() const { return m_min; }
	const Point& GetMax() const { return m_max; }
	const Point& GetExtents() const { return m_extents; }

	Point GetPoint(NSide side) override
	{
		switch (side) {
		case NSide::Top:    return { this->GetCenter().x, m_min.y };
		case NSide::Bottom: return { this->GetCenter().x, m_max.y };
		case NSide::Left:   return { m_min.x, this->GetCenter().y };
		case NSide::Right:  return { m_max.x, this->GetCenter().y };
		}
		return {};
	}

	void SetScale(const Point& scale) override
	{
		NBoundingVolume<TDrawable>::SetScale(scale);
		Reset(NBoundingVolume<TDrawable>::GetSize());
	}

protected:
	// Double dispatch methods
	bool Intersects(NBoundingBox<TDrawable>* box) override
	{
		for (size_t i = 0; i < 2; i++)
		{
			if (std::abs(this->m_center[i] - box->m_center[i])
		> m_extents[i] + box->m_extents[i])
				return false;
		}

		// Overlapping on all axes means AABBs are intersecting
		return true;
	}

	bool Intersects(NBoundingCircle<TDrawable>* circle) override
	{
		// Compute squared distance between sphere center and AABB
		float sqDist = SqDistPoint(circle->GetCenter());
		float radius = circle->GetRadius();

		// Sphere and AABB intersect if the (squared) distance
		// between them is less than the (squared) sphere radius
		return sqDist <= radius * radius;
	}

	bool Intersects(NBoundingCapsule<TDrawable>* capsule) override
	{
		// Compute the box's min and max corners
		Point boxMin = m_min;
		Point boxMax = m_max;

		auto line = capsule->GetSegment();

		// Check the line segment (capsule core) against the box
		Point closestToStart = line.start.Clamp(boxMin, boxMax);
		Point closestToEnd = line.end.Clamp(boxMin, boxMax);

		float distStart = line.SqDistPointSegment(closestToStart);
		float distEnd = line.SqDistPointSegment(closestToEnd);

		float radSq = capsule->GetRadius() * capsule->GetRadius();

		// Check if the distances are less than or equal to the capsule's radius squared
		if (distStart <= radSq || distEnd <= radSq)
			return true;

		float closestPointStartDistSq = pnt::lengthSquared((closestToStart - line.start));
		float closestPointEndDistSq = pnt::lengthSquared((closestToEnd - line.end));

		return closestPointStartDistSq <= radSq || closestPointEndDistSq <= radSq;
	}

	bool IntersectsMoving(NBoundingBox<TDrawable>* box, const Point& vb, const Point& va, float& tfirst, float& tlast) override
	{
		if (Intersects(box))
		{
			tfirst = tlast = 0.0f;
			return true;
		}

		Point v = vb - va;
		if (std::abs(v.x) < CollisionManager::EPSILON && std::abs(v.y) < CollisionManager::EPSILON)
			return false;

		tfirst = 0.0f;
		tlast = 1.0f;

		for (int i = 0; i < 2; i++)
		{
			if (std::abs(v[i]) < CollisionManager::EPSILON)
			{
				if (box->m_max[i] < m_min[i] || box->m_min[i] > m_max[i])
					return false;

				continue;
			}

			if (v[i] < 0.0f)
			{
				if (box->m_max[i] < m_min[i] && v[i] <= 0.0f) return false;
				if (m_max[i] <= box->m_min[i]) tfirst = std::max((m_max[i] - box->m_min[i]) / v[i], tfirst);
				if (box->m_max[i] >= m_min[i]) tlast = std::min((m_min[i] - box->m_max[i]) / v[i], tlast);
			}
			else if (v[i] > 0.0f)
			{
				if (box->m_min[i] > m_max[i] && v[i] >= 0.0f) return false;
				if (box->m_max[i] <= m_min[i]) tfirst = std::max((m_min[i] - box->m_max[i]) / v[i], tfirst);
				if (m_max[i] >= box->m_min[i]) tlast = std::min((m_max[i] - box->m_min[i]) / v[i], tlast);
			}
		}

		if (tfirst > tlast) return false;
		return true;
	}

	bool IntersectsMoving(NBoundingCircle<TDrawable>* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		// Calculate relative velocity: circle's motion relative to the box
		Point relativeVelocity = vb - va;

		// No movement → fall back to static check
		if (pnt::lengthSquared(relativeVelocity) < CollisionManager::EPSILON * CollisionManager::EPSILON)
			return Intersects(circle);

		// Treat the circle as a moving point by expanding the box by the radius
		float r = circle->GetRadius();
		Point boxMin = GetMin() - Point(r, r);
		Point boxMax = GetMax() + Point(r, r);

		Point invVelocity = {
			std::abs(relativeVelocity.x) > CollisionManager::EPSILON ? 1.f / relativeVelocity.x : 0.f,
			std::abs(relativeVelocity.y) > CollisionManager::EPSILON ? 1.f / relativeVelocity.y : 0.f
		};

		float tEnterX = (boxMin.x - circle->GetPosition().x) * invVelocity.x;
		float tExitX = (boxMax.x - circle->GetPosition().x) * invVelocity.x;
		if (invVelocity.x < 0.f) std::swap(tEnterX, tExitX);

		float tEnterY = (boxMin.y - circle->GetPosition().y) * invVelocity.y;
		float tExitY = (boxMax.y - circle->GetPosition().y) * invVelocity.y;
		if (invVelocity.y < 0.f) std::swap(tEnterY, tExitY);

		float entryTime = std::max(tEnterX, tEnterY);
		float exitTime = std::min(tExitX, tExitY);

		// Reject if exit before entry, or exit is in the past, or entry is too far in future
		if (entryTime > exitTime || exitTime < -CollisionManager::EPSILON || entryTime > 1.0f)
			return false;

		// ✅ Allow t=0 contact as a valid collision
		tfirst = std::max(0.f, entryTime);
		tlast = std::min(1.f, exitTime);
		return true;
	}

	bool IntersectsMoving(NBoundingCapsule<TDrawable>* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast) override
	{
		return capsule->IntersectsMoving(this, va, vb, tfirst, tlast);
	}

	Point GetSeparationVector(NBoundingBox<TDrawable>* other) override
	{
		Point delta = other->GetPosition() - this->GetPosition();
		delta = { std::abs(delta.x), std::abs(delta.y) };
		Point overlap = (other->GetExtents() + m_extents) - delta;

		if (overlap.x < overlap.y)
		{
			float direction = (other->GetPosition().x < this->GetPosition().x) ? -1.0f : 1.0f;
			return { (overlap.x + CollisionManager::BUFFER) * direction, 0 };
		}
		else
		{
			float direction = (other->GetPosition().y < this->GetPosition().y) ? -1.0f : 1.0f;
			return { 0, (overlap.y + CollisionManager::BUFFER) * direction };
		}
	}

	Point GetSeparationVector(NBoundingCircle<TDrawable>* other) override
	{
		Point circlePos = other->GetPosition();
		Point closestPoint = {
			std::max(m_min.x, std::min(circlePos.x, m_max.x)),
			std::max(m_min.y, std::min(circlePos.y, m_max.y))
		};

		Point displacement = circlePos - closestPoint;
		float distance = pnt::length(displacement);
		float penetrationDepth = other->GetRadius() - distance;

		// If overlapping and distance is meaningful
		if (penetrationDepth > 0.f && distance > std::numeric_limits<float>::epsilon())
		{
			return pnt::Normalize(displacement) * (penetrationDepth + CollisionManager::BUFFER);
		}

		// If the circle's center is inside the box (distance ≈ 0), pick an arbitrary direction
		if (distance <= std::numeric_limits<float>::epsilon())
		{
			// Choose vertical push direction based on position relative to box center
			float pushDir = (circlePos.y < this->m_center.y) ? -1.f : 1.f;
			return Point(0.f, pushDir * (other->GetRadius() + CollisionManager::BUFFER));
		}

		// No collision
		return Point(0.f, 0.f);
	}

	Point GetSeparationVector(NBoundingCapsule<TDrawable>* other) override
	{
		Point closestPoint = other->GetSegment().ClosestPointOnLineSegment(this->GetCenter());
		Point clampedPoint = {
			std::max(m_min.x, std::min(closestPoint.x, m_max.x)),
			std::max(m_min.y, std::min(closestPoint.y, m_max.y))
		};

		Point displacement = closestPoint - clampedPoint;
		float distance = pnt::length(displacement);
		float penetrationDepth = other->GetRadius() - distance;

		if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
			return pnt::Normalize(displacement) * (penetrationDepth + CollisionManager::BUFFER);

		if (distance <= std::numeric_limits<float>::epsilon())
		{
			Point centerDelta = other->GetPosition() - this->GetCenter();
			if (std::abs(centerDelta.y) > std::abs(centerDelta.x))
				return { 0.f, (centerDelta.y > 0.f ? 1.f : -1.f) * (other->GetRadius() + CollisionManager::BUFFER) };
			else
				return { (centerDelta.x > 0.f ? 1.f : -1.f) * (other->GetRadius() + CollisionManager::BUFFER), 0.f };
		}

		return { 0.f, 0.f };
	}


private:
	Point m_size;
	Point m_min;
	Point m_max;
	Point m_extents;
};
