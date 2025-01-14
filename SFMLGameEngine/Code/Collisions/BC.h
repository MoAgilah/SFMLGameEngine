#pragma once

#include "BoundingVolume.h"

class BC : public BoundingVolume
{
public:
	BC(float radius);
	~BC() = default;

	void Reset(const sf::Vector2f& size);

	void Update(const sf::Vector2f& pos);

	float SqDistPoint(Point p);

	bool Intersects(BC* circle);
	bool IntersectsMoving(BC* circle, const Point& va, const Point& vb, float& tfirst, float& tlast);

	BC* Get() { return this; }
	sf::CircleShape* GetCircle() { return ((sf::CircleShape*)m_shape.get()); }

private:

	void Set();

	int m_boxNumber;
	static int s_count;
	float m_radius;
};