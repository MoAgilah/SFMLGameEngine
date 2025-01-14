#include "BoundingVolume.h"

BoundingVolume::BoundingVolume(float radius)
{
	m_shape = std::make_unique<sf::CircleShape>(radius);
}

BoundingVolume::BoundingVolume(const Point& size)
{
	m_shape = std::make_unique<sf::RectangleShape>(size);
}

void BoundingVolume::Move(float x, float y)
{
	m_shape->move(sf::Vector2f(x, y));
	Update(GetPosition());
}

void BoundingVolume::Move(const Point& pos)
{
	m_shape->move(pos);
	Update(GetPosition());
}
