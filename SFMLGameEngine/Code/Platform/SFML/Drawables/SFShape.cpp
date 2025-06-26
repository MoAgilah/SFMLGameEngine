#include "SFShape.h"

#include <numbers>

void SFShape::Move(float x, float y)
{
	Move(Point(x, y));
}

void SFShape::Move(const Point& pos)
{
	m_drawable->move(pos);
}

SFRect::SFRect(const Point& size)
{
	m_drawable = std::make_shared<sf::RectangleShape>(size);
}

void SFRect::Update(float deltaTime)
{
	UpdateShape(GetPosition());
}

void SFRect::Reset(const Point& size)
{
	GetRect()->setSize(size);
	const auto scale = GetScale();
	m_extents[0] = (size.x * scale.x) * 0.5f;
	m_extents[1] = (size.y * scale.y) * 0.5f;
	SetOrigin(size * 0.5f);
}

void SFRect::UpdateShape(const Point& pos)
{
	SetPosition(pos);
	m_min = GetPosition() - m_extents;
	m_max = GetPosition() + m_extents;
}

SFCircle::SFCircle(float radius)
	: m_radius(radius)
{
	m_drawable = std::make_shared<sf::CircleShape>(radius);
	Reset(radius);
}

void SFCircle::Update(float deltaTime)
{
	UpdateShape(GetPosition());
}

void SFCircle::Reset(float radius)
{
	m_radius = radius * GetScale().x;
	GetCircle()->setRadius(radius);
	SetOrigin(Point(radius, radius));
}

void SFCircle::UpdateShape(const Point& pos)
{
	SetPosition(pos);
}

SFCapsule::SFCapsule(float radius, float length, float angle)
	: m_radius(radius), m_angle(angle)
{
	m_drawable = std::make_shared<sf::RectangleShape>();
	Reset(m_radius, length, m_angle);
}

void SFCapsule::Update(float deltaTime)
{
	UpdateShape(GetPosition());
}

void SFCapsule::Reset(float radius, float length, float angle)
{
	const auto scale = GetScale();
	m_length = length * scale.y;
	m_radius = radius * scale.x;

	auto body = static_cast<sf::RectangleShape*>(m_drawable.get());
	body->setSize(Point(radius * 2.f, length));
	body->setOrigin({ radius, length / 2.f });
	body->setRotation(sf::degrees(angle));

	m_circle1.setRadius(radius);
	m_circle1.setOrigin({ radius, radius });

	m_circle2.setRadius(radius);
	m_circle2.setOrigin({ radius, radius });
}

namespace
{
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
}

void SFCapsule::UpdateShape(const Point& pos)
{
	SetPosition(pos);

	Point corners[4];
	auto size = static_cast<sf::RectangleShape*>(m_drawable.get())->getSize();
	auto scale = GetScale();
	size.x *= scale.x;
	size.y *= scale.y;

	CalculateRotatedRectangleCorners(corners, GetPosition(), size, m_angle);

	// Calculate the midpoint of the top side
	m_circle1.setPosition(Line(corners[3], corners[2]).GetMidPoint());
	// Calculate the midpoint of the top side
	m_circle2.setPosition(Line(corners[1], corners[0]).GetMidPoint());

	m_segment.start = m_circle1.getPosition();
	m_segment.end = m_circle2.getPosition();
}