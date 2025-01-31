#include "BoundingVolume.h"
#include <numbers>
#include "../Game/Constants.h"
#include "../Utilities/Utilities.h"

BoundingVolume::BoundingVolume(VolumeType type)
	: m_type(type)
{
}

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
	: BoundingVolume(VolumeType::Box), m_size(16, 16)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(Point(10, 10));
}

BoundingBox::BoundingBox(const sf::Vector2f& size)
	: BoundingVolume(VolumeType::Box), m_size(size)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(GetPosition());
}

BoundingBox::BoundingBox(const sf::Vector2f& size, const sf::Vector2f& pos)
	: BoundingVolume(VolumeType::Box), m_size(size)
{
	m_boxNumber = s_count++;
	MakeRectangleShape();
	Reset(m_size);
	Update(pos);
}

BoundingBox::BoundingBox(BoundingCapsule* capsule)
	: BoundingVolume(VolumeType::Box)
{
	float radius = capsule->GetRadius();
	Line s = capsule->GetSegment();

	// Calculate min and max extents of the bounding box
	float min_x = std::min(s.start.x, s.end.x) - radius;
	float max_x = std::max(s.start.x, s.end.x) + radius;
	float min_y = std::min(s.start.y, s.end.y) - radius;
	float max_y = std::max(s.start.y, s.end.y) + radius;

	// Calculate size (width, height)
	Reset({ max_x - min_x, max_y - min_y });
	Update(s.GetMidPoint());
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

bool BoundingBox::Intersects(BoundingVolume* volume)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return Intersects((BoundingBox*)volume);
	case VolumeType::Circle:
		return Intersects((BoundingCircle*)volume);
	case VolumeType::Capsule:
		return Intersects((BoundingCapsule*)volume);
	}

	return false;
}

bool BoundingBox::IntersectsMoving(BoundingVolume* volume, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return IntersectsMoving((BoundingBox*)volume, va, vb, tfirst, tlast);
	case VolumeType::Circle:
		return IntersectsMoving((BoundingCircle*)volume, va, vb, tfirst, tlast);
	case VolumeType::Capsule:
		return IntersectsMoving((BoundingCapsule*)volume, va, vb, tfirst, tlast);
	}

	return false;
}

Point BoundingBox::GetSeparationVector(BoundingBox* other)
{
	// Calculate delta (absolute difference in positions)
	Point delta = other->GetPosition() - GetPosition();
	delta = { std::abs(delta.x), std::abs(delta.y) };  // Make delta absolute

	// Calculate overlap using provided formula
	Point overlap = (other->m_extents + m_extents) - delta;

	// Find the minimum translation direction
	if (overlap.x < overlap.y)
	{
		// Push along X axis
		float direction = (other->GetPosition().x < GetPosition().x) ? -1.0f : 1.0f;
		return { overlap.x * direction, 0 };
	}
	else
	{
		// Push along Y axis
		float direction = (other->GetPosition().y < GetPosition().y) ? -1.0f : 1.0f;
		return { 0, overlap.y * direction };
	}
}

Point BoundingBox::GetSeparationVector(BoundingCircle* other)
{
	Point circlePos = other->GetPosition();

	// Find the closest point on the AABB to the circle's center
	Point closestPoint = {
		std::max(m_min.x, std::min(circlePos.x, m_max.x)),
		std::max(m_min.y, std::min(circlePos.y, m_max.y))
	};

	// Compute the vector from the closest point to the circle's center
	Point displacement = circlePos - closestPoint;
	float distance = pnt::length(displacement);

	// Calculate the penetration depth
	float penetrationDepth = other->GetRadius() - distance;

	pnt::Normalize(displacement)* penetrationDepth;

	// Get the separation vector by normalizing the displacement and scaling by penetration depth
	return pnt::Normalize(displacement) * penetrationDepth;
}

Point BoundingBox::GetSeparationVector(BoundingCapsule* other)
{
	Point closestPoint = other->GetSegment().ClosestPointOnLineSegment(GetPosition());

	// Use AABB vs Circle logic with closest point as the "circle center"
	Point clampedPoint = {
		std::max(m_min.x, std::min(closestPoint.x, m_max.x)),
		std::max(m_min.y, std::min(closestPoint.y, m_max.y))
	};

	Point displacement = closestPoint - clampedPoint;
	float distance = pnt::length(displacement);

	float penetrationDepth = other->GetRadius() - distance;
	return pnt::Normalize(displacement) * penetrationDepth;
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

bool BoundingBox::Intersects(BoundingBox* box)
{
	for (size_t i = 0; i < 2; i++)
	{
		if (std::abs(m_center[i] - box->m_center[i])
	> m_extents[i] + box->m_extents[i])
			return false;
	}

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

bool BoundingBox::Intersects(BoundingCapsule* capsule)
{
	// Compute the box's min and max corners
	Point boxMin = m_min;
	Point boxMax = m_max;

	auto line = capsule->GetSegment();

	// Check the line segment (capsule core) against the box
	Point closestToStart = line.start.Clamp(boxMin, boxMax);
	Point closestToEnd = line.end.Clamp(boxMin, boxMax);

	float distStart = line.SqDistPointSegment(closestToStart);
	float distEnd = line.SqDistPointSegment(closestToEnd);

	float radSq = capsule->GetRadius() * capsule->GetRadius();

	// Check if the distances are less than or equal to the capsule's radius squared
	if (distStart <= radSq || distEnd <= radSq)
		return true;

	float closestPointStartDistSq = pnt::lengthSquared((closestToStart - line.start));
	float closestPointEndDistSq = pnt::lengthSquared((closestToEnd - line.end));

	return closestPointStartDistSq <= radSq || closestPointEndDistSq <= radSq;
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
	// Relative velocity (treat box as stationary)
	Point v = vb - va;

	BoundingBox expandedBox = { GetPosition() - Point(circle->GetRadius(), circle->GetRadius()),
								(m_extents * 2) + Point(circle->GetRadius() * 2, circle->GetRadius() * 2) };

	// Compute the time of collision using swept AABB test
	Point invVelocity = {
		(v.x != 0) ? 1.0f / v.x : 0.0f,
		(v.y != 0) ? 1.0f / v.y : 0.0f
	};

	Point boxMin = expandedBox.GetMin();
	Point boxMax = expandedBox.GetMax();

	Point tEnter = (boxMin - circle->GetPosition()) * invVelocity.x;
	Point tExit = (boxMax - circle->GetPosition()) * invVelocity.y;

	if (invVelocity.x < 0) std::swap(tEnter.x, tExit.x);
	if (invVelocity.y < 0) std::swap(tEnter.y, tExit.y);

	// Find the latest entry time and the earliest exit time
	float entryTime = std::max(tEnter.x, tEnter.y);
	float exitTime = std::min(tExit.x, tExit.y);

	// If the entry time is greater than the exit time, or the exit time is less than zero, no collision
	if (entryTime > exitTime || exitTime < 0)
		return false;

	// If entry time is between 0 and 1, there's a collision
	if (entryTime >= 0 && entryTime <= 1)
	{
		tfirst = entryTime;
		tlast = exitTime;
		return true;
	}

	return false;
}

bool BoundingBox::IntersectsMoving(BoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return capsule->IntersectsMoving((BoundingVolume*)this, va, vb, tfirst, tlast);
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
	: BoundingVolume(VolumeType::Circle), m_radius(8)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(m_radius);
	Update(Point(10, 10));
}

BoundingCircle::BoundingCircle(float radius)
	: BoundingVolume(VolumeType::Circle), m_radius(radius)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(m_radius);
	Update(GetPosition());
}

BoundingCircle::BoundingCircle(float radius, const sf::Vector2f& pos)
	: BoundingVolume(VolumeType::Circle), m_radius(radius)
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

bool BoundingCircle::Intersects(BoundingVolume* volume)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return Intersects((BoundingBox*)volume);
	case VolumeType::Circle:
		return Intersects((BoundingCircle*)volume);
	case VolumeType::Capsule:
		return Intersects((BoundingCapsule*)volume);
	}

	return false;
}

bool BoundingCircle::IntersectsMoving(BoundingVolume* volume, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return IntersectsMoving((BoundingBox*)volume, va, vb, tfirst, tlast);
	case VolumeType::Circle:
		return IntersectsMoving((BoundingCircle*)volume, va, vb, tfirst, tlast);
	case VolumeType::Capsule:
		return IntersectsMoving((BoundingCapsule*)volume, va, vb, tfirst, tlast);
	}

	return false;
}

Point BoundingCircle::GetSeparationVector(BoundingBox* other)
{
	return other->GetSeparationVector(this);
}

Point BoundingCircle::GetSeparationVector(BoundingCircle* other)
{
	Point displacement = other->GetPosition() - GetPosition();
	float distance = pnt::length(displacement);
	float radiusSum = other->GetRadius() + GetRadius();

	// Compute the penetration depth
	float penetrationDepth = radiusSum - distance;

	// Compute the separation vector (normalize displacement and scale by penetration)
	return pnt::Normalize(displacement) * penetrationDepth;
}

Point BoundingCircle::GetSeparationVector(BoundingCapsule* other)
{
	// Find the closest point on the capsule's segment to the circle
	Point closestPoint = other->GetSegment().ClosestPointOnLineSegment(GetPosition());

	// Use Circle vs Circle logic with the closest point as the "capsule circle centre"
	Point displacement = GetPosition() - closestPoint;
	float distance = pnt::length(displacement);
	float radiusSum = GetRadius() + other->GetRadius();

	// Compute the penetration depth
	float penetrationDepth = radiusSum - distance;

	// Compute the separation vector (normalize displacement and scale by penetration)
	return pnt::Normalize(displacement) * penetrationDepth;
}

bool BoundingCircle::Intersects(BoundingBox* box)
{
	return box->Intersects((BoundingVolume*)this);
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

bool BoundingCircle::Intersects(BoundingCapsule* capsule)
{
	float r = m_radius + capsule->GetRadius();

	float distSq = capsule->GetSegment().SqDistPointSegment(m_center);

	return distSq <= r * r;
}

bool BoundingCircle::IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return box->IntersectsMoving((BoundingVolume*)this, va, vb, tfirst, tlast);
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
		tlast = 0.0f;
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

	// Calculate the times of entry and exit
	float sqrtD = std::sqrt(d);
	tfirst = (-b - sqrtD) / a; // Time of entry
	tlast = (-b + sqrtD) / a;  // Time of exit

	// Ensure the times are within valid bounds (0 to 1 for a single frame)
	if (tfirst < 0.0f || tfirst > 1.0f)
		return false; // No intersection in the given time interval

	return true;
}

bool BoundingCircle::IntersectsMoving(BoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return capsule->IntersectsMoving((BoundingVolume*)this, va, vb, tfirst, tlast);
}

int BoundingCapsule::s_count = 0;

BoundingCapsule::BoundingCapsule()
	: BoundingVolume(VolumeType::Capsule), m_radius(8), m_angle(0)
{
	m_capsuleNumber = s_count++;
	MakeCapsuleShape();
	Reset(m_radius, 16, m_angle);

	auto thickness = GetRect()->getOutlineThickness();
	Update(Point(m_radius + thickness, (m_radius * 2) + thickness));
}

BoundingCapsule::BoundingCapsule(float radius, float length, float angle)
	: BoundingVolume(VolumeType::Capsule), m_radius(radius), m_angle(angle)
{
	m_capsuleNumber = s_count++;
	MakeCapsuleShape();
	Reset(m_radius, length, m_angle);

	auto thickness = GetRect()->getOutlineThickness();
	Update(Point(m_radius + thickness, (m_radius * 2) + thickness));
}

BoundingCapsule::BoundingCapsule(float radius, float length, float angle, const sf::Vector2f& pos)
	: BoundingVolume(VolumeType::Capsule), m_radius(radius), m_angle(angle)
{
	m_capsuleNumber = s_count++;
	MakeCapsuleShape();
	Reset(m_radius, length, m_angle);

	Update(pos);
}

BoundingCapsule::BoundingCapsule(float radius, const Line& segment)
	: BoundingVolume(VolumeType::Capsule), m_radius(radius), m_angle(segment.CalculateAngle())
{
	MakeCapsuleShape();
	Reset(m_radius, pnt::distance(segment.start, segment.end), m_angle);

	Update(segment.GetMidPoint());
}

void BoundingCapsule::Reset(float radius, float length, float angle)
{
	auto rect = GetRect();
	rect->setSize(Point(radius * 2.f, length));
	rect->setOrigin(radius, length / 2.f);
	rect->setRotation(angle);

	m_circle1.setRadius(radius);
	m_circle1.setOrigin(radius, radius);

	m_circle2.setRadius(radius);
	m_circle2.setOrigin(radius, radius);
}

void BoundingCapsule::Reset(float radius, float angle)
{
	auto rect = GetRect();
	rect->setSize(Point(radius * 2.f, 150));
	rect->setOrigin(radius, 75);
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
void CalculateRotatedRectangleCorners(Point corners[4], const Point& centre, const Point& size, float angle)
{
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

bool BoundingCapsule::Intersects(BoundingVolume* volume)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return Intersects((BoundingBox*)volume);
	case VolumeType::Circle:
		return Intersects((BoundingCircle*)volume);
	case VolumeType::Capsule:
		return Intersects((BoundingCapsule*)volume);
	}

	return false;
}

bool BoundingCapsule::IntersectsMoving(BoundingVolume* volume, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return IntersectsMoving((BoundingBox*)volume, va, vb, tfirst, tlast);
	case VolumeType::Circle:
		return IntersectsMoving((BoundingCircle*)volume, va, vb, tfirst, tlast);
	case VolumeType::Capsule:
		return IntersectsMoving((BoundingCapsule*)volume, va, vb, tfirst, tlast);
	}
}

Point BoundingCapsule::GetSeparationVector(BoundingBox* other)
{
	return other->GetSeparationVector(this);
}

Point BoundingCapsule::GetSeparationVector(BoundingCircle* other)
{
	return other->GetSeparationVector(this);
}

Point BoundingCapsule::GetSeparationVector(BoundingCapsule* other)
{
	// Start with the naive approach: project each capsule's spine onto the other.
	Point closest1 = m_segment.ClosestPointOnLineSegment(other->m_segment.start);
	Point closest2 = other->m_segment.ClosestPointOnLineSegment(m_segment.start);

	// Get the closest point on cap1 for the closest point found on cap2
	closest1 = m_segment.ClosestPointOnLineSegment(closest2);

	// Get the closest point on cap2 for the closest point found on cap1
	closest2 = other->m_segment.ClosestPointOnLineSegment(closest1);

	// Use Circle vs Circle logic with the closest points as "capsule centres"
	Point displacement = closest2 - closest1;
	float distance = pnt::length(displacement);
	float radiusSum = GetRadius() + other->GetRadius();

	// Compute the penetration depth
	float penetrationDepth = radiusSum - distance;

	// Compute the separation vector (normalize displacement and scale by penetration)
	return pnt::Normalize(displacement) * penetrationDepth;
}

bool BoundingCapsule::Intersects(const Point& pnt) const
{
	auto clsPnt = m_segment.ClosestPointOnLineSegment(pnt);

	BoundingCircle circle(m_radius, clsPnt);

	return circle.Intersects(pnt);
}

bool BoundingCapsule::Intersects(BoundingBox* box)
{
	// Compute the box's min and max corners
	Point boxMin = box->GetMin();
	Point boxMax = box->GetMax();

	// Check the line segment (capsule core) against the box
	Point closestToStart = m_segment.start.Clamp(boxMin, boxMax);
	Point closestToEnd = m_segment.end.Clamp(boxMin, boxMax);

	float distStart = m_segment.SqDistPointSegment(closestToStart);
	float distEnd = m_segment.SqDistPointSegment(closestToEnd);

	float radSq = m_radius * m_radius;

	// Check if the distances are less than or equal to the capsule's radius squared
	if (distStart <= radSq || distEnd <= radSq)
		return true;

	float closestPointStartDistSq = pnt::lengthSquared((closestToStart - m_segment.start));
	float closestPointEndDistSq = pnt::lengthSquared((closestToEnd - m_segment.end));

	return closestPointStartDistSq <= radSq || closestPointEndDistSq <= radSq;
}

bool BoundingCapsule::Intersects(BoundingCircle* circle)
{
	float r = circle->GetRadius() + m_radius;

	float dist2 = m_segment.SqDistPointSegment(circle->GetCenter());

	return dist2 <= r * r;
}

bool BoundingCapsule::Intersects(BoundingCapsule* capsule)
{
	float combinedRadiusSquared = (m_radius + capsule->m_radius) * (m_radius + capsule->m_radius);

	// Compute the shortest distance squared between the two line segments
	float distanceSquared = std::min({
		capsule->m_segment.SqDistPointSegment(m_segment.start),
		capsule->m_segment.SqDistPointSegment(m_segment.end),
		m_segment.SqDistPointSegment(capsule->m_segment.start),
		m_segment.SqDistPointSegment(capsule->m_segment.end)
	});

	// Check if the distance is within the combined radii
	return distanceSquared <= combinedRadiusSquared;
}

bool BoundingCapsule::IntersectsMoving(BoundingBox* box, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	BoundingCircle circle(m_radius, m_segment.start);
	if (circle.IntersectsMoving((BoundingVolume*)box, va, vb, tfirst, tlast))
		return true;

	circle.Update(m_segment.end);
	if (circle.IntersectsMoving((BoundingVolume*)box, va, vb, tfirst, tlast))
		return true;

	BoundingBox capBox(this);
	return capBox.IntersectsMoving((BoundingVolume*)box, va, vb, tfirst, tlast);
}

bool BoundingCapsule::IntersectsMoving(BoundingCircle* circle1, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	auto clsPnt = m_segment.ClosestPointOnLineSegment(circle1->GetCenter());

	BoundingCircle circle2(m_radius, clsPnt);

	return circle2.IntersectsMoving((BoundingVolume*)circle1, va, vb, tfirst, tlast);
}

bool BoundingCapsule::IntersectsMoving(BoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	BoundingCircle endpoint1(m_radius, m_segment.start);
	BoundingCircle endpoint2(m_radius, m_segment.end);

	BoundingCircle otherEndpoint1(m_radius, m_segment.start);
	BoundingCircle otherEndpoint2(m_radius, m_segment.end);

	if (endpoint1.IntersectsMoving((BoundingVolume*)&otherEndpoint2, va, vb, tfirst, tlast))
		return true;

	if (endpoint2.IntersectsMoving((BoundingVolume*)&otherEndpoint1, va, vb, tfirst, tlast))
		return true;

	if (endpoint2.IntersectsMoving((BoundingVolume*)&otherEndpoint2, va, vb, tfirst, tlast))
		return true;

	if (endpoint1.IntersectsMoving((BoundingVolume*)&otherEndpoint1, va, vb, tfirst, tlast))
		return true;

	BoundingBox box1 = BoundingBox(this);
	BoundingBox box2 = BoundingBox(capsule);

	return box1.IntersectsMoving((BoundingVolume*)&box2, va, vb, tfirst, tlast);
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