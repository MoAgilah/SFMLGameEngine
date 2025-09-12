#include "CollisionManager.h"

#include "../Interfaces/IGrid.h"
#include "../Interfaces/ITile.h"
#include "../Interfaces/IGameObject.h"
#include "../Interfaces/IRenderer.h"

#include <algorithm>
#include <ranges>
#include <utility>

std::vector<std::string> CollisionManager::s_canCollideWithTile = {};

CollisionManager::CollisionManager(std::shared_ptr<IGrid> grid)
	: m_grid(std::move(grid))
{
	if (m_grid)
	{
		for (auto& tile : m_grid->GetGrid())
		{
			if (tile->GetType() != Types::EMPTY)
				m_tiles.push_back(tile.get());
		}
	}
}

void CollisionManager::ProcessCollisions(IGameObject* gobj)
{
	if (!gobj)
		return;

	if (CanCollideWithTile(gobj->GetID()))
	{
		if (auto* dynObj = dynamic_cast<IDynamicGameObject*>(gobj))
			DynamicObjectToTileCollisions(dynObj);
	}

	for (auto collidable : m_collidables)
	{
		if (!collidable || !collidable->GetActive())
			continue;

		if (gobj->GetObjectNum() == collidable->GetObjectNum())
			continue;

		ObjectToObjectCollisions(gobj, collidable);
	}
}

void CollisionManager::Render(IRenderer* renderer)
{
	if (m_grid)
		m_grid->Render(renderer);
}

void CollisionManager::RemoveCollidable(IGameObject* ngo)
{
	m_collidables.erase(
		std::remove_if(m_collidables.begin(), m_collidables.end(),
			[ngo](IGameObject* ptr) {
				return ptr == ngo;
			}),
		m_collidables.end());
}

void CollisionManager::RemoveLastAdded()
{
	if (!m_collidables.empty())
		m_collidables.pop_back();
}

IGameObject* CollisionManager::GetLastAdded()
{
	return m_collidables.empty() ? nullptr : m_collidables.back();
}

ITile* CollisionManager::GetTile(int x, int y)
{
	return m_grid->GetTile(x, y);
}

std::vector<ITile*> CollisionManager::GetGrid()
{
	if (m_grid)
	{
		auto stripShared = [](const std::vector<std::shared_ptr<ITile>>& src) {
			std::vector<ITile*> result;
			result.reserve(src.size());
			std::transform(src.begin(), src.end(), std::back_inserter(result),
				[](const std::shared_ptr<ITile>& sp) { return sp.get(); });
			return result;
			};

		return stripShared(m_grid->GetGrid());
	}

	return {};
}

std::vector<IGameObject*> CollisionManager::GetCollidables()
{
	return m_collidables;
}

bool CollisionManager::CanCollideWithTile(const std::string& texID)
{
	return std::find(s_canCollideWithTile.begin(), s_canCollideWithTile.end(), texID) != s_canCollideWithTile.end();
}

void CollisionManager::SortCollidedTiles(std::vector<ITile*> collidedWith)
{
	std::ranges::sort(collidedWith, [](const auto& a, const auto& b)
		{
			if (a->GetColNum() == b->GetColNum())
				return a->GetRowNum() < b->GetRowNum();

			return a->GetColNum() > b->GetColNum();
		});
}

void CollisionManager::DynamicObjectToTileCollisions(IDynamicGameObject* obj)
{
	if (!obj)
		return;

	std::vector<ITile*> collidedWith;
	for (const auto& tile : m_tiles)
	{
		if (!tile->GetActive())
			continue;

		if (tile->GetBoundingBox()->Intersects(obj->GetVolume()))
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

void CollisionManager::ObjectToObjectCollisions(IGameObject* obj1, IGameObject* obj2)
{
	if (!obj1 || !obj2)
		return;

	const bool isDyn1 = obj1->IsDynamicObject();
	const bool isDyn2 = obj2->IsDynamicObject();

	if (isDyn1 && isDyn2)
	{
		DynamicObjectToDynamicObjectCollisions(
			dynamic_cast<IDynamicGameObject*>(obj1),
			dynamic_cast<IDynamicGameObject*>(obj2));
	}
	else if (isDyn1)
	{
		DynamicObjectToObjectCollisions(dynamic_cast<IDynamicGameObject*>(obj1), obj2);
	}
	else if (isDyn2)
	{
		DynamicObjectToObjectCollisions(dynamic_cast<IDynamicGameObject*>(obj2), obj1);
	}
	else
	{
		if (obj1->Intersects(obj2))
			ObjectToObjectResolution(obj1, obj2);
	}
}

void CollisionManager::DynamicObjectToObjectCollisions(IDynamicGameObject* obj1, IGameObject* obj2)
{
	if (!obj1 || !obj2)
		return;

	float tFirst, tLast;
	if (obj2->Intersects(obj1, tFirst, tLast))
		DynamicObjectToObjectResolution(obj1, obj2, tFirst);
}

void CollisionManager::DynamicObjectToDynamicObjectCollisions(IDynamicGameObject* obj1, IDynamicGameObject* obj2)
{
	if (!obj1 || !obj2)
		return;

	float tFirst, tLast;
	if (obj1->Intersects(obj2, tFirst, tLast))
		DynamicObjectToDynamicObjectResolution(obj1, obj2, tFirst);
}

void CollisionManager::ObjectToObjectResolution(IGameObject* obj1, IGameObject* obj2)
{
	// Custom logic placeholder
}

void CollisionManager::DynamicObjectToObjectResolution(IDynamicGameObject* obj1, IGameObject* obj2, float time)
{
	// Custom logic placeholder
}

void CollisionManager::DynamicObjectToDynamicObjectResolution(IDynamicGameObject* obj1, IDynamicGameObject* obj2, float time)
{
	// Custom logic placeholder
}