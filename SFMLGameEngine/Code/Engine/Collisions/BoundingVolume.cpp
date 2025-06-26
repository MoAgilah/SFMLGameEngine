#include "BoundingVolume.h"

#include "CollisionManager.h"
#include "../Core/Constants.h"
#include "../../Utilities/Utilities.h"
#include <numbers>

namespace
{
	constexpr float BUFFER = 0.01f;

	void SetupShape(sf::Shape& shape, const Point& scale)
	{
		shape.setScale(scale);
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color::Red);
		shape.setOutlineThickness(2);
	}
}

BoundingVolume::BoundingVolume(VolumeType type)
	: m_type(type), m_scale(GameConstants::Scale)
{
}

void BoundingVolume::Render(sf::RenderWindow& window)
{
	window.draw(*m_shape);
}

void BoundingVolume::Move(float x, float y)
{
	Move(Point(x, y));
}

void BoundingVolume::Move(const Point& pos)
{
	m_shape->move(pos);
	Update(GetPosition());
}

void BoundingVolume::MakeCircleShape()
{
	auto circle = std::make_unique<sf::CircleShape>();
	SetupShape(*circle, m_scale);
	m_shape = std::move(circle);
}

void BoundingVolume::MakeRectangleShape()
{
	auto rect = std::make_unique<sf::RectangleShape>();
	SetupShape(*rect, m_scale);
	m_shape = std::move(rect);
}

int BoundingBox::s_count = 0;

BoundingBox::BoundingBox()
	: BoundingBox(Point(16, 16), Point(10, 10))
{}

BoundingBox::BoundingBox(const Point& size)
	: BoundingBox(size, Point(10, 10))
{}

BoundingBox::BoundingBox(const Point& size, const Point& pos)
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
	MakeRectangleShape();
	const float radius = capsule->GetRadius();
	const auto& seg = capsule->GetSegment();

	const float minX = std::min(seg.start.x, seg.end.x) - radius;
	const float maxX = std::max(seg.start.x, seg.end.x) + radius;
	const float minY = std::min(seg.start.y, seg.end.y) - radius;
	const float maxY = std::max(seg.start.y, seg.end.y) + radius;

	Reset({ maxX - minX, maxY - minY });
	Update(seg.GetMidPoint());
}

void BoundingBox::Reset(const Point& size)
{
	GetRect()->setSize(size);
	const auto scale = GetScale();
	m_extents[0] = (size.x * scale.x) * 0.5f;
	m_extents[1] = (size.y * scale.y) * 0.5f;
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
		return Intersects(static_cast<BoundingBox*>(volume));
	case VolumeType::Circle:
		return Intersects(static_cast<BoundingCircle*>(volume));
	case VolumeType::Capsule:
		return Intersects(static_cast<BoundingCapsule*>(volume));
	}

	return false;
}

bool BoundingBox::IntersectsMoving(BoundingVolume* volume, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return IntersectsMoving(static_cast<BoundingBox*>(volume), va, vb, tfirst, tlast);
	case VolumeType::Circle:
		return IntersectsMoving(static_cast<BoundingCircle*>(volume), va, vb, tfirst, tlast);
	case VolumeType::Capsule:
		return IntersectsMoving(static_cast<BoundingCapsule*>(volume), va, vb, tfirst, tlast);
	}

	return false;
}

Point BoundingBox::GetSeparationVector(BoundingVolume* volume)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return GetSeparationVector(static_cast<BoundingBox*>(volume));
	case VolumeType::Circle:
		return GetSeparationVector(static_cast<BoundingCircle*>(volume));
	case VolumeType::Capsule:
		return GetSeparationVector(static_cast<BoundingCapsule*>(volume));
	}

	return Point();
}

Line BoundingBox::GetSide(Side side)
{
	switch (side)
	{
	case Side::Left:
		return Line(m_min, Point(m_min.x, m_max.y));
	case Side::Right:
		return Line(Point(m_max.x, m_min.y), m_max);
	case Side::Top:
		return Line(m_min, Point(m_max.x, m_min.y));
	case Side::Bottom:
		return Line(Point(m_min.x, m_max.y), m_max);
	default:
		throw std::out_of_range("Side enum value doesn't exist");
	}
}

Point BoundingBox::GetPoint(Side side)
{
	switch (side)
	{
	case Side::Left:
		return m_center - Point(m_extents.x, 0);
	case Side::Right:
		return m_center + Point(m_extents.x, 0);
	case Side::Top:
		return m_center - Point(0, m_extents.y);
	case Side::Bottom:
		return m_center + Point(0, m_extents.y);
	default:
		throw std::out_of_range("Side enum value doesn't exist");
	}
}

void BoundingBox::SetScale(const Point& scale)
{
	BoundingVolume::SetScale(scale);
	Reset(GetRect()->getSize());
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
	if (Intersects(box))
	{
		tfirst = tlast = 0.0f;
		return true;
	}

	Point v = vb - va;
	if (std::abs(v.x) < CollisionManager::EPSILON && std::abs(v.y) < CollisionManager::EPSILON)
		return false;

	tfirst = 0.0f;
	tlast = 1.0f;

	for (int i = 0; i < 2; i++)
	{
		if (std::abs(v[i]) < CollisionManager::EPSILON)
		{
			if (box->m_max[i] < m_min[i] || box->m_min[i] > m_max[i])
				return false;

			continue;
		}

		if (v[i] < 0.0f)
		{
			if (box->m_max[i] < m_min[i] && v[i] <= 0.0f) return false;
			if (m_max[i] <= box->m_min[i]) tfirst = std::max((m_max[i] - box->m_min[i]) / v[i], tfirst);
			if (box->m_max[i] >= m_min[i]) tlast = std::min((m_min[i] - box->m_max[i]) / v[i], tlast);
		}
		else if (v[i] > 0.0f)
		{
			if (box->m_min[i] > m_max[i] && v[i] >= 0.0f) return false;
			if (box->m_max[i] <= m_min[i]) tfirst = std::max((m_min[i] - box->m_max[i]) / v[i], tfirst);
			if (m_max[i] >= box->m_min[i]) tlast = std::min((m_max[i] - box->m_min[i]) / v[i], tlast);
		}
	}

	if (tfirst > tlast) return false;
	return true;
}

bool BoundingBox::IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	// Calculate relative velocity: circle's motion relative to the box
	Point relativeVelocity = vb - va;

	// No movement → fall back to static check
	if (pnt::lengthSquared(relativeVelocity) < CollisionManager::EPSILON * CollisionManager::EPSILON)
		return Intersects(circle);

	// Treat the circle as a moving point by expanding the box by the radius
	float r = circle->GetRadius();
	Point boxMin = GetMin() - Point(r, r);
	Point boxMax = GetMax() + Point(r, r);

	Point invVelocity = {
		std::abs(relativeVelocity.x) > CollisionManager::EPSILON ? 1.f / relativeVelocity.x : 0.f,
		std::abs(relativeVelocity.y) > CollisionManager::EPSILON ? 1.f / relativeVelocity.y : 0.f
	};

	float tEnterX = (boxMin.x - circle->GetPosition().x) * invVelocity.x;
	float tExitX = (boxMax.x - circle->GetPosition().x) * invVelocity.x;
	if (invVelocity.x < 0.f) std::swap(tEnterX, tExitX);

	float tEnterY = (boxMin.y - circle->GetPosition().y) * invVelocity.y;
	float tExitY = (boxMax.y - circle->GetPosition().y) * invVelocity.y;
	if (invVelocity.y < 0.f) std::swap(tEnterY, tExitY);

	float entryTime = std::max(tEnterX, tEnterY);
	float exitTime = std::min(tExitX, tExitY);

	// Reject if exit before entry, or exit is in the past, or entry is too far in future
	if (entryTime > exitTime || exitTime < -CollisionManager::EPSILON || entryTime > 1.0f)
		return false;

	// ✅ Allow t=0 contact as a valid collision
	tfirst = std::max(0.f, entryTime);
	tlast = std::min(1.f, exitTime);
	return true;
}


bool BoundingBox::IntersectsMoving(BoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return capsule->IntersectsMoving(static_cast<BoundingVolume*>(this), va, vb, tfirst, tlast);
}

Point BoundingBox::GetSeparationVector(BoundingBox* other)
{
	Point delta = other->GetPosition() - GetPosition();
	delta = { std::abs(delta.x), std::abs(delta.y) };
	Point overlap = (other->GetExtents() + m_extents) - delta;

	if (overlap.x < overlap.y)
	{
		float direction = (other->GetPosition().x < GetPosition().x) ? -1.0f : 1.0f;
		return { (overlap.x + BUFFER) * direction, 0 };
	}
	else
	{
		float direction = (other->GetPosition().y < GetPosition().y) ? -1.0f : 1.0f;
		return { 0, (overlap.y + BUFFER) * direction };
	}
}

Point BoundingBox::GetSeparationVector(BoundingCircle* other)
{
	Point circlePos = other->GetPosition();
	Point closestPoint = {
		std::max(m_min.x, std::min(circlePos.x, m_max.x)),
		std::max(m_min.y, std::min(circlePos.y, m_max.y))
	};

	Point displacement = circlePos - closestPoint;
	float distance = pnt::length(displacement);
	float penetrationDepth = other->GetRadius() - distance;

	// If overlapping and distance is meaningful
	if (penetrationDepth > 0.f && distance > std::numeric_limits<float>::epsilon())
	{
		return pnt::Normalize(displacement) * (penetrationDepth + BUFFER);
	}

	// If the circle's center is inside the box (distance ≈ 0), pick an arbitrary direction
	if (distance <= std::numeric_limits<float>::epsilon())
	{
		// Choose vertical push direction based on position relative to box center
		float pushDir = (circlePos.y < m_center.y) ? -1.f : 1.f;
		return Point(0.f, pushDir * (other->GetRadius() + BUFFER));
	}

	// No collision
	return Point(0.f, 0.f);
}

Point BoundingBox::GetSeparationVector(BoundingCapsule* other)
{
	Point closestPoint = other->GetSegment().ClosestPointOnLineSegment(GetCenter());
	Point clampedPoint = {
		std::max(m_min.x, std::min(closestPoint.x, m_max.x)),
		std::max(m_min.y, std::min(closestPoint.y, m_max.y))
	};

	Point displacement = closestPoint - clampedPoint;
	float distance = pnt::length(displacement);
	float penetrationDepth = other->GetRadius() - distance;

	if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
		return pnt::Normalize(displacement) * (penetrationDepth + BUFFER);

	if (distance <= std::numeric_limits<float>::epsilon())
	{
		Point centerDelta = other->GetPosition() - GetCenter();
		if (std::abs(centerDelta.y) > std::abs(centerDelta.x))
			return { 0.f, (centerDelta.y > 0.f ? 1.f : -1.f) * (other->GetRadius() + BUFFER) };
		else
			return { (centerDelta.x > 0.f ? 1.f : -1.f) * (other->GetRadius() + BUFFER), 0.f };
	}

	return { 0.f, 0.f };
}

BoundingCircle CalculateMinimumBoundingCircle(BoundingBox* box)
{
	const Point min = box->GetMin();
	const Point max = box->GetMax();
	const Point center = (min + max) * 0.5f;
	const float radius = std::sqrtf(pnt::lengthSquared(max - min)) * 0.5f;
	return BoundingCircle(radius, center);
}

Direction GetCollisionDirection(const Point& mtv, const Point& velA, const Point& velB)
{
	const Point relativeVelocity = velA - velB;

	if (std::abs(mtv.x) > 0 && std::abs(mtv.y) == 0)
		return (relativeVelocity.x > 0) ? Direction::LDIR : Direction::RDIR;
	if (std::abs(mtv.y) > 0 && std::abs(mtv.x) == 0)
		return (relativeVelocity.y > 0) ? Direction::DDIR : Direction::UDIR;

	return (std::abs(mtv.x) > std::abs(mtv.y))
		? ((relativeVelocity.x > 0) ? Direction::LDIR : Direction::RDIR)
		: ((relativeVelocity.y > 0) ? Direction::DDIR : Direction::UDIR);
}

int BoundingCircle::s_count = 0;

BoundingCircle::BoundingCircle()
	: BoundingCircle(8.0f, Point(10, 10))
{}

BoundingCircle::BoundingCircle(float radius)
	: BoundingCircle(radius, Point(10, 10))
{}

BoundingCircle::BoundingCircle(float radius, const Point& pos)
	: BoundingVolume(VolumeType::Circle), m_radius(radius)
{
	m_circleNumber = s_count++;
	MakeCircleShape();
	Reset(radius);
	Update(pos);
}

void BoundingCircle::Reset(float radius)
{
	m_radius = radius * GetScale().x;
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
		return Intersects(static_cast<BoundingBox*>(volume));
	case VolumeType::Circle:
		return Intersects(static_cast<BoundingCircle*>(volume));
	case VolumeType::Capsule:
		return Intersects(static_cast<BoundingCapsule*>(volume));
	}

	return false;
}

bool BoundingCircle::IntersectsMoving(BoundingVolume* volume, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return IntersectsMoving(static_cast<BoundingBox*>(volume), va, vb, tfirst, tlast);
	case VolumeType::Circle:
		return IntersectsMoving(static_cast<BoundingCircle*>(volume), va, vb, tfirst, tlast);
	case VolumeType::Capsule:
		return IntersectsMoving(static_cast<BoundingCapsule*>(volume), va, vb, tfirst, tlast);
	}

	return false;
}

Point BoundingCircle::GetSeparationVector(BoundingVolume* volume)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return GetSeparationVector(static_cast<BoundingBox*>(volume));
	case VolumeType::Circle:
		return GetSeparationVector(static_cast<BoundingCircle*>(volume));
	case VolumeType::Capsule:
		return GetSeparationVector(static_cast<BoundingCapsule*>(volume));
	}

	return Point();
}

Point BoundingCircle::GetPoint(Side side)
{
	switch (side)
	{
	case Side::Left:
		return m_center - Point(m_radius, 0);
	case Side::Right:
		return m_center + Point(m_radius, 0);
	case Side::Top:
		return m_center - Point(0, m_radius);
	case Side::Bottom:
		return m_center + Point(0, m_radius);
	default:
		throw std::out_of_range("Side enum value doesn't exist");
	}
}

void BoundingCircle::SetScale(const Point& scale)
{
	BoundingVolume::SetScale(scale);
	Reset(GetCircle()->getRadius());
}

bool BoundingCircle::Intersects(BoundingBox* box)
{
	return box->Intersects(static_cast<BoundingVolume*>(this));
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
	return box->IntersectsMoving(static_cast<BoundingVolume*>(this), va, vb, tfirst, tlast);
}

bool BoundingCircle::IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	Point s = GetCenter() - circle->GetCenter();
	float r = circle->m_radius + m_radius;

	Point v = vb - va; // Relative motion
	float a = pnt::dot(v, v);

	if (a < CollisionManager::EPSILON) return false; // No relative motion

	float b = pnt::dot(v, s);
	if (b >= 0.0f) return false; // Moving away

	float c = pnt::dot(s, s) - r * r;
	if (c < -CollisionManager::EPSILON) // Initial overlap case
	{
		tfirst = tlast = 0.0f;
		return true;
	}

	float d = b * b - a * c;
	if (d < 0.0f) return false; // No real solution

	float sqrtD = std::sqrt(d);
	tfirst = (-b - sqrtD) / a;
	tlast = (-b + sqrtD) / a;

	if (tlast < 0.0f || tfirst > 1.0f) return false;
	if (tfirst > tlast) return false;

	// If tfirst is negative, clamp to zero
	if (tfirst < 0.0f && tlast >= 0.0f)
		tfirst = 0.0f;

	return true;
}

bool BoundingCircle::IntersectsMoving(BoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	return capsule->IntersectsMoving(static_cast<BoundingVolume*>(this), va, vb, tfirst, tlast);
}

Point BoundingCircle::GetSeparationVector(BoundingBox* other)
{
	return other->GetSeparationVector(static_cast<BoundingVolume*>(this));
}

Point BoundingCircle::GetSeparationVector(BoundingCircle* other)
{
	Point displacement = other->GetPosition() - GetPosition();
	float distance = pnt::length(displacement);
	float radiusSum = other->GetRadius() + GetRadius();
	float penetrationDepth = radiusSum - distance;

	if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
		return pnt::Normalize(displacement) * (penetrationDepth + BUFFER);

	if (distance <= std::numeric_limits<float>::epsilon())
		return { 0.f, (other->GetPosition().y > GetPosition().y ? 1.f : -1.f) * (radiusSum + BUFFER) };

	return { 0.f, 0.f };
}

Point BoundingCircle::GetSeparationVector(BoundingCapsule* other)
{
	Point closestPoint = other->GetSegment().ClosestPointOnLineSegment(GetPosition());
	Point displacement = GetPosition() - closestPoint;
	float distance = pnt::length(displacement);
	float radiusSum = GetRadius() + other->GetRadius();
	float penetrationDepth = radiusSum - distance;

	if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
		return pnt::Normalize(displacement) * (penetrationDepth + BUFFER);

	if (distance <= std::numeric_limits<float>::epsilon())
		return { 0.f, (GetPosition().y > other->GetPosition().y ? 1.f : -1.f) * (radiusSum + BUFFER) };

	return { 0.f, 0.f };
}

int BoundingCapsule::s_count = 0;

BoundingCapsule::BoundingCapsule()
	: BoundingCapsule(8.0f, 16.0f, 0.0f, Point(8.0f, 16.0f))
{}

BoundingCapsule::BoundingCapsule(float radius, float length, float angle)
	: BoundingCapsule(radius, length, angle, Point(radius, radius * 2.f))
{}

BoundingCapsule::BoundingCapsule(float radius, float length, float angle, const Point& pos)
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
	m_capsuleNumber = s_count++;
	MakeCapsuleShape();
	Reset(m_radius, pnt::distance(segment.start, segment.end), m_angle);

	Update(segment.GetMidPoint());
}

void BoundingCapsule::Reset(float radius, float length, float angle)
{
	const auto scale = GetScale();
	m_length = length * scale.y;
	m_radius = radius * scale.x;

	auto rect = GetRect();
	rect->setSize(Point(radius * 2.f, length));
	rect->setOrigin({ radius, length / 2.f });
	rect->setRotation(sf::degrees(angle));

	m_circle1.setRadius(radius);
	m_circle1.setOrigin({ radius, radius });

	m_circle2.setRadius(radius);
	m_circle2.setOrigin({ radius, radius });
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
	auto scale = GetScale();
	size.x *= scale.x;
	size.y *= scale.y;

	CalculateRotatedRectangleCorners(corners, GetCenter(), size, m_angle);

	// Calculate the midpoint of the top side
	m_circle1.setPosition(Line(corners[3], corners[2]).GetMidPoint());
	// Calculate the midpoint of the top side
	m_circle2.setPosition(Line(corners[1], corners[0]).GetMidPoint());

	m_segment.start = m_circle1.getPosition();
	m_segment.end = m_circle2.getPosition();
}

bool BoundingCapsule::Intersects(BoundingVolume* volume)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return Intersects(static_cast<BoundingBox*>(volume));
	case VolumeType::Circle:
		return Intersects(static_cast<BoundingCircle*>(volume));
	case VolumeType::Capsule:
		return Intersects(static_cast<BoundingCapsule*>(volume));
	}

	return false;
}

bool BoundingCapsule::IntersectsMoving(BoundingVolume* volume, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return IntersectsMoving(static_cast<BoundingBox*>(volume), va, vb, tfirst, tlast);
	case VolumeType::Circle:
		return IntersectsMoving(static_cast<BoundingCircle*>(volume), va, vb, tfirst, tlast);
	case VolumeType::Capsule:
		return IntersectsMoving(static_cast<BoundingCapsule*>(volume), va, vb, tfirst, tlast);
	}

	return false;
}

Point BoundingCapsule::GetSeparationVector(BoundingVolume* volume)
{
	switch (volume->GetType())
	{
	case VolumeType::Box:
		return GetSeparationVector(static_cast<BoundingBox*>(volume));
	case VolumeType::Circle:
		return GetSeparationVector(static_cast<BoundingCircle*>(volume));
	case VolumeType::Capsule:
		return GetSeparationVector(static_cast<BoundingCapsule*>(volume));
	}

	return Point();
}

Point BoundingCapsule::GetPoint(Side side)
{
	return Point();
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
	if (circle.IntersectsMoving(static_cast<BoundingVolume*>(box), va, vb, tfirst, tlast))
		return true;

	circle.Update(m_segment.end);
	if (circle.IntersectsMoving(static_cast<BoundingVolume*>(box), va, vb, tfirst, tlast))
		return true;

	BoundingBox capBox(this);
	return capBox.IntersectsMoving(static_cast<BoundingVolume*>(box), va, vb, tfirst, tlast);
}

bool BoundingCapsule::IntersectsMoving(BoundingCircle* circle, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	// check the capsule spherical ends
	BoundingCircle capCircle(circle->GetRadius(), m_segment.start);
	if (static_cast<BoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<BoundingVolume*>(circle), va, vb, tfirst, tlast))
		return true;

	capCircle.Update(m_segment.end);

	if (static_cast<BoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<BoundingVolume*>(circle), va, vb, tfirst, tlast))
		return true;

	capCircle.Update(m_segment.GetMidPoint());

	if (static_cast<BoundingVolume*>(&capCircle)->IntersectsMoving(static_cast<BoundingVolume*>(circle), va, vb, tfirst, tlast))
		return true;

	return m_segment.IntersectsMoving(circle, va, vb, tfirst, tlast);
}

bool BoundingCapsule::IntersectsMoving(BoundingCapsule* capsule, const Point& va, const Point& vb, float& tfirst, float& tlast)
{
	BoundingCircle endpoint1(m_radius, m_segment.start);
	BoundingCircle endpoint2(m_radius, m_segment.end);

	BoundingCircle otherEndpoint1(m_radius, m_segment.start);
	BoundingCircle otherEndpoint2(m_radius, m_segment.end);

	if (endpoint1.IntersectsMoving(static_cast<BoundingVolume*>(&otherEndpoint2), va, vb, tfirst, tlast))
		return true;

	if (endpoint2.IntersectsMoving(static_cast<BoundingVolume*>(&otherEndpoint1), va, vb, tfirst, tlast))
		return true;

	if (endpoint2.IntersectsMoving(static_cast<BoundingVolume*>(&otherEndpoint2), va, vb, tfirst, tlast))
		return true;

	if (endpoint1.IntersectsMoving(static_cast<BoundingVolume*>(&otherEndpoint1), va, vb, tfirst, tlast))
		return true;

	BoundingBox box1 = BoundingBox(this);
	BoundingBox box2 = BoundingBox(capsule);

	return box1.IntersectsMoving(static_cast<BoundingVolume*>(&box2), va, vb, tfirst, tlast);
}

Point BoundingCapsule::GetSeparationVector(BoundingBox* other)
{
	return other->GetSeparationVector(static_cast<BoundingVolume*>(this));
}

Point BoundingCapsule::GetSeparationVector(BoundingCircle* other)
{
	return other->GetSeparationVector(static_cast<BoundingVolume*>(this));
}

Point BoundingCapsule::GetSeparationVector(BoundingCapsule* other)
{
	Point closest1 = m_segment.ClosestPointOnLineSegment(other->GetSegment().start);
	Point closest2 = other->GetSegment().ClosestPointOnLineSegment(closest1);
	closest1 = m_segment.ClosestPointOnLineSegment(closest2);

	Point displacement = closest2 - closest1;
	float distance = pnt::length(displacement);
	float radiusSum = GetRadius() + other->GetRadius();
	float penetrationDepth = radiusSum - distance;

	if (penetrationDepth > 0.0f && distance > std::numeric_limits<float>::epsilon())
		return pnt::Normalize(displacement) * (penetrationDepth + BUFFER);

	if (distance <= std::numeric_limits<float>::epsilon())
		return { 0.f, (other->GetPosition().y > GetPosition().y ? 1.f : -1.f) * (radiusSum + BUFFER) };

	return { 0.f, 0.f };
}

void BoundingCapsule::SetScale(const Point& scale)
{
	BoundingVolume::SetScale(scale);
	m_circle1.setScale(scale);
	m_circle2.setScale(scale);
	Reset(m_circle1.getRadius(), GetRect()->getSize().y, m_angle);
}

void BoundingCapsule::MakeCapsuleShape()
{
	MakeRectangleShape();
	const auto scale = GetScale();
	SetupShape(m_circle1, scale);
	SetupShape(m_circle2, scale);
}

void BoundingVolume::SetScale(const Point& scale)
{
	m_scale = scale;
	m_shape->setScale(scale);
}
