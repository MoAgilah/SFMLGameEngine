#include "BC.h"
#include "AABB.h"

namespace
{
	BC CalculateMinimumBoundingCircle(AABB * box)
	{
		Point corners[4] = {
			{box->GetMin().x, box->GetMin().y}, // Bottom-left
			{box->GetMax().x, box->GetMin().y}, // Bottom-right
			{box->GetMin().x, box->GetMax().y}, // Top-left
			{box->GetMax().x, box->GetMax().y}  // Top-right
		};

		// Find the pair of extremal points that are farthest apart
		double maxDistance = 0.0;
		Point p1, p2;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = i + 1; j < 4; ++j)
			{
				double dist = pnt::distance(corners[i], corners[j]);
				if (dist > maxDistance)
				{
					maxDistance = dist;
					p1 = corners[i];
					p2 = corners[j];
				}
			}
		}

		Point center;
		center.x = (float)((p1.x + p2.x) / 2.0);
		center.y = (float)((p1.y + p2.y) / 2.0);

		return BC((float)(maxDistance / 2.0), center);
	}
}

int BC::s_count = 0;

BC::BC(float radius)
	: m_radius(radius), BoundingVolume(m_radius)
{
	m_circleNumber = s_count++;
	m_shape->setFillColor(sf::Color::Transparent);
	m_shape->setOutlineColor(sf::Color::Red);
	m_shape->setOutlineThickness(1);
	Set();
	Update(GetPosition());
}

BC::BC(float radius, const Point& center)
	: m_radius(radius), BoundingVolume(m_radius)
{
	m_circleNumber = s_count++;
	m_shape->setFillColor(sf::Color::Transparent);
	m_shape->setOutlineColor(sf::Color::Red);
	m_shape->setOutlineThickness(1);
	Set();
	Update(center);
}

void BC::Reset(float radius)
{
	m_radius = radius;
	GetCircle()->setRadius(m_radius);
	Set();
}

void BC::Update(const Point& pos)
{
	SetPosition(pos);

	m_center[0] = GetPosition().x;
	m_center[1] = GetPosition().y;
}

void BC::Render(sf::RenderWindow& window)
{
	window.draw(*GetCircle());
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

void BC::Set()
{
	GetCircle()->setOrigin(m_radius, m_radius);
}
