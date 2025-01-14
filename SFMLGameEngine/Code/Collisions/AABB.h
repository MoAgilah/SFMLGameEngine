#pragma once

#include "BoundingVolume.h"

enum Side
{
	Left, Right, Top, Bottom
};

struct Line;

class AABB : public BoundingVolume
{
public:
	AABB();
	AABB(const sf::Vector2f& size);
	~AABB() = default;

	void Reset(const sf::Vector2f& size);

	void Update(const sf::Vector2f& pos);

	float SqDistPoint(Point p);

	bool Intersects(AABB* box);
	bool IntersectsMoving(AABB* box, const Point& va, const Point& vb, float& tfirst, float& tlast);

	AABB* Get() { return this; }
	sf::RectangleShape* GetRect() { return ((sf::RectangleShape*)m_shape.get()); }

	Line GetSide(Side side);
	Point GetPoint(Side side);

	const Point& GetMin() const { return m_min; }
	const Point& GetMax() const { return m_max; }

	const Point& GetExtents() const { return m_extents; }
	const Point& GetOverlap() const { return m_overlap; }

private:

	void Set();

	int m_boxNumber;
	static int s_count;
	Point m_min;
	Point m_max;
	Point m_extents;
	Point m_overlap;
};