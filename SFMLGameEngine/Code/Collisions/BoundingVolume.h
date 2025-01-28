#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../Utilities/Utilities.h"

enum Side
{
	Left, Right, Top, Bottom
};

class BoundingBox;
class BoundingCircle;
class BoundingCapsule;

class BoundingVolume
{
public:
	BoundingVolume() = default;
	~BoundingVolume() = default;

	virtual void Update(const Point& pos) = 0;
	virtual void Render(sf::RenderWindow& window);

	virtual bool Intersects(const Point& pnt) const = 0;
	virtual bool Intersects(BoundingBox* box) = 0;
	virtual bool Intersects(BoundingCircle* circle) = 0;
	virtual bool Intersects(BoundingCapsule* capsule) = 0;

	virtual bool IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) = 0;
	virtual bool IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) = 0;

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

class BoundingBox : public BoundingVolume
{
public:
	BoundingBox();
	BoundingBox(const sf::Vector2f& size);
	BoundingBox(const sf::Vector2f& size, const sf::Vector2f& pos);
	BoundingBox(BoundingCapsule* capsule);
	~BoundingBox() = default;

	void Reset(const Point& size);
	void Update(const Point& pos);

	float SqDistPoint(Point p);

	bool Intersects(const Point& pnt) const override;
	bool Intersects(BoundingBox* box) override;
	bool Intersects(BoundingCircle* circle) override;
	bool Intersects(BoundingCapsule* capsule) override;

	bool IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override;
	bool IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override;

	const Point& GetMin() const { return m_min; }
	const Point& GetMax() const { return m_max; }
	const Point& GetExtents() const { return m_extents; }
	const Point& GetOverlap() const { return m_overlap; }

	Line GetSide(Side side);
	Point GetPoint(Side side);

	BoundingBox* Get() { return this; }
	sf::CircleShape* GetCircle() = delete;

protected:

	void MakeCircleShape() = delete;
	void MakeCapsuleShape() = delete;

private:

	int m_boxNumber;
	static int s_count;
	Point m_size;
	Point m_min;
	Point m_max;
	Point m_extents;
	Point m_overlap;
};

BoundingCircle CalculateMinimumBoundingCircle(BoundingBox* box);

class BoundingCircle : public BoundingVolume
{
public:
	BoundingCircle();
	BoundingCircle(float radius);
	BoundingCircle(float radius, const sf::Vector2f& pos);
	~BoundingCircle() = default;

	void Reset(float radius);
	void Update(const Point& pos);

	bool Intersects(const Point& pnt) const override;
	bool Intersects(BoundingBox* box) override;
	bool Intersects(BoundingCircle* circle) override;
	bool Intersects(BoundingCapsule* capsule) override;

	bool IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override;
	bool IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast) override;

	float GetRadius() const { return m_radius; }

	BoundingCircle* Get() { return this; }
	sf::RectangleShape* GetRect() = delete;

protected:

	void MakeRectangleShape() = delete;

private:

	int m_circleNumber;
	static int s_count;
	float m_radius;
};

class BoundingCapsule : public BoundingVolume
{
public:
	BoundingCapsule();
	BoundingCapsule(float radius, float length, float angle);
	BoundingCapsule(float radius, float length, float angle, const sf::Vector2f& pos);
	~BoundingCapsule() = default;

	void Reset(float radius, float angle);
	void Reset(float radius, float length, float angle);
	void Render(sf::RenderWindow& window) override;

	void Update(const Point& pos);

	bool Intersects(const Point& pnt) const override;
	bool Intersects(BoundingBox* box) override;
	bool Intersects(BoundingCircle* circle) override;
	bool Intersects(BoundingCapsule* capsule) override;

	bool IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast) override;
	bool IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast);

	float GetRadius() const { return m_radius; }
	Line& GetSegment() { return m_segment; }

	BoundingCapsule* Get() { return this; }
	sf::CircleShape* GetCircle() = delete;

protected:

	void MakeCircleShape() = delete;
	void MakeCapsuleShape();

private:

	int m_capsuleNumber;
	static int s_count;
	float m_radius;
	float m_angle;
	Line m_segment;
	sf::CircleShape m_circle1;
	sf::CircleShape m_circle2;
};