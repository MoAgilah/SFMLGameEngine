#pragma once

#include "CollisionManager.h"
#include "../Interfaces/IShape.h"
#include "../Interfaces/IBoundingVolume.h"

template <typename PlatformType>
class NBoundingCircle : public IBoundingCircle, public NBoundingVolume<PlatformType>
{
public:
    NBoundingCircle()
        : IBoundingVolume(NVolumeType::Circle)
        , IBoundingCircle()
        , NBoundingVolume<PlatformType>(NVolumeType::Circle)
    {
        this->m_shape = std::make_shared<PlatformType>();
    }

    NBoundingCircle(float radius, const Point& pos)
        : IBoundingVolume(NVolumeType::Circle)
        , IBoundingCircle()
        , NBoundingVolume<PlatformType>(NVolumeType::Circle)
    {
        this->m_shape = std::make_shared<PlatformType>();
        Reset(radius);
        Update(pos);
    }

    void Reset(float radius)
    {
        this->m_shape->Reset(radius);
    }

    void Update(const Point& pos) override
    {
        this->m_shape->Update(pos);
    }

    float GetRadius() const override
    {
        ICircleShape* radiusShape = dynamic_cast<ICircleShape*>(this->m_shape.get());
        if (radiusShape)
            return radiusShape->GetRadius() * this->GetScale().x;

        return 0.f;
    }

    void SetScale(const Point& scale) override
    {
        this->SetScale(scale);
        if (this->m_shape)
            Reset(this->m_shape->GetRadius());
    }

    bool Intersects(const Point& pnt) const override
    {
        // get distance between the point and circle's center
        // using the Pythagorean Theorem
        Point dist = pnt - this->GetCenter();

        float distance = pnt::length(dist);

        // if the distance is less than the circle's
        // radius the point is inside!
        return distance <= this->GetRadius();
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
        switch (side) {
        case NSide::Left:   return this->GetCenter() - Point(this->GetRadius(), 0);
        case NSide::Right:  return this->GetCenter() + Point(this->GetRadius(), 0);
        case NSide::Top:    return this->GetCenter() - Point(0, this->GetRadius());
        case NSide::Bottom: return this->GetCenter() + Point(0, this->GetRadius());
        }
        return {};
    }

protected:

    bool Intersects(IBoundingBox* box) override
    {
        return this->Intersects(box);
    }

    bool Intersects(IBoundingCircle* circle) override
    {
        // Calculate squared distance between centers
        Point d = this->GetCenter() - circle->GetCenter();
        float dist2 = pnt::dot(d, d);

        // Spheres intersect if squared distance is less than squared sum of radii
        float radiusSum = this->GetRadius() + circle->GetRadius();
        return dist2 <= radiusSum * radiusSum;
    }

    bool Intersects(IBoundingCapsule* capsule) override
    {
        float r = this->GetRadius() + capsule->GetRadius();

        float distSq = capsule->GetSegment().SqDistPointSegment(this->GetCenter());

        return distSq <= r * r;
    }

    bool IntersectsMoving(IBoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override
    {
        return this->IntersectsMoving(box, va, vb, tfirst, tlast);
    }

    bool IntersectsMoving(IBoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override
    {
        Point s = this->GetCenter() - circle->GetCenter();
        float r = circle->GetRadius() + this->GetRadius();

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

    bool IntersectsMoving(IBoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast) override
    {
        return this->IntersectsMoving(capsule, va, vb, tfirst, tlast);
    }

    Point GetSeparationVector(IBoundingBox* other) override
    {
        return this->GetSeparationVector(other);
    }

    Point GetSeparationVector(IBoundingCircle* other) override
    {
        Point displacement = other->GetPosition() - GetPosition();
        float distance = pnt::length(displacement);
        float radiusSum = other->GetRadius() + GetRadius();
        float penetrationDepth = radiusSum - distance;

        if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
            return pnt::Normalize(displacement) * (penetrationDepth + CollisionManager::BUFFER);

        if (distance <= std::numeric_limits<float>::epsilon())
            return { 0.f, (other->GetPosition().y > GetPosition().y ? 1.f : -1.f) * (radiusSum + CollisionManager::BUFFER) };

        return Point();
    }

    Point GetSeparationVector(IBoundingCapsule* other) override
    {
        Point closestPoint = other->GetSegment().ClosestPointOnLineSegment(GetPosition());
        Point displacement = GetPosition() - closestPoint;
        float distance = pnt::length(displacement);
        float radiusSum = GetRadius() + other->GetRadius();
        float penetrationDepth = radiusSum - distance;

        if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
            return pnt::Normalize(displacement) * (penetrationDepth + CollisionManager::BUFFER);

        if (distance <= std::numeric_limits<float>::epsilon())
            return { 0.f, (GetPosition().y > other->GetPosition().y ? 1.f : -1.f) * (radiusSum + CollisionManager::BUFFER) };

        return Point();
    }
};