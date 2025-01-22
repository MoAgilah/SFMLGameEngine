#include "CollisionManager.h"
#include <format>
#include <algorithm>
#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Box.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"
#include "../GameObjects/Collectables.h"
#include "../Utilities/Utilities.h"

namespace
{
	std::vector<std::string> canCollideWithTile =
	{
	};

	bool CanCollideWithTile(const std::string& texID)
	{
		return std::find(canCollideWithTile.begin(), canCollideWithTile.end(), texID) != canCollideWithTile.end();
	}

	void SortCollidedTiles(std::vector<std::shared_ptr<Tile>> collidedWith)
	{
		std::ranges::sort(collidedWith, [](const std::shared_ptr<Tile>& a, const std::shared_ptr<Tile>& b)
			{
				if (a->GetColNum() == b->GetColNum())
					return a->GetRowNum() < b->GetRowNum();

				return a->GetColNum() > b->GetColNum();
			});
	}

	std::vector<std::string> dynamicCollectables =
	{
	};

	bool IsDynamicCollectable(const std::string& texID)
	{
		return std::find(dynamicCollectables.begin(), dynamicCollectables.end(), texID) != dynamicCollectables.end();
	}

	std::vector<std::string> dynamicObject =
	{
	};

	bool IsDynamicObject(const std::string& texId)
	{
		return std::find(dynamicObject.begin(), dynamicObject.end(), texId) != dynamicObject.end();
	}

	float GetYOffSet(float pDistX, float lDistY, float slopeY, float currY, float tileHeight)
	{
		auto percent = pDistX / lDistY;
		auto colHeight = lDistY * percent + slopeY;
		return ((currY - colHeight) / 16);
	}

	bool IsMovingTowards(Point p1, Point p2, Point v1, Point v2)
	{
		Point v = v2 - v1;
		Point d = p2 - p1;

		return (d.x * v.x + d.y * v.y) < 0;
	}

	Point GetPreviousOverlap(BoundingBox* box, DynamicObject* obj)
	{
		Point delta = box->GetPosition() - obj->GetPrevPosition();
		delta = Point(std::abs(delta.x), std::abs(delta.y));
		return (obj->GetBoundingBox()->GetExtents() + box->GetExtents()) - delta;
	}
}

CollisionManager::CollisionManager()
{
	for (auto& tile : m_grid.GetGrid())
	{
		if (tile->GetType() == EMPTY)
			continue;

		m_tiles.push_back(tile);
	}
}

void CollisionManager::AddCollidable(Object* go)
{
	m_collidables.push_back(go);
}

void CollisionManager::RemoveLastAdded()
{
	m_collidables.pop_back();
}

Object* CollisionManager::GetLastAdded()
{
	return m_collidables.back();
}

void CollisionManager::Render(sf::RenderWindow& window)
{
	m_grid.Render(window);
}

void CollisionManager::ProcessCollisions(Object* gobj)
{
	if (CanCollideWithTile(gobj->GetID()))
		DynamicObjectToTileCollisions((DynamicObject*)gobj);

	for (size_t i = 0; i < m_collidables.size(); i++)
	{
		if (!m_collidables[i]->GetActive())
			continue;

		if (gobj->GetObjectNum() == m_collidables[i]->GetObjectNum())
			continue;

		ObjectToObjectCollisions(gobj, m_collidables[i]);
	}
}

Tile* CollisionManager::GetTile(int x, int y)
{
	return m_grid.GetTile(x, y);
}

std::vector<std::shared_ptr<Tile>> CollisionManager::GetGrid()
{
	return m_grid.GetGrid();
}

std::vector<Object*> CollisionManager::GetCollidables()
{
	return m_collidables;
}

void CollisionManager::DynamicObjectToTileCollisions(DynamicObject* obj)
{
	bool collided = false;
	std::vector<std::shared_ptr<Tile>> collidedWith;

	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (tile->GetBoundingBox()->Intersects(obj->GetBoundingBox()))
			collidedWith.push_back(tile);
	}

	if (collided = !collidedWith.empty())
	{
		if (obj->GetDirection())
			SortCollidedTiles(collidedWith);

		for (auto& tile : collidedWith)
			if (tile->GetBoundingBox()->Intersects(obj->GetBoundingBox()))
				DynamicObjectToTileResolution(obj, tile.get());
	}

	if (!collided)
	{
		obj->SetOnGround(false);
		obj->SetOnSlope(false);
	}
}

void CollisionManager::DynamicObjectToTileResolution(DynamicObject* obj, Tile* tile)
{
	Direction dir = GetFacingDirection(obj);
	Line tileTopEdge = tile->GetBoundingBox()->GetSide(Side::Top);
	Point objBottomPoint = obj->GetBoundingBox()->GetPoint(Side::Bottom);

	switch (tile->GetType())
	{
	case Types::OWAY:
	{
		if (dir == DDIR || dir == LDIR || dir == RDIR)
		{
			/*Capsule capsule(tileTopEdge, 6);
			Circle circle(obj->GetBoundingBox(), 4);
			if (capsule.IntersectsCircle(circle))
			{
				if (tileTopEdge.IsPointAboveLine(objBottomPoint))
					ResolveObjectToBoxTop(obj, tile->GetBoundingBox());
			}*/
		}
		return;
	}
	case Types::GRND:
		if (dir == DDIR || dir == LDIR || dir == RDIR)
		{
			if (tileTopEdge.IsPointAboveLine(objBottomPoint))
				ResolveObjectToBoxTop(obj, tile->GetBoundingBox());
		}
		return;
	case Types::LCRN:
		[[fallthrough]];
	case Types::RCRN:
	{
		Point prevOverlap = GetPreviousOverlap(tile->GetBoundingBox(), obj);

		if (dir == DDIR)
		{
			// the collision came from a vertical direction
			if (prevOverlap.x > 0)
			{
				// the collision came from the bottom
				if (prevOverlap.x > prevOverlap.y)
				{
					if (tileTopEdge.IsPointAboveLine(objBottomPoint))
					{
						ResolveObjectToBoxTop(obj, tile->GetBoundingBox());
						return;
					}
				}
			}
		}

		// the collision came from a horizontal direction
		if (prevOverlap.y > 0)
		{
			ResolveObjectToBoxHorizontally(obj, tile->GetBoundingBox());
		}
		else
		{
			if (dir == LDIR || dir == RDIR)
			{
				if (tileTopEdge.IsPointAboveLine(objBottomPoint))
				{
					ResolveObjectToBoxTop(obj, tile->GetBoundingBox());
				}

				DynamicObjectToEdgeBounds(obj, tile);
			}
		}
		return;
	}
	case Types::WALL:
		ResolveObjectToBoxHorizontally(obj, tile->GetBoundingBox());
		return;
	case Types::DIAGU:
	{
		switch (dir)
		{
		case DDIR:
			if (ResolveObjectToSlopeTop(obj, tile))
			{
				if (!obj->GetShouldSlideLeft())
					obj->SetShouldSlideLeft(true);
			}
			break;
		case RDIR:
			if (ResolveObjectToSlopeIncline(obj, tile, 0, 1))
			{
				if (!obj->GetShouldSlideLeft())
					obj->SetShouldSlideLeft(true);
			}
			break;
		case LDIR:
			if (ResolveObjectToSlopeDecline(obj, tile, 1, 0))
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
			if (ResolveObjectToSlopeTop(obj, tile))
			{
				if (!obj->GetShouldSlideRight())
					obj->SetShouldSlideRight(true);
			}
			break;
		case LDIR:
			if (ResolveObjectToSlopeIncline(obj, tile, 1, 0))
			{
				if (!obj->GetShouldSlideRight())
					obj->SetShouldSlideRight(true);
			}
			break;
		case RDIR:
			if (ResolveObjectToSlopeDecline(obj, tile, 0, 1))
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

void CollisionManager::PlayerToObjectCollisions(DynamicObject* ply, Object* obj)
{
	if (IsBoxObject(obj->GetID()))
	{
		if (obj->Intersects(ply))
		{
		}
	}
	else if (IsCollectableObject(obj->GetID()))
	{
		if (obj->Intersects(ply))
		{
			if (IsDynamicCollectable(obj->GetID()))
				((DynamicCollectable*)obj)->Collect(ply);
			else
				((StaticCollectable*)obj)->Collect(ply);
		}
	}
	else if (IsEnemyObject(obj->GetID()))
	{
		Enemy* enemy = (Enemy*)obj;
		if (!enemy->HasLifes())
			return;

		if (obj->Intersects(ply))
			PlayerToEnemyResolutions(ply, (Enemy*)obj);
	}
}

void CollisionManager::ObjectToObjectCollisions(Object* obj1, Object* obj2)
{
	if (IsPlayerObject(obj1->GetID()))
	{
		PlayerToObjectCollisions((DynamicObject*)obj1, obj2);
	}
	else if (IsPlayerObject(obj2->GetID()))
	{
		PlayerToObjectCollisions((DynamicObject*)obj2, obj1);
	}
	else if (IsBoxObject(obj2->GetID()))
	{
		if (obj2->Intersects(obj1))
		{
			DynamicObject* dynObj = (DynamicObject*)obj1;

			Point prevOverlap = GetPreviousOverlap(obj2->GetBoundingBox(), dynObj);

			DynamicObjectToBoxResolutions(GetFacingDirection(dynObj), prevOverlap, dynObj, obj2->GetBoundingBox());
		}
	}
	else
	{
		if (IsDynamicObject(obj1->GetID()) && IsDynamicObject(obj2->GetID()))
			DynamicObjectToDynamicObjectCollisions((DynamicObject*)obj1, (DynamicObject*)obj2);
	}
}

void CollisionManager::PlayerToEnemyResolutions(DynamicObject* ply, Enemy* enmy)
{
}

void CollisionManager::DynamicObjectToBoxResolutions(Direction dirOfTravel, const Point& prevOverlap, DynamicObject* obj, BoundingBox* box, bool resolveUpDir)
{
	// the collision came from a horizontal direction
	if (prevOverlap.y > 0)
	{
		// there is no collision vertically
		if (prevOverlap.x <= 0)
		{
			ResolveObjectToBoxHorizontally(obj, box);
			return;
		}
	}

	// the collision came from a vertical direction
	if (prevOverlap.x > 0)
	{
		if (dirOfTravel == Direction::UDIR)
		{
			if (resolveUpDir)
			{
				// collision came from the bottom
				if (prevOverlap.x > prevOverlap.y)
				{
					ResolveObjectToBoxBottom(obj, box);
					return;
				}
			}
		}
		else
		{
			ResolveObjectToBoxTop(obj, box);
		}
	}
}

void CollisionManager::DynamicObjectToDynamicObjectResolution(DynamicObject* obj, float tFirst)
{
	// get amount to move the object out of collision
	Point move = Point(std::lerp(obj->GetPrevPosition().x, obj->GetPosition().y, tFirst),
		std::lerp(obj->GetPrevPosition().y, obj->GetPosition().y, tFirst));

	Point amt = obj->GetPosition() - move;

	// resolve collision and invert direction of travel and associated velocity
	obj->Move(amt * (obj->GetDirection() ? 1.f : -1.f));
	obj->SetDirection(!obj->GetDirection());
	obj->SetVelocity(obj->GetVelocity().x * -1, obj->GetVelocity().x * -1);
}

void CollisionManager::DynamicObjectToDynamicObjectCollisions(DynamicObject* obj1, DynamicObject* obj2)
{
	if (!IsMovingTowards(obj1->GetBoundingBox()->GetPosition(), obj2->GetBoundingBox()->GetPosition(), obj1->GetVelocity(), obj2->GetVelocity()))
		return;

	float tFirst, tLast = 0;
	if (obj1->GetBoundingBox()->IntersectsMoving(obj2->GetBoundingBox(), obj1->GetVelocity(), obj2->GetVelocity(), tFirst, tLast))
	{
		DynamicObjectToDynamicObjectResolution(obj1, tFirst);
		DynamicObjectToDynamicObjectResolution(obj2, tFirst);
	}
}

void CollisionManager::ResolveObjectToBoxTop(DynamicObject* obj, BoundingBox* box)
{
	obj->Move(0, -box->GetOverlap().y);
	obj->SetOnGround(true);
	obj->SetOnSlope(false);
}

void CollisionManager::ResolveObjectToBoxBottom(DynamicObject* obj, BoundingBox* box)
{
	obj->Move(0, box->GetOverlap().y);
}

void CollisionManager::ResolveObjectToBoxHorizontally(DynamicObject* obj, BoundingBox* box)
{
	obj->Move((obj->GetDirection() ? -1 : 1) * box->GetOverlap().x, 0);
}

bool CollisionManager::ResolveObjectToSlopeTop(DynamicObject* obj, Tile* tile)
{
	Line line = tile->GetSlope(0, 1);
	/*Circle circle(obj->GetBoundingBox(), 4);
	if (line.IsPointAboveLine(circle.center))
	{
		Capsule capsule(line, 6);
		if (capsule.IntersectsCircle(circle))
		{
			obj->SetOnSlope(true);
			return true;
		}
	}*/

	return false;
}

bool CollisionManager::ResolveObjectToSlopeIncline(DynamicObject* obj, Tile* tile, int start, int end)
{
	Line line = tile->GetSlope(start, end);
	/*Circle circle(obj->GetBoundingBox(), 4);
	Capsule capsule(line, 6);
	if (capsule.IntersectsCircle(circle))
	{
		auto yOffset = GetYOffSet(start ? GetXDist(circle.center, line.start) : GetXDist(line.start, circle.center),
			line.DistY(),
			line.start.y,
			obj->GetBoundingBox()->GetPosition().y,
			tile->GetTileHeight());

		obj->Move(sf::Vector2f(0, yOffset));
		obj->SetOnSlope(true);

		return true;
	}*/


	return false;
}

bool CollisionManager::ResolveObjectToSlopeDecline(DynamicObject* obj, Tile* tile, int start, int end)
{
	Line line = tile->GetSlope(start, end);
	/*Circle circle(obj->GetBoundingBox(), 4);
	Capsule capsule(line, 6);
	if (!capsule.IntersectsCircle(circle))
	{
		auto yOffset = GetYOffSet(start ? GetXDist(circle.center, line.start) : GetXDist(line.start, circle.center),
			line.DistY(),
			line.start.y,
			obj->GetBoundingBox()->GetPosition().y,
			tile->GetTileHeight());

		obj->Move(sf::Vector2f(0, -yOffset));
		obj->SetOnSlope(true);

		return true;
	}*/

	return false;
}

void CollisionManager::DynamicObjectToEdgeBounds(DynamicObject* obj, Tile* tile)
{
	if (IsPlayerObject(obj->GetID()))
		return;

	Point side;
	if (tile->GetType() == LCRN)
		side = obj->GetBoundingBox()->GetPoint(Side::Right);
	else
		side = obj->GetBoundingBox()->GetPoint(Side::Left);

	Line edge = tile->GetEdge();

	if (IsMovingTowards(edge.start, side, Point(0, 0), obj->GetVelocity()))
	{
		/*Circle circle(side, 4);
		Capsule capsule(edge, 4);
		if (capsule.IntersectsCircle(circle))
			obj->SetDirection(!obj->GetDirection());*/
	}
}

Direction CollisionManager::GetFacingDirection(DynamicObject* obj)
{
	const sf::Vector2f& currentVel = obj->GetVelocity();

	Direction currentDir = DDIR;

	if (currentVel.x != 0.f || currentVel.y != 0.f)
	{
		float vxa = std::abs(currentVel.x);
		float vya = std::abs(currentVel.y);

		if (vxa > vya)
		{
			currentDir = (currentVel.x < 0) ?
				Direction::LDIR : Direction::RDIR;
		}
		else
		{
			currentDir = (currentVel.y < 0) ?
				Direction::UDIR : Direction::DDIR;
		}
	}

	return currentDir;
}