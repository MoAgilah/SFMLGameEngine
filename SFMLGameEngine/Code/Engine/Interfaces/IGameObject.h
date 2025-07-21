#pragma once

#include "../Interfaces/IDrawable.h"
#include "../Interfaces/IBoundingVolume.h"

struct NSpawnData
{
	bool initialDir = true;
	Point initialPos;
};

struct NAnimationData
{
	int rows;
	int cols;
	bool symmetrical;
	float animationSpeed;
};

class IDynamicGameObject;

class IGameObject
{
public:
	virtual ~IGameObject() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Render(IRenderer* renderer) = 0;
	virtual bool Intersects(IGameObject* obj) = 0;
	virtual bool Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast) = 0;
	virtual void OnCollisionEnter(IGameObject* obj) = 0;
	virtual void OnCollisionStay(IGameObject* obj) = 0;
	virtual void OnCollisionExit(IGameObject* obj) = 0;
	virtual void ResolveCollisions(float time, const Point& separationVector, float relativeHitPosition) = 0;
	virtual void Reset() = 0;

	virtual IDrawable* GetDrawable() = 0;
	virtual IBoundingVolume* GetVolume() = 0;
};

class IDynamicGameObject : public virtual IGameObject, public IMoveableWithVelocity
{
public:
	virtual ~IDynamicGameObject() = 0;
};