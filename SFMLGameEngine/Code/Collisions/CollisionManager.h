#pragma once

#include <SFML/Graphics.hpp>
#include "../Collisions/BoundingVolume.h"
#include "../Collisions/Grid.h"
#include "../Collisions/Tile.h"

class Box;
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
	void RemoveLastAdded();

	Object* GetLastAdded();

	Tile* GetTile(int x, int y);
	std::vector<std::shared_ptr<Tile>> GetGrid();
	std::vector<Object*> GetCollidables();

private:

	bool CanCollideWithTile(const std::string& texID);
	void SortCollidedTiles(std::vector<std::shared_ptr<Tile>> collidedWith);

	bool IsDynamicCollectable(const std::string& texID);
	bool IsDynamicObject(const std::string& texId);

	void DynamicObjectToTileCollisions(DynamicObject* obj);

	void ObjectToObjectCollisions(Object* obj1, Object* obj2);

	void PlayerToObjectCollisions(DynamicObject* ply, Object* obj);

	void DynamicObjectToDynamicObjectCollisions(DynamicObject* obj1, DynamicObject* obj2);

	virtual void PlayerToEnemyResolutions(DynamicObject* ply, Enemy* enmy);

	virtual void DynamicObjectToBoxResolutions(DynamicObject* ply, Box* box, bool resolveUpDir = true);

	virtual void DynamicObjectToDynamicObjectResolution(DynamicObject* obj1, DynamicObject* obj2);

	static std::vector<std::string> s_canCollideWithTile;
	static std::vector<std::string> s_dynamicCollectables;
	static std::vector<std::string> s_dynamicObject;

	Grid m_grid;
	std::vector<Object*> m_collidables;
	std::vector<std::shared_ptr<Tile>> m_tiles;
};