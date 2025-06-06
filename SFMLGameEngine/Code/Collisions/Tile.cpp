#include "Tile.h"

#include "../Game/Constants.h"
#include "../GameObjects/Object.h"
#include <format>

using namespace obj;

Tile::Tile()
{
	m_aabb.SetOutlineColour(sf::Color::Black);
}

Tile::Tile(int x, int y, sf::Font* font)
	: m_hasFont(true)
{
	m_text = std::make_unique<sf::Text>(*font, std::format("{}\n{}", m_colNum, m_rowNum), 12);

	m_colNum = x;
	m_rowNum = y;

	m_id = std::format("{},{}", m_colNum, m_rowNum);

	m_aabb.SetOutlineColour(sf::Color::Black);

	m_text->setOrigin({ 6, 6 });
	m_text->setStyle(sf::Text::Bold);
}

bool Tile::Intersects(DynamicObject* obj)
{
	return m_aabb.Intersects(obj->GetColVolume());
}

void Tile::ResolveCollision(DynamicObject* obj)
{
	Direction dir = obj->GetFacingDirection();
	Line tileTopEdge = m_aabb.GetSide(Side::Top);
	Point objBottomPoint = obj->GetColVolume()->GetPoint(Side::Bottom);

	switch (GetType())
	{
	case Types::OWAY:
	{
		if (dir == DDIR || dir == LDIR || dir == RDIR)
		{
			BoundingCapsule capsule(6, tileTopEdge);
			BoundingCircle circle(4, obj->GetBoundingBox()->GetPoint(Side::Bottom));
			if (capsule.Intersects(static_cast<BoundingVolume*>(&circle)))
			{
				if (tileTopEdge.IsPointAboveLine(objBottomPoint))
					ResolveObjectToBoxTop(obj);
			}
		}
		return;
	}
	case Types::GRND:
		if (dir == DDIR || dir == LDIR || dir == RDIR)
		{
			if (tileTopEdge.IsPointAboveLine(objBottomPoint))
				ResolveObjectToBoxTop(obj);
		}
		return;
	case Types::LCRN:
		[[fallthrough]];
	case Types::RCRN:
	{
		Point seperationVector = obj->GetColVolume()->GetSeparationVector(static_cast<BoundingVolume*>(&m_aabb));
		Direction colDir = GetCollisionDirection(seperationVector, obj->GetVelocity(), Point());

		if (dir == DDIR)
		{
			// the collision came from a vertical direction
			if (colDir == DDIR)
			{
				if (tileTopEdge.IsPointAboveLine(objBottomPoint))
				{
					ResolveObjectToBoxTop(obj);
					return;
				}
			}
		}

		// the collision came from a horizontal direction
		if (colDir == LDIR || colDir == RDIR)
		{
			ResolveObjectToBoxHorizontally(obj);
		}
		else
		{
			if (dir == LDIR || dir == RDIR)
			{
				if (tileTopEdge.IsPointAboveLine(objBottomPoint))
				{
					ResolveObjectToBoxTop(obj);
				}

				ResolveObjectToEdgeBounds(obj);
			}
		}
		return;
	}
	case Types::WALL:
		ResolveObjectToBoxHorizontally(obj);
		return;
	case Types::DIAGU:
	{
		switch (dir)
		{
		case DDIR:
			if (ResolveObjectToSlopeTop(obj))
			{
				if (!obj->GetShouldSlideLeft())
					obj->SetShouldSlideLeft(true);
			}
			break;
		case RDIR:
			if (ResolveObjectToSlopeIncline(obj, 0, 1))
			{
				if (!obj->GetShouldSlideLeft())
					obj->SetShouldSlideLeft(true);
			}
			break;
		case LDIR:
			if (ResolveObjectToSlopeDecline(obj, 1, 0))
			{
				if (!obj->GetShouldSlideLeft())
					obj->SetShouldSlideLeft(true);
			}
			break;
		}
		return;
	}
	case Types::DIAGD:
	{
		switch (dir)
		{
		case DDIR:
			if (ResolveObjectToSlopeTop(obj))
			{
				if (!obj->GetShouldSlideRight())
					obj->SetShouldSlideRight(true);
			}
			break;
		case LDIR:
			if (ResolveObjectToSlopeIncline(obj, 1, 0))
			{
				if (!obj->GetShouldSlideRight())
					obj->SetShouldSlideRight(true);
			}
			break;
		case RDIR:
			if (ResolveObjectToSlopeDecline(obj, 0, 1))
			{
				if (!obj->GetShouldSlideRight())
					obj->SetShouldSlideRight(true);
			}
			break;
		}
		return;
	}
	}
}

void Tile::SetPosition(const Point& pos)
{
	m_aabb.Update(pos);
	if (m_type == DIAGU)
	{
		m_slope.setPointCount(3);
		m_slope.setPoint(0, m_aabb.GetPosition() + Point(-m_aabb.GetExtents().x, m_aabb.GetExtents().y));
		m_slope.setPoint(1, m_aabb.GetPosition() + Point(m_aabb.GetExtents().x, -m_aabb.GetExtents().y));
		m_slope.setPoint(2, m_aabb.GetPosition() + m_aabb.GetExtents());
		m_slope.setFillColor(sf::Color::Yellow);
	}

	if (m_type == DIAGD)
	{
		m_slope.setPointCount(3);
		m_slope.setPoint(0, m_aabb.GetPosition() - Point(m_aabb.GetExtents().x, m_aabb.GetExtents().y));
		m_slope.setPoint(1, m_aabb.GetPosition() + m_aabb.GetExtents());
		m_slope.setPoint(2, m_aabb.GetPosition() - Point(m_aabb.GetExtents().x, -m_aabb.GetExtents().y));
		m_slope.setFillColor(sf::Color::Yellow);
	}

	if (m_type == LCRN)
	{
		m_edge.start = m_aabb.GetMin() + Point(m_aabb.GetExtents().x * 2, 0);
		m_edge.end = m_edge.start - Point(0, GetTileHeight());
	}

	if (m_type == RCRN)
	{
		m_edge.start = m_aabb.GetMin();
		m_edge.end = m_edge.start - Point(0, GetTileHeight());
	}

	m_text->setPosition({ m_aabb.GetPosition().x - 10.f, m_aabb.GetPosition().y - 7.5f });
}

void Tile::Render(sf::RenderWindow& window)
{
	if (m_type == DIAGU || m_type == DIAGD)
		window.draw(m_slope);

	if (m_type == LCRN || m_type == RCRN)
	{
		sf::Vertex line[2];
		line[0].position = m_edge.start;
		line[0].color = sf::Color::Red;
		line[1].position = m_edge.end;
		line[1].color = sf::Color::Red;

		window.draw(line, 2, sf::PrimitiveType::Lines);
	}

	m_aabb.Render(window);

	if (m_hasFont)
	{
		const sf::Text* text = m_text.get();
		if (text)
		{
			window.draw(*text);
		}
	}
}

Point Tile::GetSeperationVector(DynamicObject* obj)
{
	return m_aabb.GetSeparationVector(obj->GetColVolume());
}

void Tile::ResolveObjectToBoxTop(DynamicObject* obj)
{
	Point seperationVector = GetSeperationVector(obj);

	obj->Move(0, -seperationVector.y);
	obj->SetOnGround(true);
	obj->SetOnSlope(false);
}

void Tile::ResolveObjectToBoxBottom(DynamicObject* obj)
{
	Point seperationVector = GetSeperationVector(obj);

	obj->Move(0, seperationVector.y);
}

void Tile::ResolveObjectToBoxHorizontally(DynamicObject* obj)
{
	Point seperationVector = GetSeperationVector(obj);

	obj->Move((obj->GetDirection() ? -1 : 1) * seperationVector.x, 0);
}

bool Tile::ResolveObjectToSlopeTop(DynamicObject* obj)
{
	Line line = GetSlope(0, 1);
	BoundingCircle circle(4, obj->GetColVolume()->GetPoint(Side::Bottom));

	if (line.IsPointAboveLine(circle.GetCenter()))
	{
		BoundingCapsule capsule(6, line);
		if (capsule.Intersects(static_cast<BoundingVolume*>(&circle)))
		{
			obj->SetOnSlope(true);
			return true;
		}
	}

	return false;
}

float GetYOffSet(float pDistX, float lDistY, float slopeY, float currY, float tileHeight)
{
	auto percent = pDistX / lDistY;
	auto colHeight = lDistY * percent + slopeY;
	return ((currY - colHeight) / tileHeight);
}

bool Tile::ResolveObjectToSlopeIncline(DynamicObject* obj, int start, int end)
{
	Line line = GetSlope(start, end);
	BoundingCircle circle(4, obj->GetColVolume()->GetPoint(Side::Bottom));
	BoundingCapsule capsule(6, line);
	if (capsule.Intersects(static_cast<BoundingVolume*>(&circle)))
	{
		auto yOffset = GetYOffSet(start ? GetXDist(circle.GetCenter(), line.start) : GetXDist(line.start, circle.GetCenter()),
			line.DistY(),
			line.start.y,
			obj->GetBoundingBox()->GetPosition().y,
			GetTileHeight());

		obj->Move(0, yOffset);
		obj->SetOnSlope(true);

		return true;
	}

	return false;
}

bool Tile::ResolveObjectToSlopeDecline(DynamicObject* obj, int start, int end)
{
	Line line = GetSlope(start, end);
	BoundingCircle circle(4, obj->GetColVolume()->GetPoint(Side::Bottom));
	BoundingCapsule capsule(6, line);
	if (capsule.Intersects(static_cast<BoundingVolume*>(&circle)))
	{
		auto yOffset = GetYOffSet(start ? GetXDist(circle.GetCenter(), line.start) : GetXDist(line.start, circle.GetCenter()),
			line.DistY(),
			line.start.y,
			obj->GetBoundingBox()->GetPosition().y,
			GetTileHeight());

		obj->Move(0, -yOffset);
		obj->SetOnSlope(true);

		return true;
	}

	return false;
}

void Tile::ResolveObjectToEdgeBounds(DynamicObject* obj)
{
	if (IsPlayerObject(obj->GetID()))
		return;

	Point side;
	if (m_type == LCRN)
		side = obj->GetBoundingBox()->GetPoint(Side::Right);
	else
		side = obj->GetBoundingBox()->GetPoint(Side::Left);

	Line edge = GetEdge();

	if (pnt::IsMovingTowards(edge.start, side, Point(0, 0), obj->GetVelocity()))
	{
		BoundingCircle circle(4, side);
		BoundingCapsule capsule(4, edge);
		if (capsule.Intersects(static_cast<BoundingVolume*>(&circle)))
			obj->SetDirection(!obj->GetDirection());
	}
}