#include "Collectables.h"

#include "../Game/GameManager.h"

StaticCollectable::StaticCollectable(const std::string& texId, const Point& boxSize, const Point& initPos)
	: Object(texId, boxSize)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBoundingBox()->Update(GetPosition());
}

StaticCollectable::StaticCollectable(const std::string& texID, const AnimationData& animData, const Point& boxSize, const Point& initPos)
	: Object(texID, animData, boxSize)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBoundingBox()->Update(GetPosition());
}

DynamicCollectable::DynamicCollectable(const std::string& texId, const Point& boxSize, const Point& initPos)
	: DynamicObject(texId, boxSize)
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBoundingBox()->Update(GetPosition());
}

DynamicCollectable::DynamicCollectable(const std::string& texID, const AnimationData& animData, const Point& boxSize, const Point& initPos)
	: DynamicObject(texID, animData, boxSize)
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBoundingBox()->Update(GetPosition());
}