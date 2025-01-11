#include "Collectables.h"
#include "../Game/GameManager.h"

StaticCollectable::StaticCollectable(const std::string& texId, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: Object(texId, boxSize)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());
}

StaticCollectable::StaticCollectable(AnimatedSprite* sprite, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: Object(sprite, boxSize)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());
}

DynamicCollectable::DynamicCollectable(const std::string& texId, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: DynamicObject(texId, boxSize)
{
	SetInitialDirection(false);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());
}