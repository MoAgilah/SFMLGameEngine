#pragma once

#include "NCollisionManager.h"
#include "../Interfaces/IShape.h"
#include "../Interfaces/IBoundingVolume.h"
#include "../../Utilities/Traits.h"

template <typename PlatformCircle>
class NBoundingCircle : public IBoundingCircle, public NBoundingVolume<PlatformCircle>
{
public:
    NBoundingCircle()
        : IBoundingVolume(NVolumeType::Circle)
        , IBoundingCircle()
        , NBoundingVolume<PlatformCircle>(NVolumeType::Circle)
    {
        this->m_shape = std::make_shared<PlatformCircle>();
    }

    NBoundingCircle(float radius, const Point& pos)
        : IBoundingVolume(NVolumeType::Circle)
        , IBoundingCircle()
        , NBoundingVolume<PlatformCircle>(NVolumeType::Circle)
    {
        this->m_shape = std::make_shared<PlatformCircle>();
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

    void Render(IRenderer* r) override { NBoundingVolume<PlatformCircle>::Render(r); }
    void* GetNativeShape() override { return NBoundingVolume<PlatformCircle>::GetNativeShape(); }

    Point GetCenter() const override { return NBoundingVolume<PlatformCircle>::GetCenter(); }
    void SetCenter(const Point& c) override { NBoundingVolume<PlatformCircle>::SetCenter(c); }

    Point GetPosition() const override { return NBoundingVolume<PlatformCircle>::GetPosition(); }
    void SetPosition(const Point& p) override { NBoundingVolume<PlatformCircle>::SetPosition(p); }

    Point GetOrigin() const override { return NBoundingVolume<PlatformCircle>::GetOrigin(); }
    void SetOrigin(const Point& o) override { NBoundingVolume<PlatformCircle>::SetOrigin(o); }

    Point GetScale() const override { return NBoundingVolume<PlatformCircle>::GetScale(); }
    void SetScale(const Point& scale) override
    {
        NBoundingVolume<PlatformCircle>::SetScale(scale);
        if (this->m_shape)
            Reset(this->m_shape->GetRadius());
    }

    float GetRadius() const override
    {
        ICircleShape* radiusShape = dynamic_cast<ICircleShape*>(this->m_shape.get());
        if (radiusShape)
            return radiusShape->GetRadius() * NBoundingVolume<PlatformCircle>::GetScale().x;
        return 0.f;
    }

    bool Intersects(const Point& pnt) const override
    {
        // get distance between the point and circle's center
        // using the Pythagorean Theorem
        Point dist = pnt - NBoundingVolume<PlatformCircle>::GetCenter();;

        float distance = pnt::length(dist);

        // if the distance is less than the circle's
        // radius the point is inside!
        return distance <= this->GetRadius();
    }

    Point GetPoint(NSide side) override
    {
        auto center = NBoundingVolume<PlatformCircle>::GetCenter();
        auto radius = GetRadius();
        switch (side) {
        case NSide::Left:   return center - Point(radius, 0);
        case NSide::Right:  return center + Point(radius, 0);
        case NSide::Top:    return center - Point(0, radius);
        case NSide::Bottom: return center + Point(0, radius);
        }
        return {};
    }

protected:

    bool Intersects(IBoundingBox* box) override
    {
        return box->Intersects(static_cast<IBoundingVolume*>(this));
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
        return box->IntersectsMoving(static_cast<IBoundingVolume*>(this), va, vb, tfirst, tlast);
    }

    bool IntersectsMoving(IBoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override
    {
        Point s = this->GetCenter() - circle->GetCenter();
        float r = circle->GetRadius() + this->GetRadius();

        Point v = vb - va; // Relative motion
        float a = pnt::dot(v, v);

        if (a < NCollisionManager::EPSILON) return false; // No relative motion

        float b = pnt::dot(v, s);
        if (b >= 0.0f) return false; // Moving away

        float c = pnt::dot(s, s) - r * r;
        if (c < -NCollisionManager::EPSILON) // Initial overlap case
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
        return capsule->IntersectsMoving(static_cast<IBoundingVolume*>(this), va, vb, tfirst, tlast);
    }

    Point GetSeparationVector(IBoundingBox* box) override
    {
        return box->GetSeparationVector(static_cast<IBoundingVolume*>(this));
    }

    Point GetSeparationVector(IBoundingCircle* other) override
    {
        Point displacement = other->GetPosition() - GetPosition();
        float distance = pnt::length(displacement);
        float radiusSum = other->GetRadius() + GetRadius();
        float penetrationDepth = radiusSum - distance;

        if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
            return pnt::Normalize(displacement) * (penetrationDepth + NCollisionManager::BUFFER);

        if (distance <= std::numeric_limits<float>::epsilon())
            return { 0.f, (other->GetPosition().y > GetPosition().y ? 1.f : -1.f) * (radiusSum + NCollisionManager::BUFFER) };

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
            return pnt::Normalize(displacement) * (penetrationDepth + NCollisionManager::BUFFER);

        if (distance <= std::numeric_limits<float>::epsilon())
            return { 0.f, (GetPosition().y > other->GetPosition().y ? 1.f : -1.f) * (radiusSum + NCollisionManager::BUFFER) };

        return Point();
    }
};