#include "Collectables.h"

#include "../../Engine/Core/GameManager.h"

StaticCollectable::StaticCollectable(const std::string& texID, std::unique_ptr<BoundingVolume> volume, const Point& initPos)
	: Object(texID,
		std::move(volume),
		std::make_unique<Sprite>(texID))
{
	Init(initPos);
}

StaticCollectable::StaticCollectable(const std::string& texID, const AnimationData& animData, std::unique_ptr<BoundingVolume> volume, const Point& initPos)
	: Object(texID, std::move(volume),
		std::make_unique<AnimatedSprite>(texID, animData.rows, animData.cols, GameConstants::FPS, animData.symmetrical, animData.animationSpeed))
{
	Init(initPos);
}

void StaticCollectable::Init(const Point& initPos)
{
	Init(initPos);
}

DynamicCollectable::DynamicCollectable(const std::string& texID, std::unique_ptr<BoundingVolume> volume, const Point& initPos)
	: DynamicObject(texID,
		std::move(volume),
		std::make_unique<Sprite>(texID))
{
	Init(initPos);
}

DynamicCollectable::DynamicCollectable(const std::string& texID, const AnimationData& animData, std::unique_ptr<BoundingVolume> volume, const Point& initPos)
	: DynamicObject(texID, std::move(volume),
		std::make_unique<AnimatedSprite>(texID, animData.rows, animData.cols, GameConstants::FPS, animData.symmetrical, animData.animationSpeed))
{
	Init(initPos);
}

void DynamicCollectable::Init(const Point& initPos)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBoundingBox()->Update(GetPosition());
}
