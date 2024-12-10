#include "Box.h"
#include "../Game/GameManager.h"

Box::Box(AnimatedSprite* sprite, const sf::Vector2f& initPos)
	: Object(sprite, sf::Vector2f(16,16))
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetAABB()->Update(GetPosition());
}

void Box::Reset()
{
	m_canHit = true;
	m_justHit = false;
	Object::Reset();
}

void Box::WasJustHit()
{
	if (m_canHit)
		m_justHit = true;
}