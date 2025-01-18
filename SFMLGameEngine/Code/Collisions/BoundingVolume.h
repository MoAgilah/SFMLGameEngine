#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../Utilities/Point.h"

enum Side
{
	Left, Right, Top, Bottom
};

struct Line;
class AABB;
class BC;

class BoundingVolume
{
public:
	BoundingVolume() = default;
	~BoundingVolume() = default;

	virtual void Update(const Point& pos) = 0;
	void Render(sf::RenderWindow& window);

	virtual bool Intersects(const Point& pnt) const = 0;
	virtual bool Intersects(AABB* box) = 0;
	virtual bool Intersects(BC* circle) = 0;

	virtual void SetPosition(const Point& pos) { m_shape->setPosition(pos); }
	virtual Point GetPosition() { return m_shape->getPosition(); }

	void SetOrigin(const Point& origin) { m_shape->setOrigin(origin); }
	Point GetOrigin() const { return m_shape->getOrigin(); }

	void SetCenter(const Point& pos) { m_center = pos; }
	const Point& GetCenter() const { return m_center; }

	void Move(float x, float y);
	void Move(const Point& pos);

	void SetFillColour(const sf::Color& col) { m_shape->setFillColor(col); }
	void SetOutlineColour(const sf::Color& col) { m_shape->setOutlineColor(col); }
	void SetOutlineThickness(float scale) { m_shape->setOutlineThickness(scale); }

	sf::CircleShape* GetCircle() { return static_cast<sf::CircleShape*>(m_shape.get()); }
	sf::RectangleShape* GetRect() { return static_cast<sf::RectangleShape*>(m_shape.get()); }

protected:

	void MakeCircleShape();
	void MakeRectangleShape();

	Point m_center;

private:

	std::unique_ptr<sf::Shape> m_shape;
};

class AABB : public BoundingVolume
{
public:
	AABB();
	AABB(const sf::Vector2f& size);
	AABB(const sf::Vector2f& size, const sf::Vector2f& pos);
	~AABB() = default;

	void Reset(const Point& size);

	void Update(const Point& pos);

	float SqDistPoint(Point p);

	bool Intersects(const Point& pnt) const override;
	bool Intersects(AABB* box)  override;
	bool Intersects(BC* circle)  override;

	bool IntersectsMoving(AABB* box, const Point& va, const Point& vb, float& tfirst, float& tlast);

	AABB* Get() { return this; }

	const Point& GetMin() const { return m_min; }
	const Point& GetMax() const { return m_max; }
	const Point& GetExtents() const { return m_extents; }
	const Point& GetOverlap() const { return m_overlap; }

	Line GetSide(Side side);

	Point GetPoint(Side side);

	sf::CircleShape* GetCircle() = delete;

protected:
	void MakeCircleShape() = delete;

private:

	int m_boxNumber;
	static int s_count;
	Point m_size;
	Point m_min;
	Point m_max;
	Point m_extents;
	Point m_overlap;
};

namespace
{
	BC CalculateMinimumBoundingCircle(AABB* box);
}

class BC : public BoundingVolume
{
public:
	BC();
	BC(float radius);
	BC(float radius, const sf::Vector2f& pos);
	~BC() = default;

	void Reset(float radius);

	void Update(const Point& pos);

	bool Intersects(const Point& pnt) const override;
	bool Intersects(AABB* box)  override;
	bool Intersects(BC* circle)  override;

	bool IntersectsMoving(BC* circle, const Point& va, const Point& vb, float& tfirst, float& tlast);

	BC* Get() { return this; }

	float GetRadius() const { return m_radius; }

	sf::RectangleShape* GetRect() = delete;

protected:
	void MakeRectangleShape() = delete;

private:

	int m_circleNumber;
	static int s_count;
	float m_radius;
};