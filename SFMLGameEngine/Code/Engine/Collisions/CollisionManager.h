#pragma once

#include <memory>
#include <vector>
#include <string>
#include <limits>

class IGrid;
class ITile;
class IRenderer;
class IGameObject;
class IDynamicGameObject;

class CollisionManager
{
public:
	CollisionManager(std::shared_ptr<IGrid> grid);
	~CollisionManager() = default;

	void ProcessCollisions(IGameObject* object);
	void Render(IRenderer* renderer);

	void AddCollidable(IGameObject* ngo) { m_collidables.emplace_back(ngo); }
	void RemoveCollidable(IGameObject* ngo);

	void RemoveLastAdded();
	IGameObject* GetLastAdded();

	ITile* GetTile(int x, int y);
	std::vector<std::shared_ptr<ITile>> GetGrid();
	std::vector<std::shared_ptr<IGameObject>> GetCollidables();

	static std::vector<std::string> s_canCollideWithTile;
	inline static const float EPSILON = std::numeric_limits<float>::epsilon() * 100;
	inline static const float BUFFER = 0.01f;

protected:

	bool CanCollideWithTile(const std::string& texID);
	void SortCollidedTiles(std::vector<std::shared_ptr<ITile>> collidedWith);

	void DynamicObjectToTileCollisions(IDynamicGameObject* obj);

	void ObjectToObjectCollisions(IGameObject* obj1, IGameObject* obj2);

	void DynamicObjectToObjectCollisions(IDynamicGameObject* obj1, IGameObject* obj2);

	void DynamicObjectToDynamicObjectCollisions(IDynamicGameObject* obj1, IDynamicGameObject* obj2);

	virtual void ObjectToObjectResolution(IGameObject* obj1, IGameObject* obj2);

	virtual void DynamicObjectToObjectResolution(IDynamicGameObject* obj1, IGameObject* obj2, float time);

	virtual void DynamicObjectToDynamicObjectResolution(IDynamicGameObject* obj1, IDynamicGameObject* obj2, float time);

	std::shared_ptr<IGrid> m_grid;
	std::vector<std::shared_ptr<ITile>> m_tiles;
	std::vector< std::shared_ptr<IGameObject>> m_collidables;
};