#pragma once

#include "IDrawable.h"
#include "../../Utilities/Utilities.h"
#include <memory>

enum class VolumeType { None, Box, Circle, Capsule };
enum class NSide { Left, Right, Top, Bottom };
enum class NDirection { LDIR, RDIR, UDIR, DDIR };

// Forward declarations
class IBoundingVolume;
class IBoundingBox;
class IBoundingCircle;
class IBoundingCapsule;

class IBoundingVolume
{
public:
	IBoundingVolume(VolumeType type)
		: m_type(type) {}

	virtual ~IBoundingVolume() = default;

    VolumeType GetType() const { return m_type; }

	virtual void Update(const Point& pos) = 0;
    virtual void Render(class IRenderer* renderer) = 0;

    virtual Point GetCenter() const = 0;
    virtual void SetCenter(const Point& center) = 0;

    virtual Point GetPosition() const = 0;
    virtual void SetPosition(const Point& pos) = 0;

    virtual Point GetOrigin() const = 0;
    virtual void SetOrigin(const Point& scl) = 0;

    virtual Point GetScale() const = 0;
    virtual void SetScale(const Point& scl) = 0;

    virtual void* GetNativeShape() = 0;

	virtual bool Intersects(const Point& pnt) const = 0;
	virtual bool Intersects(IBoundingVolume* volume) = 0;
	virtual bool IntersectsMoving(IBoundingVolume* volume, const Point& va, const Point& vb, float& tfirst, float& tlast) = 0;
	virtual Point GetSeparationVector(IBoundingVolume* volume) = 0;
	virtual Point GetPoint(NSide side) = 0;

protected:
	// Double dispatch methods
	virtual bool Intersects(IBoundingBox* box) = 0;
	virtual bool Intersects(IBoundingCircle* circle) = 0;
	virtual bool Intersects(IBoundingCapsule* capsule) = 0;

	virtual bool IntersectsMoving(IBoundingBox* box, const Point&, const Point&, float&, float&) = 0;
	virtual bool IntersectsMoving(IBoundingCircle* circle, const Point&, const Point&, float&, float&) = 0;
	virtual bool IntersectsMoving(IBoundingCapsule* capsule, const Point&, const Point&, float&, float&) = 0;

	virtual Point GetSeparationVector(IBoundingBox* other) = 0;
	virtual Point GetSeparationVector(IBoundingCircle* other) = 0;
	virtual Point GetSeparationVector(IBoundingCapsule* other) = 0;

protected:

	VolumeType m_type;
};

template <typename PlatformType>
class BoundingVolume : public virtual IBoundingVolume
{
public:
    explicit BoundingVolume(VolumeType type) : IBoundingVolume(type) {}
    virtual ~BoundingVolume() = default;

    void Render(IRenderer* renderer) override
    {
        if (m_shape)
            m_shape->Render(renderer);
    }

    Point GetCenter() const override
    {
        return m_shape ? m_shape->GetCenter() : Point();
    }

    void SetCenter(const Point& center) override
    {
        if (m_shape)
            m_shape->SetCenter(center);
    }

    // Commonly shared methods
    Point GetPosition() const override
    {
        return m_shape ? m_shape->GetPosition() : Point();
    }

    void SetPosition(const Point& pos) override
    {
        if (m_shape)
            m_shape->SetPosition(pos);
    }

    Point GetOrigin() const override
    {
        return m_shape ? m_shape->GetOrigin() : Point();
    }

    void SetOrigin(const Point& origin) override
    {
        if (m_shape)
            m_shape->SetOrigin(origin);
    }

    Point GetScale() const override
    {
        return m_shape ? m_shape->GetScale() : Point();
    }

    void SetScale(const Point& scl) override
    {
        if (m_shape)
            m_shape->SetScale(scl);
    }

    void* GetNativeShape() override { return static_cast<void*>(m_shape.get()); }

    PlatformType* GetShape() { return m_shape.get(); }

protected:

    std::shared_ptr<PlatformType> m_shape = nullptr;
};

class IBoundingBox : public virtual IBoundingVolume {
public:
    IBoundingBox() : IBoundingVolume(VolumeType::Box) {}
    virtual ~IBoundingBox() = default;

    virtual const Point& GetMin() const { return m_min; }
    virtual const Point& GetMax() const { return m_max; }
    virtual const Point& GetExtents() const { return m_extents; }
    virtual Line GetSide(NSide side) = 0;

protected:

    Point m_min;
    Point m_max;
    Point m_extents;
};

class IBoundingCircle : public virtual IBoundingVolume {
public:
    IBoundingCircle() : IBoundingVolume(VolumeType::Circle) {}
    virtual ~IBoundingCircle() = default;

    virtual float GetRadius() const = 0;
};

class IBoundingCapsule : public virtual IBoundingVolume {
public:
    IBoundingCapsule() : IBoundingVolume(VolumeType::Capsule) {}
    virtual ~IBoundingCapsule() = default;

    virtual float GetRadius() const = 0;
    virtual float GetLength() const = 0;
    virtual float GetAngle()  const = 0;
    virtual const Line& GetSegment() const = 0;
};
