#pragma once

#include "IDrawable.h"
#include "../../Utilities/Utilities.h"
#include <memory>

enum class NVolumeType { None, Box, Circle, Capsule };
enum class NSide { Left, Right, Top, Bottom };
enum class NDirection { LDIR, RDIR, UDIR, DDIR };

// Forward declarations
class IBoundingVolume;

template<typename TDrawable> class NBoundingBox;
template<typename TDrawable> class NBoundingCircle;
template<typename TDrawable> class NBoundingCapsule;

class IBoundingVolume
{
public:
	IBoundingVolume(NVolumeType type)
		: m_type(type) {}

	virtual ~IBoundingVolume() = default;

	virtual void Update(const Point& pos) = 0;

	virtual bool Intersects(const Point& pnt) const = 0;
	virtual bool Intersects(IBoundingVolume* volume) = 0;
	virtual bool IntersectsMoving(IBoundingVolume* volume, const Point& va, const Point& vb, float& tfirst, float& tlast) = 0;
	virtual Point GetSeparationVector(IBoundingVolume* volume) = 0;
	virtual Point GetPoint(NSide side) = 0;

	NVolumeType GetType() const { return m_type; }

protected:
	// Double dispatch methods
	virtual bool Intersects(NBoundingBox<IDrawable>* box) = 0;
	virtual bool Intersects(NBoundingCircle<IDrawable>* circle) = 0;
	virtual bool Intersects(NBoundingCapsule<IDrawable>* capsule) = 0;

	virtual bool IntersectsMoving(NBoundingBox<IDrawable>* box, const Point&, const Point&, float&, float&) = 0;
	virtual bool IntersectsMoving(NBoundingCircle<IDrawable>* circle, const Point&, const Point&, float&, float&) = 0;
	virtual bool IntersectsMoving(NBoundingCapsule<IDrawable>* capsule, const Point&, const Point&, float&, float&) = 0;

	virtual Point GetSeparationVector(NBoundingBox<IDrawable>* other) = 0;
	virtual Point GetSeparationVector(NBoundingCircle<IDrawable>* other) = 0;
	virtual Point GetSeparationVector(NBoundingCapsule<IDrawable>* other) = 0;

protected:

	NVolumeType m_type;
};

// Templated BoundingVolume with platform-specific drawable
template<typename TDrawable>
class NBoundingVolume : public IBoundingVolume, public IDrawableNoUpdate {
public:
	explicit NBoundingVolume(NVolumeType type, std::unique_ptr<TDrawable> drawable)
		: IBoundingVolume(type), m_drawable(std::move(drawable))
	{
		ConfigureDrawable();
	}

	// --- IDrawableNoUpdate interface ---
	void Render(IRenderer* renderer) override
	{
		if (m_drawable)
			m_drawable->Render(renderer);
	}

	void SetPosition(const Point& pos) override
	{
		if (m_drawable)
			m_drawable->SetPosition(pos);
	}

	Point GetPosition() override
	{
		return m_drawable ? m_drawable->GetPosition() : Point();
	}

	void SetScale(const Point& scale) override
	{
		m_scale = scale;
		if (m_drawable)
			m_drawable->SetScale(scale);
	}

	Point GetScale() override
	{
		return m_drawable ? m_drawable->GetScale() : m_scale;
	}

	void SetOrigin(const Point& origin) override
	{
		if (m_drawable)
			m_drawable->SetOrigin(origin);
	}

	Point GetOrigin() override
	{
		return m_drawable ? m_drawable->GetOrigin() : Point();
	}

	Point GetSize() override
	{
		return m_drawable ? m_drawable->GetSize() : Point();
	}

	void SetCenter(const Point& pos) { m_center = pos; }
	Point GetCenter() { return m_center; }

	TDrawable* GetDrawable() const { return m_drawable.get(); }
	NVolumeType GetType() const { return m_type; }

protected:

	void ConfigureDrawable()
	{
		if (!m_drawable)
			return;

		m_drawable->SetScale(m_scale);
		m_drawable->SetOrigin(m_drawable->GetSize() * 0.5f);
	}

	Point m_scale;
	Point m_center;
	std::unique_ptr<TDrawable> m_drawable;
};