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

std::vector<std::string> CollisionManager::s_canCollideWithTile = {};
std::vector<std::string> CollisionManager::s_dynamicCollectables = {};
std::vector<std::string> CollisionManager::s_dynamicObject = {};

CollisionManager::CollisionManager(int rows, int columns, const std::string& tileFilePaths)
	: m_grid(rows, columns, tileFilePaths)
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

bool CollisionManager::CanCollideWithTile(const std::string& texID)
{
	return std::find(s_canCollideWithTile.begin(), s_canCollideWithTile.end(), texID) != s_canCollideWithTile.end();
}

void CollisionManager::SortCollidedTiles(std::vector<std::shared_ptr<Tile>> collidedWith)
{
	std::ranges::sort(collidedWith, [](const std::shared_ptr<Tile>& a, const std::shared_ptr<Tile>& b)
		{
			if (a->GetColNum() == b->GetColNum())
				return a->GetRowNum() < b->GetRowNum();

			return a->GetColNum() > b->GetColNum();
		});
}

bool CollisionManager::IsDynamicCollectable(const std::string& texID)
{
	return std::find(s_dynamicCollectables.begin(), s_dynamicCollectables.end(), texID) != s_dynamicCollectables.end();
}

bool CollisionManager::IsDynamicObject(const std::string& texId)
{
	return std::find(s_dynamicObject.begin(), s_dynamicObject.end(), texId) != s_dynamicObject.end();
}

void CollisionManager::DynamicObjectToTileCollisions(DynamicObject* obj)
{
	bool collided = false;
	std::vector<std::shared_ptr<Tile>> collidedWith;

	for (auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (tile->GetBoundingBox()->Intersects(obj->GetColVolume()))
			collidedWith.push_back(tile);
	}

	if (collided = !collidedWith.empty())
	{
		if (obj->GetDirection())
			SortCollidedTiles(collidedWith);

		for (auto& tile : collidedWith)
			if (tile->Intersects(obj))
				tile->ResolveCollision(obj);
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
			DynamicObjectToBoxResolutions((DynamicObject*)obj2, (Box*)obj1);
		}
	}
	else
	{
		if (IsDynamicObject(obj1->GetID()) && IsDynamicObject(obj2->GetID()))
			DynamicObjectToDynamicObjectCollisions((DynamicObject*)obj1, (DynamicObject*)obj2);
	}
}

void CollisionManager::PlayerToObjectCollisions(DynamicObject* ply, Object* obj)
{
	if (IsBoxObject(obj->GetID()))
	{
		if (obj->Intersects(ply))
		{
			DynamicObjectToBoxResolutions(ply, (Box*)obj);
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
	else
	{
		if (IsDynamicObject(ply->GetID()) && IsDynamicObject(obj->GetID()))
			DynamicObjectToDynamicObjectCollisions(ply, (DynamicObject*)obj);
	}
}

void CollisionManager::PlayerToEnemyResolutions(DynamicObject* ply, Enemy* enmy)
{
	// add special instruction for enemy resolution
}

void CollisionManager::DynamicObjectToBoxResolutions(DynamicObject* ply, Box* box, bool resolveUpDir)
{
	// utilise tile for resolution
}

//void CollisionManager::DynamicObjectToBoxResolutions(Direction dirOfTravel, const Point& prevOverlap, DynamicObject* obj, BoundingBox* box, bool resolveUpDir)
//{
//	// the collision came from a horizontal direction
//	if (prevOverlap.y > 0)
//	{
//		// there is no collision vertically
//		if (prevOverlap.x <= 0)
//		{
//			ResolveObjectToBoxHorizontally(obj, box);
//			return;
//		}
//	}
//
//	// the collision came from a vertical direction
//	if (prevOverlap.x > 0)
//	{
//		if (dirOfTravel == Direction::UDIR)
//		{
//			if (resolveUpDir)
//			{
//				// collision came from the bottom
//				if (prevOverlap.x > prevOverlap.y)
//				{
//					ResolveObjectToBoxBottom(obj, box);
//					return;
//				}
//			}
//		}
//		else
//		{
//			ResolveObjectToBoxTop(obj, box);
//		}
//	}
//}

void CollisionManager::DynamicObjectToDynamicObjectCollisions(DynamicObject* obj1, DynamicObject* obj2)
{
	if (!pnt::IsMovingTowards(obj1->GetBoundingBox()->GetPosition(), obj2->GetBoundingBox()->GetPosition(), obj1->GetVelocity(), obj2->GetVelocity()))
		return;

	float tFirst, tLast = 0;
	if (obj1->GetColVolume()->IntersectsMoving(obj2->GetColVolume(), obj1->GetVelocity(), obj2->GetVelocity(), tFirst, tLast))
	{
		obj1->ResolveCollisions(tFirst);
		obj2->ResolveCollisions(tFirst);
	}
}