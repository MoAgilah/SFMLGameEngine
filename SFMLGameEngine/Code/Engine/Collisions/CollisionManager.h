#pragma once

#include "BoundingVolume.h"
#include "Grid.h"
#include "Tile.h"
#include <SFML/Graphics.hpp>

class Enemy;
class Object;
class DynamicObject;

class CollisionManager
{
public:
	CollisionManager(int rows = 0, int columns = 0, const std::string& tileFilePaths = GameConstants::TileFilePaths);
	~CollisionManager() = default;

	void ProcessCollisions(Object* object);
	void Render(sf::RenderWindow& window);

	void AddCollidable(Object* ngo);
	void RemoveCollidable(Object* ngo);
	void RemoveLastAdded();

	Object* GetLastAdded();

	Tile* GetTile(int x, int y);
	std::vector<std::shared_ptr<Tile>> GetGrid();
	std::vector<Object*> GetCollidables();

	static std::vector<std::string> s_canCollideWithTile;
	inline static const float EPSILON = std::numeric_limits<float>::epsilon() * 100;

private:

	bool CanCollideWithTile(const std::string& texID);
	void SortCollidedTiles(std::vector<std::shared_ptr<Tile>> collidedWith);

	void DynamicObjectToTileCollisions(DynamicObject* obj);

	void ObjectToObjectCollisions(Object* obj1, Object* obj2);

	void DynamicObjectToObjectCollisions(DynamicObject* obj1, Object* obj2);

	void DynamicObjectToDynamicObjectCollisions(DynamicObject* obj1, DynamicObject* obj2);

	virtual void ObjectToObjectResolution(Object* obj1, Object* obj2);

	virtual void DynamicObjectToObjectResolution(DynamicObject* obj1, Object* obj2, float time);

	virtual void DynamicObjectToDynamicObjectResolution(DynamicObject* obj1, DynamicObject* obj2, float time);

	Grid m_grid;
	std::vector<Object*> m_collidables;
	std::vector<std::shared_ptr<Tile>> m_tiles;
};