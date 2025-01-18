#include "BoundingVolume.h"
#include "../Game/Constants.h"
#include "../Utilities/Utilities.h"

void BoundingVolume::Render(sf::RenderWindow& window)
{
	window.draw(*m_shape);
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

void BoundingVolume::MakeCircleShape()
{
	m_shape = std::make_unique<sf::CircleShape>();
	m_shape->setScale(GameConstants::Scale);
}

void BoundingVolume::MakeRectangleShape()
{
	m_shape = std::make_unique<sf::RectangleShape>();
	m_shape->setScale(GameConstants::Scale);
}

int AABB::s_count = 0;

AABB::AABB()
	: m_size(16,16)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(GetPosition());

	SetFillColour(sf::Color::Transparent);
	SetOutlineColour(sf::Color::Red);
	SetOutlineThickness(2);
}

AABB::AABB(const sf::Vector2f& size)
	: m_size(size)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(GetPosition());

	SetFillColour(sf::Color::Transparent);
	SetOutlineColour(sf::Color::Red);
	SetOutlineThickness(2);
}

AABB::AABB(const sf::Vector2f& size, const sf::Vector2f& pos)
	: m_size(size)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(pos);

	SetFillColour(sf::Color::Transparent);
	SetOutlineColour(sf::Color::Red);
	SetOutlineThickness(2);
}

void AABB::Reset(const Point& size)
{
	GetRect()->setSize(size);
	m_extents[0] = (size.x * GameConstants::Scale.x) * 0.5f;
	m_extents[1] = (size.y * GameConstants::Scale.y) * 0.5f;
	SetOrigin(size * 0.5f);
}

void AABB::Update(const Point& pos)
{
	SetPosition(pos);
	SetCenter(GetPosition());
	m_min = GetCenter() - m_extents;
	m_max = GetCenter() + m_extents;
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
	{
		if (std::abs(m_center[i] - box->m_center[i])
			> m_extents[i] + box->m_extents[i])
				return false;
	}

	// calculate the overlap
	Point delta = box->GetPosition() - GetPosition();
	delta = Point(std::abs(delta.x), std::abs(delta.y));
	m_overlap = (m_extents + box->m_extents) - delta;

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

BC CalculateMinimumBoundingCircle(AABB* box)
{
	// Get the min and max points of the AABB
	Point minPoint = box->GetMin();
	Point maxPoint = box->GetMax();

	// Calculate the center of the AABB
	Point center;
	center.x = (minPoint.x + maxPoint.x) / 2.0f;
	center.y = (minPoint.y + maxPoint.y) / 2.0f;

	// Calculate the radius as half the diagonal distance of the AABB
	float dx = maxPoint.x - minPoint.x;
	float dy = maxPoint.y - minPoint.y;
	float radius = std::sqrtf(dx * dx + dy * dy) / 2.0f;

	// Return the bounding circle
	return BC(radius, center);
}

int BC::s_count = 0;

BC::BC()
	: m_radius(16)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(m_radius);
	Update(GetPosition());

	SetFillColour(sf::Color::Transparent);
	SetOutlineColour(sf::Color::Red);
	SetOutlineThickness(2);
}

BC::BC(float radius)
	: m_radius(radius)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(m_radius);
	Update(GetPosition());

	SetFillColour(sf::Color::Transparent);
	SetOutlineColour(sf::Color::Red);
	SetOutlineThickness(2);
}

BC::BC(float radius, const sf::Vector2f& pos)
	: m_radius(radius)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(m_radius);
	Update(pos);

	SetFillColour(sf::Color::Transparent);
	SetOutlineColour(sf::Color::Red);
	SetOutlineThickness(2);
}

void BC::Reset(float radius)
{
	GetCircle()->setRadius(radius);
	SetOrigin(Point(radius, radius));
}

void BC::Update(const Point& pos)
{
	SetPosition(pos);
	SetCenter(GetPosition());
}

bool BC::Intersects(const Point& pnt) const
{
	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	Point dist = pnt - GetCenter();

	float distance = pnt::length(dist);

	// if the distance is less than the circle's
	// radius the point is inside!
	return distance <= m_radius;
}

bool BC::Intersects(AABB* box)
{
	// Compute squared distance between sphere center and AABB
	float sqDist = box->SqDistPoint(GetCenter());

	// Sphere and AABB intersect if the (squared) distance
	// between them is less than the (squared) sphere radius
	return sqDist <= m_radius * m_radius;
}

bool BC::Intersects(BC* circle)
{
	// Calculate squared distance between centers
	Point d = GetCenter() - circle->GetCenter();
	float dist2 = pnt::dot(d, d);

	// Spheres intersect if squared distance is less than squared sum of radii
	float radiusSum = m_radius + circle->m_radius;
	return dist2 <= radiusSum * radiusSum;
}

bool BC::IntersectsMoving(BC* circle, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	Point s = GetCenter() - circle->GetCenter(); // Vector between sphere centers
	float r = circle->m_radius + m_radius; // Sum of sphere radii
	float c = pnt::dot(s, s) - r * r;

	if (c < 0.0f)
	{
		// Spheres initially overlapping so exit directly
		tfirst = 0.0f;
		return true;
	}

	Point v = vb - va; // Relative motion of s1 with respect to stationary s0

	float a = pnt::dot(v, v);
	if (a < std::numeric_limits<float>::epsilon())
		return false; // Spheres not moving relative each other

	float b = pnt::dot(v, s);
	if (b >= 0.0f)
		return false; // Spheres not moving towards each other

	float d = b * b - a * c;
	if (d < 0.0f)
		return false; // No real-valued root, spheres do not intersect

	tfirst = (-b - std::sqrt(d)) / a;

	return true;
}