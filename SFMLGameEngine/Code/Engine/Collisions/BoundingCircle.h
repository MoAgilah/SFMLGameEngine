#pragma once

#include "../Interfaces/IBoundingVolume.h"
#include "CollisionManager.h"

template<typename TDrawable>
class NBoundingCircle : public NBoundingVolume<TDrawable>
{
public:
    NBoundingCircle(float radius, const Point& pos)
        : NBoundingVolume<TDrawable>(NVolumeType::Circle, std::make_unique<TDrawable>(radius)),
        m_radius(radius)
    {
        Reset(radius);
        this->Update(pos);
    }

    void Reset(float radius)
    {
        auto* circle = this->GetDrawable();
        if (circle)
        {
            m_radius = radius * this->GetScale().x;
            this->SetRadius(radius);
            this->SetOrigin(Point(radius, radius));
        }
    }

    void Update(const Point& pos) override
    {
        this->SetPosition(pos);
        this->SetCenter(this->GetPosition());
    }

    bool Intersects(const Point& pnt) const override
    {
        // get distance between the point and circle's center
        // using the Pythagorean Theorem
        Point dist = pnt - this->GetCenter();

        float distance = pnt::length(dist);

        // if the distance is less than the circle's
        // radius the point is inside!
        return distance <= m_radius;
    }

    bool Intersects(IBoundingVolume* other) override
    {
        return other->Intersects(this);
    }

    bool IntersectsMoving(BoundingVolume* other, const Point& va, const Point& vb, float& tfirst, float& tlast) override
    {
        return other->IntersectsMoving(this, va, vb, tfirst, tlast);
    }

    Point GetSeparationVector(IBoundingVolume* other) override
    {
        return other->GetSeparationVector(this);
    }

    Point GetPoint(NSide side) override
    {
        switch (side) {
        case NSide::Left:   return m_center - Point(m_radius, 0);
        case NSide::Right:  return m_center + Point(m_radius, 0);
        case NSide::Top:    return m_center - Point(0, m_radius);
        case NSide::Bottom: return m_center + Point(0, m_radius);
        }
        return {};
    }

    float GetRadius() const { return m_radius; }

    void SetScale(const Point& scale) override
    {
        NBoundingVolume<TDrawable>::SetScale(scale);
        Reset(GetRadius());
    }

protected:
    // Double dispatch methods
    bool Intersects(NBoundingBox<TDrawable>* box) override
    {
        return box->Intersects(this);
    }

    bool Intersects(NBoundingCircle<TDrawable>* circle) override
    {
        // Calculate squared distance between centers
        Point d = this->GetCenter() - circle->GetCenter();
        float dist2 = pnt::dot(d, d);

        // Spheres intersect if squared distance is less than squared sum of radii
        float radiusSum = m_radius + circle->m_radius;
        return dist2 <= radiusSum * radiusSum;
    }

    bool Intersects(NBoundingCapsule<TDrawable>* capsule) override
    {
        float r = m_radius + capsule->GetRadius();

        float distSq = capsule->GetSegment().SqDistPointSegment(m_center);

        return distSq <= r * r;
    }

    bool IntersectsMoving(NBoundingBox<TDrawable>* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override
    {
        return box->IntersectsMoving(this, va, vb, tfirst, tlast);
    }

    bool IntersectsMoving(NBoundingCircle<TDrawable>* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override
    {
        Point s = this->GetCenter() - circle->GetCenter();
        float r = circle->m_radius + m_radius;

        Point v = vb - va; // Relative motion
        float a = pnt::dot(v, v);

        if (a < CollisionManager::EPSILON) return false; // No relative motion

        float b = pnt::dot(v, s);
        if (b >= 0.0f) return false; // Moving away

        float c = pnt::dot(s, s) - r * r;
        if (c < -CollisionManager::EPSILON) // Initial overlap case
        {
            tfirst = tlast = 0.0f;
            return true;
        }

        float d = b * b - a * c;
        if (d < 0.0f) return false; // No real solution

        float sqrtD = std::sqrt(d);
        tfirst = (-b - sqrtD) / a;
        tlast = (-b + sqrtD) / a;

        if (tlast < 0.0f || tfirst > 1.0f) return false;
        if (tfirst > tlast) return false;

        // If tfirst is negative, clamp to zero
        if (tfirst < 0.0f && tlast >= 0.0f)
            tfirst = 0.0f;

        return true;
    }

    bool IntersectsMoving(NBoundingCapsule<TDrawable>* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast) override
    {
        return capsule->IntersectsMoving(this, va, vb, tfirst, tlast);
    }

    Point GetSeparationVector(NBoundingBox<TDrawable>* other) override
    {
        return other->GetSeparationVector(static_cast<BoundingVolume*>(this));
    }

    Point GetSeparationVector(NBoundingCircle<TDrawable>* other) override
    {
        Point displacement = other->GetPosition() - this->GetPosition();
        float distance = pnt::length(displacement);
        float radiusSum = other->GetRadius() + GetRadius();
        float penetrationDepth = radiusSum - distance;

        if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
            return pnt::Normalize(displacement) * (penetrationDepth + CollisionManager::BUFFER);

        if (distance <= std::numeric_limits<float>::epsilon())
            return { 0.f, (other->GetPosition().y > this->GetPosition().y ? 1.f : -1.f) * (radiusSum + CollisionManager::BUFFER) };

        return { 0.f, 0.f };
    }

    Point GetSeparationVector(NBoundingCapsule<TDrawable>* other) override
    {
        Point closestPoint = other->GetSegment().ClosestPointOnLineSegment(this->GetPosition());
        Point displacement = this->GetPosition() - closestPoint;
        float distance = pnt::length(displacement);
        float radiusSum = GetRadius() + other->GetRadius();
        float penetrationDepth = radiusSum - distance;

        if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
            return pnt::Normalize(displacement) * (penetrationDepth + CollisionManager::BUFFER);

        if (distance <= std::numeric_limits<float>::epsilon())
            return { 0.f, (this->GetPosition().y > other->GetPosition().y ? 1.f : -1.f) * (radiusSum + CollisionManager::BUFFER) };

        return { 0.f, 0.f };
    }

private:
    float m_radius;
    Point m_center;
    std::unique_ptr<TDrawable> m_drawable;
};
