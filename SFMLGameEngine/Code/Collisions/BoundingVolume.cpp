#include "BoundingVolume.h"
#include <numbers>
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
	m_shape->setFillColor(sf::Color::Transparent);
	m_shape->setOutlineColor(sf::Color::Red);
	m_shape->setOutlineThickness(2);
}

void BoundingVolume::MakeRectangleShape()
{
	m_shape = std::make_unique<sf::RectangleShape>();
	m_shape->setScale(GameConstants::Scale);
	m_shape->setFillColor(sf::Color::Transparent);
	m_shape->setOutlineColor(sf::Color::Red);
	m_shape->setOutlineThickness(2);
}

int BoundingBox::s_count = 0;

BoundingBox::BoundingBox()
	: m_size(16, 16)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(Point(10, 10));
}

BoundingBox::BoundingBox(const sf::Vector2f& size)
	: m_size(size)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(GetPosition());
}

BoundingBox::BoundingBox(const sf::Vector2f& size, const sf::Vector2f& pos)
	: m_size(size)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(pos);
}

void BoundingBox::Reset(const Point& size)
{
	GetRect()->setSize(size);
	m_extents[0] = (size.x * GameConstants::Scale.x) * 0.5f;
	m_extents[1] = (size.y * GameConstants::Scale.y) * 0.5f;
	SetOrigin(size * 0.5f);
}

void BoundingBox::Update(const Point& pos)
{
	SetPosition(pos);
	SetCenter(GetPosition());
	m_min = GetCenter() - m_extents;
	m_max = GetCenter() + m_extents;
}

float BoundingBox::SqDistPoint(Point p)
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

bool BoundingBox::Intersects(const Point& pnt) const
{
	if (pnt.x >= m_min.x &&
		pnt.x <= m_max.x &&
		pnt.y >= m_min.y &&
		pnt.y <= m_max.y)
		return true;

	return false;
}

bool BoundingBox::Intersects(BoundingBox* box)
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

bool BoundingBox::Intersects(BoundingCircle* circle)
{
	// Compute squared distance between sphere center and AABB
	float sqDist = SqDistPoint(circle->GetCenter());
	float radius = circle->GetRadius();

	// Sphere and AABB intersect if the (squared) distance
	// between them is less than the (squared) sphere radius
	return sqDist <= radius * radius;
}

bool BoundingBox::IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast)
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

bool BoundingBox::IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return false;
}

Line BoundingBox::GetSide(Side side)
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

Point BoundingBox::GetPoint(Side side)
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

BoundingCircle CalculateMinimumBoundingCircle(BoundingBox* box)
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
	return BoundingCircle(radius, center);
}

int BoundingCircle::s_count = 0;

BoundingCircle::BoundingCircle()
	: m_radius(8)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(m_radius);
	Update(Point(10, 10));
}

BoundingCircle::BoundingCircle(float radius)
	: m_radius(radius)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(m_radius);
	Update(GetPosition());
}

BoundingCircle::BoundingCircle(float radius, const sf::Vector2f& pos)
	: m_radius(radius)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(m_radius);
	Update(pos);
}

void BoundingCircle::Reset(float radius)
{
	GetCircle()->setRadius(radius);
	SetOrigin(Point(radius, radius));
}

void BoundingCircle::Update(const Point& pos)
{
	SetPosition(pos);
	SetCenter(GetPosition());
}

bool BoundingCircle::Intersects(const Point& pnt) const
{
	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	Point dist = pnt - GetCenter();

	float distance = pnt::length(dist);

	// if the distance is less than the circle's
	// radius the point is inside!
	return distance <= m_radius;
}

bool BoundingCircle::Intersects(BoundingBox* box)
{
	// Compute squared distance between sphere center and AABB
	float sqDist = box->SqDistPoint(GetCenter());

	// Sphere and AABB intersect if the (squared) distance
	// between them is less than the (squared) sphere radius
	return sqDist <= m_radius * m_radius;
}

bool BoundingCircle::Intersects(BoundingCircle* circle)
{
	// Calculate squared distance between centers
	Point d = GetCenter() - circle->GetCenter();
	float dist2 = pnt::dot(d, d);

	// Spheres intersect if squared distance is less than squared sum of radii
	float radiusSum = m_radius + circle->m_radius;
	return dist2 <= radiusSum * radiusSum;
}

bool BoundingCircle::IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return false;
}

bool BoundingCircle::IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast)
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

int BoundingCapsule::s_count = 0;

BoundingCapsule::BoundingCapsule()
	: m_radius(8), m_angle(0)
{
	m_capsuleNumber = s_count++;
	MakeCapsuleShape();
	Reset(m_radius, m_angle);

	auto thickness = GetRect()->getOutlineThickness();
	Update(Point(m_radius + thickness, (m_radius * 2) + thickness));
}

BoundingCapsule::BoundingCapsule(float radius, float angle)
	: m_radius(radius), m_angle(angle)
{
	m_capsuleNumber = s_count++;
	MakeCapsuleShape();
	Reset(m_radius, m_angle);

	auto thickness = GetRect()->getOutlineThickness();
	Update(Point(m_radius + thickness, (m_radius * 2) + thickness));
}

BoundingCapsule::BoundingCapsule(float radius, float angle, const sf::Vector2f& pos)
	: m_radius(radius), m_angle(angle)
{
	m_capsuleNumber = s_count++;
	MakeCapsuleShape();
	Reset(m_radius, m_angle);

	Update(pos);
}

void BoundingCapsule::Reset(float radius, float angle)
{
	auto rect = GetRect();
	rect->setSize(Point(radius * 2.f, radius * 2.f));
	rect->setOrigin(radius, radius);
	rect->setRotation(angle);

	m_circle1.setRadius(radius);
	m_circle1.setOrigin(radius, radius);

	m_circle2.setRadius(radius);
	m_circle2.setOrigin(radius, radius);
}

void BoundingCapsule::Render(sf::RenderWindow& window)
{
	window.draw(m_circle1);
	BoundingVolume::Render(window);
	window.draw(m_circle2);
}

// Function to calculate the four corners of a rotated rectangle
void CalculateRotatedRectangleCorners(Point corners[4], const Point& centre, const Point& size, float angle) {
	// Convert the angle from degrees to radians
	float radians = angle * std::numbers::pi_v<float> / 180.0f;

	// Precompute sine and cosine of the angle
	float cosTheta = cos(radians);
	float sinTheta = sin(radians);

	// Half dimensions
	float halfWidth = size.x / 2.0f;
	float halfHeight = size.y / 2.0f;

	// Relative corners before rotation
	Point relativeCorners[4] =
	{
		{ -halfWidth, -halfHeight }, // Bottom-left
		{  halfWidth, -halfHeight }, // Bottom-right
		{  halfWidth,  halfHeight }, // Top-right
		{ -halfWidth,  halfHeight }  // Top-left
	};

	// Compute rotated corners
	for (int i = 0; i < 4; ++i)
	{
		corners[i].x = centre.x + relativeCorners[i].x * cosTheta - relativeCorners[i].y * sinTheta;
		corners[i].y = centre.y + relativeCorners[i].x * sinTheta + relativeCorners[i].y * cosTheta;
	}
}

void BoundingCapsule::Update(const Point& pos)
{
	SetPosition(pos);

	m_center = GetPosition();

	Point corners[4];
	auto size = GetRect()->getSize();
	size.x *= GameConstants::Scale.x;
	size.y *= GameConstants::Scale.y;

	CalculateRotatedRectangleCorners(corners, GetCenter(), size, m_angle);

	// Calculate the midpoint of the top side
	m_circle1.setPosition(Line(corners[3], corners[2]).GetMidPoint());
	// Calculate the midpoint of the top side
	m_circle2.setPosition(Line(corners[1], corners[0]).GetMidPoint());
}

bool BoundingCapsule::Intersects(const Point& pnt) const
{
	auto clsPnt = m_segment.ClosestPointOnLineSegment(pnt);

	BoundingCircle circle(m_radius, clsPnt);

	return circle.Intersects(pnt);
}

bool BoundingCapsule::Intersects(BoundingBox* box)
{
	auto clsPnt = m_segment.ClosestPointOnLineSegment(box->GetCenter());

	BoundingCircle circle(m_radius, clsPnt);

	return circle.Intersects(box);
}

bool BoundingCapsule::Intersects(BoundingCircle* circle)
{
	float r = circle->GetRadius() + m_radius;

	float dist2 = m_segment.SqDistPointSegment(circle->GetCenter());

	return dist2 <= r * r;
}

bool BoundingCapsule::IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return false;
}

bool BoundingCapsule::IntersectsMoving(BoundingCircle* circle1, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	auto clsPnt = m_segment.ClosestPointOnLineSegment(circle1->GetCenter());

	BoundingCircle circle2(m_radius, clsPnt);

	return circle2.IntersectsMoving(circle1, va, vb, tfirst, tlast);
}

void BoundingCapsule::MakeCapsuleShape()
{
	MakeRectangleShape();
	m_circle1.setScale(GameConstants::Scale);

	m_circle1.setFillColor(sf::Color::Transparent);
	m_circle1.setOutlineColor(sf::Color::Red);
	m_circle1.setOutlineThickness(2);

	m_circle2.setScale(GameConstants::Scale);
	m_circle2.setFillColor(sf::Color::Transparent);
	m_circle2.setOutlineColor(sf::Color::Red);
	m_circle2.setOutlineThickness(2);
}
