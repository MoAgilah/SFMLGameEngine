#pragma once

#include "BoundingVolume.h"
#include "../Utilities/Utilities.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum Types
{
	EMPTY, GRND, WALL, DIAGU, DIAGD, LCRN, RCRN, OWAY
};

class DynamicObject;

class Tile
{
public:
	Tile();
	Tile(int gX, int gY, sf::Font* font);
	~Tile() = default;

	bool Intersects(DynamicObject* obj);
	void ResolveCollision(DynamicObject* obj);

	std::string_view GetID() const { return m_id; }

	int GetRowNum() const { return m_rowNum; }
	int GetColNum() const { return m_colNum; }

	void SetType(int type) { m_type = type; };
	int GetType() const { return m_type; }

	bool GetActive() const { return m_visible; }
	void SetActive(bool vis) { m_visible = vis; }

	void SetPosition(const Point& pos);
	Point GetPosition() { return m_aabb.GetPosition(); }

	Point GetOrigin() { return m_aabb.GetOrigin(); }

	BoundingBox* GetBoundingBox() { return &m_aabb; }
	sf::RectangleShape* GetRect() { return m_aabb.GetRect(); }
	Line GetSlope(int bgn, int end) { return Line(m_slope, bgn, end); }
	Line GetEdge() const { return m_edge; }

	float GetTileHeight() { return m_aabb.GetExtents().y * 2; }

	void SetFillColour(sf::Color col) { m_aabb.SetFillColour(col); }
	void SetOutlineColour(sf::Color col) { m_aabb.SetOutlineColour(col); }

	void Render(sf::RenderWindow& window);

private:

	Point GetSeperationVector(DynamicObject* obj);

	void ResolveObjectToBoxTop(DynamicObject* obj);
	void ResolveObjectToBoxBottom(DynamicObject* obj);
	void ResolveObjectToBoxHorizontally(DynamicObject* obj);
	bool ResolveObjectToSlopeTop(DynamicObject* obj);
	bool ResolveObjectToSlopeIncline(DynamicObject* obj, int start, int end);
	bool ResolveObjectToSlopeDecline(DynamicObject* obj, int start, int end);

	void ResolveObjectToEdgeBounds(DynamicObject* obj);

	bool m_visible = false;
	bool m_hasFont = false;
	int m_colNum = -1;
	int m_rowNum = -1;
	int m_type = EMPTY;

	Line m_edge;
	sf::Text m_text;
	std::string m_id;
	BoundingBox m_aabb;
	sf::ConvexShape m_slope;
};