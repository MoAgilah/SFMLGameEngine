#include "CollisionManager.h"

#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../Utilities/Utilities.h"
#include <format>
#include <algorithm>

using namespace obj;

std::vector<std::string> CollisionManager::s_canCollideWithTile = {};

CollisionManager::CollisionManager(int rows, int columns, const std::string& tileFilePaths)
	: m_grid(rows, columns, tileFilePaths)
{
	for (auto& tile : m_grid.GetGrid())
	{
		if (tile->GetType() != Types::EMPTY)
			m_tiles.push_back(tile);
	}
}

void CollisionManager::AddCollidable(Object* go)
{
	m_collidables.push_back(go);
}

void CollisionManager::RemoveLastAdded()
{
	if (!m_collidables.empty())
		m_collidables.pop_back();
}

Object* CollisionManager::GetLastAdded()
{
	return m_collidables.empty() ? nullptr : m_collidables.back();
}

void CollisionManager::Render(sf::RenderWindow& window)
{
	m_grid.Render(window);
}

void CollisionManager::ProcessCollisions(Object* gobj)
{
	if (!gobj)
		return;

	if (CanCollideWithTile(gobj->GetID()))
	{
		if (auto* dynObj = dynamic_cast<DynamicObject*>(gobj))
			DynamicObjectToTileCollisions(dynObj);
	}

	for (Object* collidable : m_collidables)
	{
		if (!collidable || !collidable->GetActive())
			continue;

		if (gobj->GetObjectNum() == collidable->GetObjectNum())
			continue;

		ObjectToObjectCollisions(gobj, collidable);
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

void CollisionManager::DynamicObjectToTileCollisions(DynamicObject* obj)
{
	if (!obj)
		return;

	std::vector<std::shared_ptr<Tile>> collidedWith;
	for (const auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (tile->GetBoundingBox()->Intersects(obj->GetColVolume()))
			collidedWith.push_back(tile);
	}

	if (!collidedWith.empty())
	{
		if (obj->GetDirection())
			SortCollidedTiles(collidedWith);

		for (const auto& tile : collidedWith)
		{
			if (tile->Intersects(obj))
				tile->ResolveCollision(obj);
		}
	}
}

void CollisionManager::ObjectToObjectCollisions(Object* obj1, Object* obj2)
{
	if (!obj1 || !obj2)
		return;

	const bool isDyn1 = obj1->IsDynamicObject();
	const bool isDyn2 = obj2->IsDynamicObject();

	if (isDyn1 && isDyn2)
	{
		DynamicObjectToDynamicObjectCollisions(
			dynamic_cast<DynamicObject*>(obj1),
			dynamic_cast<DynamicObject*>(obj2));
	}
	else if (isDyn1)
	{
		DynamicObjectToObjectCollisions(dynamic_cast<DynamicObject*>(obj1), obj2);
	}
	else if (isDyn2)
	{
		DynamicObjectToObjectCollisions(dynamic_cast<DynamicObject*>(obj2), obj1);
	}
	else
	{
		if (obj1->Intersects(obj2))
			ObjectToObjectResolution(obj1, obj2);
	}
}

void CollisionManager::DynamicObjectToObjectCollisions(DynamicObject* obj1, Object* obj2)
{
	if (!obj1 || !obj2)
		return;

	float tFirst, tLast;
	if (obj2->Intersects(obj1, tFirst, tLast))
		DynamicObjectToObjectResolution(obj1, obj2, tFirst);
}

void CollisionManager::DynamicObjectToDynamicObjectCollisions(DynamicObject* obj1, DynamicObject* obj2)
{
	if (!obj1 || !obj2)
		return;

	float tFirst, tLast;
	if (obj1->Intersects(obj2, tFirst, tLast))
		DynamicObjectToDynamicObjectResolution(obj1, obj2, tFirst);
}

void CollisionManager::ObjectToObjectResolution(Object* obj1, Object* obj2)
{
	// Custom logic placeholder
}

void CollisionManager::DynamicObjectToObjectResolution(DynamicObject* obj1, Object* obj2, float time)
{
	// Custom logic placeholder
}

void CollisionManager::DynamicObjectToDynamicObjectResolution(DynamicObject* obj1, DynamicObject* obj2, float time)
{
	// Custom logic placeholder
}