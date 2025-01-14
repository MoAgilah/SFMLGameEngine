#pragma once

#include "BoundingVolume.h"

namespace
{
	BC CalculateMinimumBoundingCircle(AABB* box);
}

class BC : public BoundingVolume
{
public:
	BC(float radius);
	BC(float radius, const Point& center);
	~BC() = default;

	void Reset(float radius);

	void Update(const Point& pos);
	void Render(sf::RenderWindow& window);

	bool Intersects(const Point& pnt) const override;
	bool Intersects(AABB* box) override;
	bool Intersects(BC* circle) override;

	bool IntersectsMoving(BC* circle, const Point& va, const Point& vb, float& tfirst, float& tlast);

	BC* Get() { return this; }
	sf::CircleShape* GetCircle() { return ((sf::CircleShape*)m_shape.get()); }

	float GetRadius() const { return m_radius; }

private:

	void Set();

	int m_circleNumber;
	static int s_count;
	float m_radius;
};