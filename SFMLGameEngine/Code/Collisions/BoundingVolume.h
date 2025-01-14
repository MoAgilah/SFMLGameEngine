#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../Utilities/Point.h"

class AABB;
class BC;

class BoundingVolume
{
public:
	BoundingVolume(float radius);
	BoundingVolume(const Point& size);
	~BoundingVolume() = default;

	virtual void Update(const Point& pos) = 0;
	virtual void Render(sf::RenderWindow& window) = 0;

	virtual bool Intersects(const Point& pnt) const = 0;
	virtual bool Intersects(AABB* box) = 0;
	virtual bool Intersects(BC* circle) = 0;

	const Point& GetCenter() const { return m_center; }

	void SetPosition(const Point& pos) { m_shape->setPosition(pos); }
	Point GetPosition() const { return m_shape->getPosition(); }

	void SetOrigin(const Point& origin) { m_shape->setOrigin(origin); }
	Point GetOrigin() const { return m_shape->getOrigin(); }

	void Move(float x, float y);
	void Move(const Point& pos);

	void SetFillColour(const sf::Color& col) { m_shape->setFillColor(col); }
	void SetOutlineColour(const sf::Color& col) { m_shape->setOutlineColor(col); }

protected:

	Point m_center;
	std::unique_ptr<sf::Shape> m_shape;
};