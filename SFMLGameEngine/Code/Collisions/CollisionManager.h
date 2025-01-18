#pragma once

#include <SFML/Graphics.hpp>
#include "../Collisions/BoundingVolume.h"
#include "../Collisions/Grid.h"
#include "../Collisions/Tile.h"

enum Direction
{
	LDIR, RDIR, UDIR, DDIR
};

class Enemy;
class Object;
class DynamicObject;

class CollisionManager
{
public:
	CollisionManager();
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

	void DynamicObjectToTileCollisions(DynamicObject* obj);
	void DynamicObjectToTileResolution(DynamicObject* obj, Tile* tile);

	void PlayerToObjectCollisions(DynamicObject* ply, Object* obj);
	void ObjectToObjectCollisions(Object* obj1, Object* obj2);

	void PlayerToEnemyResolutions(DynamicObject* ply, Enemy* enmy);

	void DynamicObjectToBoxResolutions(Direction dirOfTravel, const Point& prevOverlap, DynamicObject* obj, AABB* box, bool resolveUpDir = true);

	void DynamicObjectToDynamicObjectCollisions(DynamicObject* obj1, DynamicObject* obj2);
	void DynamicObjectToDynamicObjectResolution(DynamicObject* obj, float tFirst);

	void ResolveObjectToBoxTop(DynamicObject* obj, AABB* box);
	void ResolveObjectToBoxBottom(DynamicObject* obj, AABB* box);
	void ResolveObjectToBoxHorizontally(DynamicObject* obj, AABB* box);

	bool ResolveObjectToSlopeTop(DynamicObject* obj, Tile* box);
	bool ResolveObjectToSlopeIncline(DynamicObject* obj, Tile* box, int start, int end);
	bool ResolveObjectToSlopeDecline(DynamicObject* obj, Tile* box, int start, int end);

	void DynamicObjectToEdgeBounds(DynamicObject* obj, Tile* tile);

	Direction GetFacingDirection(DynamicObject* obj);

	Grid m_grid;
	std::vector<Object*> m_collidables;
	std::vector<std::shared_ptr<Tile>> m_tiles;
};