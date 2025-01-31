#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "../Collisions/BoundingVolume.h"
#include "../Utilities/Utilities.h"

enum Types
{
	EMPTY, GRND, WALL, DIAGU, DIAGD, LCRN, RCRN, OWAY
};

class Tile
{
public:
	Tile();
	Tile(int gX, int gY, sf::Font* font);
	~Tile() = default;

	std::string_view GetID() const { return m_id; }

	int GetRowNum() const { return m_rowNum; }
	int GetColNum() const { return m_colNum; }

	void SetType(int type) { m_type = type; };
	int GetType() const { return m_type; }

	bool GetActive() const { return m_visible; }
	void SetActive(bool vis) { m_visible = vis; }

	void SetPosition(sf::Vector2f pos);
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

	bool m_visible = false;
	bool m_hasFont = false;
	int m_colNum = -1;
	int m_rowNum = -1;
	int m_type = EMPTY;

	BoundingBox m_aabb;
	sf::Text m_text;
	std::string m_id;
	Line m_edge;
	sf::ConvexShape m_slope;
};