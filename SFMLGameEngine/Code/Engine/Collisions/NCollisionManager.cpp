#include "NCollisionManager.h"

#include <algorithm>

std::vector<std::string> NCollisionManager::s_canCollideWithTile = {};

NCollisionManager::NCollisionManager(std::shared_ptr<IGrid> grid)
	: m_grid(std::move(grid))
{
	if (m_grid)
	{
		for (auto& tile : m_grid->GetGrid())
		{
			if (tile->GetType() != NTypes::EMPTY)
				m_tiles.push_back(tile);
		}
	}
}

void NCollisionManager::ProcessCollisions(IGameObject* gobj)
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

		ObjectToObjectCollisions(gobj, collidable.get());
	}
}

void NCollisionManager::Render(IRenderer* renderer)
{
	m_grid->Render(renderer);
}

void NCollisionManager::RemoveCollidable(IGameObject* ngo)
{
	// Use remove_if and compare underlying raw pointers
	m_collidables.erase(
		std::remove_if(m_collidables.begin(), m_collidables.end(),
			[ngo](const std::shared_ptr<IGameObject>& ptr)
			{
				return ptr.get() == ngo;
			}),
		m_collidables.end());
}

void NCollisionManager::RemoveLastAdded()
{
	if (!m_collidables.empty())
		m_collidables.pop_back();
}

IGameObject* NCollisionManager::GetLastAdded()
{
	return m_collidables.empty() ? nullptr : m_collidables.back().get();
}

ITile* NCollisionManager::GetTile(int x, int y)
{
	return m_grid->GetTile(x, y);
}

std::vector<std::shared_ptr<ITile>> NCollisionManager::GetGrid()
{
	return m_grid->GetGrid();
}

std::vector<std::shared_ptr<IGameObject>> NCollisionManager::GetCollidables()
{
	return m_collidables;
}

bool NCollisionManager::CanCollideWithTile(const std::string& texID)
{
	return std::find(s_canCollideWithTile.begin(), s_canCollideWithTile.end(), texID) != s_canCollideWithTile.end();
}

void NCollisionManager::SortCollidedTiles(std::vector<std::shared_ptr<ITile>> collidedWith)
{
	std::ranges::sort(collidedWith, [](const auto& a, const auto& b)
		{
			if (a->GetColNum() == b->GetColNum())
				return a->GetRowNum() < b->GetRowNum();

			return a->GetColNum() > b->GetColNum();
		});
}

void NCollisionManager::DynamicObjectToTileCollisions(IDynamicGameObject* obj)
{
	if (!obj)
		return;

	std::vector<std::shared_ptr<ITile>> collidedWith;
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

void NCollisionManager::ObjectToObjectCollisions(IGameObject* obj1, IGameObject* obj2)
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

void NCollisionManager::DynamicObjectToObjectCollisions(IDynamicGameObject* obj1, IGameObject* obj2)
{
	if (!obj1 || !obj2)
		return;

	float tFirst, tLast;
	if (obj2->Intersects(obj1, tFirst, tLast))
		DynamicObjectToObjectResolution(obj1, obj2, tFirst);
}

void NCollisionManager::DynamicObjectToDynamicObjectCollisions(IDynamicGameObject* obj1, IDynamicGameObject* obj2)
{
	if (!obj1 || !obj2)
		return;

	float tFirst, tLast;
	if (obj1->Intersects(obj2, tFirst, tLast))
		DynamicObjectToDynamicObjectResolution(obj1, obj2, tFirst);
}

void NCollisionManager::ObjectToObjectResolution(IGameObject* obj1, IGameObject* obj2)
{
	// Custom logic placeholder
}

void NCollisionManager::DynamicObjectToObjectResolution(IDynamicGameObject* obj1, IGameObject* obj2, float time)
{
	// Custom logic placeholder
}

void NCollisionManager::DynamicObjectToDynamicObjectResolution(IDynamicGameObject* obj1, IDynamicGameObject* obj2, float time)
{
	// Custom logic placeholder
}