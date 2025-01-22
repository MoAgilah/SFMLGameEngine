#include "Box.h"
#include "../Game/GameManager.h"

Box::Box(const std::string& texID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: Object(texID, boxSize)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBoundingBox()->Update(GetPosition());
}

Box::Box(const std::string& texID, const AnimationData& animData, const sf::Vector2f& boxSize, const sf::Vector2f& initPos)
	: Object(texID, animData, boxSize)
{
	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetInitialPosition(initPos);
	SetPosition(GetInitialPosition());
	GetBoundingBox()->Update(GetPosition());
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