#include "BC.h"

BC::BC(float radius)
	: m_radius(radius), BoundingVolume(m_radius)
{
}

void BC::Reset(const sf::Vector2f& size)
{
}

void BC::Update(const sf::Vector2f& pos)
{
}

float BC::SqDistPoint(Point p)
{
	return 0.0f;
}

bool BC::Intersects(BC* circle)
{
	return false;
}

bool BC::IntersectsMoving(BC* circle, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return false;
}

void BC::Set()
{
}
