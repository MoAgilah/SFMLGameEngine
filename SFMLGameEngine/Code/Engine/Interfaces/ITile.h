#pragma once

#include "IShape.h"
#include "IText.h"
#include "IBoundingVolume.h"
#include "IGameObject.h"
#include "../../Utilities/Colour.h"
#include <memory>
#include <string>
#include <vector>

enum class NTypes
{
	EMPTY, GRND, WALL, DIAGU, DIAGD, LCRN, RCRN, OWAY
};

class ITile
{
public:
	ITile() {}
	ITile(std::shared_ptr<IBoundingBox> aabb, std::shared_ptr<IDrawable> text, std::shared_ptr<ITriangleShape> slope)
		: m_text(std::move(text)), m_slope(std::move(slope)), m_aabb(std::move(aabb)) {}
	virtual ~ITile() = 0;

	virtual void Render(IRenderer* renderer) = 0;

	bool Intersects(IDynamicGameObject* obj) { return m_aabb->Intersects(obj->GetVolume()); }
	virtual void ResolveCollision(IDynamicGameObject* obj) = 0;

	std::string_view GetID() const { return m_id; }

	int GetRowNum() const { return m_rowNum; }
	int GetColNum() const { return m_colNum; }

	void SetType(int type) { m_type = (NTypes)type; };
	NTypes GetType() const { return m_type; }

	bool GetActive() const { return m_visible; }
	void SetActive(bool vis) { m_visible = vis; }

	virtual void SetPosition(const Point& pos) = 0;
	Point GetPosition() { return m_aabb->GetPosition(); }

	void SetOrigin(const Point& origin) { m_aabb->SetOrigin(origin); }
	Point GetOrigin() { return m_aabb->GetOrigin(); }

	IBoundingBox* GetBoundingBox() { return m_aabb.get(); }

	virtual void SetSlope(std::shared_ptr<ITriangleShape> slope)
	{
		m_slope = std::move(slope);
	}

	virtual Line GetSlope(int bgn, int end) { return m_slope->GetLine(bgn, end); }

	Line GetEdge() const { return m_edge; }

	float GetTileHeight() { return m_aabb->GetExtents().y * 2; }
	virtual void SetFillColour(Colour col) = 0;
	virtual void SetOutlineColour(Colour col) = 0;

protected:

	Point GetSeperationVector(IDynamicGameObject* obj) { return m_aabb->GetSeparationVector(obj->GetVolume()); }

	virtual void ResolveObjectToBoxTop(IDynamicGameObject* obj) = 0;
	virtual void ResolveObjectToBoxBottom(IDynamicGameObject* obj) = 0;
	virtual void ResolveObjectToBoxHorizontally(IDynamicGameObject* obj) = 0;
	virtual bool ResolveObjectToSlopeTop(IDynamicGameObject* obj) = 0;
	virtual bool ResolveObjectToSlopeIncline(IDynamicGameObject* obj, int start, int end) = 0;
	virtual bool ResolveObjectToSlopeDecline(IDynamicGameObject* obj, int start, int end) = 0;
	virtual void ResolveObjectToEdgeBounds(IDynamicGameObject* obj) = 0;

	int m_colNum = -1;
	int m_rowNum = -1;
	bool m_visible = false;
	bool m_hasFont = false;
	NTypes m_type = NTypes::EMPTY;
	std::string m_id;
	Line m_edge;
	std::shared_ptr<IDrawable> m_text;
	std::shared_ptr<ITriangleShape> m_slope;
	std::shared_ptr<IBoundingBox> m_aabb;
};