#include "AABB.h"
#include <limits>
#include "../Collisions/BC.h"
#include "../Game/Constants.h"
#include "../Utilities/Utilities.h"

int AABB::s_count = 0;

AABB::AABB()
	: BoundingVolume(Point(16, 16))
{
	m_boxNumber = s_count++;
	m_shape->setFillColor(sf::Color::Transparent);
	m_shape->setOutlineColor(sf::Color::Red);
	m_shape->setOutlineThickness(5);
	Set();
	Update(GetPosition());
}

AABB::AABB(const Point& size)
	: BoundingVolume(sf::Vector2f(size))
{
	m_boxNumber = s_count++;
	m_shape->setFillColor(sf::Color::Transparent);
	m_shape->setOutlineColor(sf::Color::Red);
	m_shape->setOutlineThickness(5);
	Set();
	Update(GetPosition());
}

void AABB::Reset(const Point& size)
{
	GetRect()->setSize(size);
	Set();
}

void AABB::Update(const Point& pos)
{
	SetPosition(pos);

	m_center[0] = GetPosition().x;
	m_center[1] = GetPosition().y;

	m_min = m_center - m_extents;
	m_max = m_center + m_extents;
}

void AABB::Render(sf::RenderWindow& window)
{
	window.draw(*GetRect());
}

float AABB::SqDistPoint(Point p)
{
	float sqDist = 0.0f;

	for (size_t i = 0; i < 2; i++)
	{
		float v = p[i];
		if (v < m_min[i]) sqDist += (m_min[i] - v) * (m_min[i] - v);
		if (v > m_max[i]) sqDist += (v - m_max[i]) * (v - m_max[i]);
	}

	return sqDist;
}

bool AABB::Intersects(const Point& pnt) const
{
	if (pnt.x >= m_min.x &&
		pnt.x <= m_max.x &&
		pnt.y >= m_min.y &&
		pnt.y <= m_max.y)
		return true;

	return false;
}

bool AABB::Intersects(AABB* box)
{
	for (size_t i = 0; i < 2; i++)
		if (std::abs(m_center[i] - box->m_center[i]) > m_extents[i] + box->m_extents[i]) return false;

	// calculate the overlap
	Point delta = box->GetPosition() - GetPosition();
	delta = Point(std::abs(delta.x), std::abs(delta.y));
	m_overlap = (GetExtents() + box->GetExtents()) - delta;

	// Overlapping on all axes means AABBs are intersecting
	return true;
}

bool AABB::Intersects(BC* circle)
{
	// Compute squared distance between sphere center and AABB
	float sqDist = SqDistPoint(circle->GetCenter());
	float radius = circle->GetRadius();

	// Sphere and AABB intersect if the (squared) distance
	// between them is less than the (squared) sphere radius
	return sqDist <= radius * radius;
}

bool AABB::IntersectsMoving(AABB* box, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	// Exit early if ‘a’ and ‘b’ initially overlapping
	if (Intersects(box))
	{
		tfirst = tlast = 0.0f;
		return true;
	}

	// Use relative velocity; effectively treating ’a’ as stationary
	Point v = vb - va;

	// Initialize times of first and last contact
	tfirst = 0.0f;
	tlast = 1.0f;

	for (int i = 0; i < 2; i++)
	{
		if (v[i] < 0.0f)
		{
			if (box->m_max[i] < m_min[i]) return false; // Non intersecting and moving apart
			if (m_max[i] < box->m_min[i]) tfirst = std::max((m_max[i] - box->m_min[i]) / v[i], tfirst);
			if (box->m_max[i] > m_min[i]) tlast = std::min((m_min[i] - box->m_max[i]) / v[i], tlast);
		}

		if (v[i] > 0.0f)
		{
			if (box->m_min[i] > m_max[i]) return false; // Non intersecting and moving apart
			if (box->m_max[i] < m_min[i]) tfirst = std::max((m_min[i] - box->m_max[i]) / v[i], tfirst);
			if (m_max[i] > box->m_min[i]) tlast = std::min((m_max[i] - box->m_min[i]) / v[i], tlast);
		}

		// No overlap possible if time of first contact occurs after time of last contact
		if (tfirst > tlast) return false;
	}

	return true;
}

Line AABB::GetSide(Side side)
{
	switch (side)
	{
	case Left:
		return Line(m_min, Point(m_min.x, m_max.y));
	case Right:
		return Line(Point(m_max.x, m_min.y), m_max);
	case Top:
		return Line(m_min, Point(m_max.x, m_min.y));
	case Bottom:
		return Line(Point(m_min.x, m_max.y), m_max);
	default:
		throw std::out_of_range("Side enum value doesn't exist");
	}
}

Point AABB::GetPoint(Side side)
{
	switch (side)
	{
	case Left:
		return m_center - Point(m_extents.x, 0);
	case Right:
		return m_center + Point(m_extents.x, 0);
	case Top:
		return m_center - Point(0, m_extents.y);
	case Bottom:
		return m_center + Point(0, m_extents.y);
	default:
		throw std::out_of_range("Side enum value doesn't exist");
	}
}

void AABB::Set()
{
	m_shape->setScale(GameConstants::Scale);
	m_shape->setOrigin(GetRect()->getSize() * 0.5f);
	m_extents[0] = (GetRect()->getSize().x * GameConstants::Scale.x) / 2;
	m_extents[1] = (GetRect()->getSize().y * GameConstants::Scale.y) / 2;
}
