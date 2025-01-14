#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../Utilities/Point.h"

class BoundingVolume
{
public:
	BoundingVolume(float radius);
	BoundingVolume(const sf::Vector2f& size);
	~BoundingVolume() = default;

	virtual void Update(const sf::Vector2f& pos) = 0;
	void Render(sf::RenderWindow& window) { window.draw(*m_shape.get()); }

	const Point& GetCenter() const { return m_center; }

	void SetPosition(const sf::Vector2f& pos) { m_shape->setPosition(pos); }
	const sf::Vector2f& GetPosition() const { return m_shape->getPosition(); }

	void SetOrigin(const sf::Vector2f& origin) { m_shape->setOrigin(origin); }
	const sf::Vector2f& GetOrigin() const { return m_shape->getOrigin(); }

	void Move(float x, float y);
	void Move(const sf::Vector2f& pos);

	void SetFillColour(const sf::Color& col) { m_shape->setFillColor(col); }
	void SetOutlineColour(const sf::Color& col) { m_shape->setOutlineColor(col); }

protected:

	Point m_center;
	std::unique_ptr<sf::Shape> m_shape;
};