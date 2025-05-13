#include "Object.h"

#include "../Game/GameManager.h"
#include <format>
#include <iostream>

int Object::s_objectNum = 0;

Object::Object(const std::string& texID, float radius)
	: m_texID(texID)
{
	m_sprite = std::make_unique<Sprite>(m_texID);
	m_colVolume = std::make_unique<BoundingCircle>(radius);
	m_objectID = s_objectNum++;
	GameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

Object::Object(const std::string& texID, const Point& boxSize)
	: m_texID(texID)
{
	m_sprite = std::make_unique<Sprite>(m_texID);
	m_colVolume = std::make_unique<BoundingBox>(boxSize);
	m_objectID = s_objectNum++;
	GameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

Object::Object(const std::string& texID, float radius, float length, float angle)
	: m_texID(texID)
{
	m_sprite = std::make_unique<Sprite>(m_texID);
	m_colVolume = std::make_unique<BoundingCapsule>(radius, length, angle);
	m_objectID = s_objectNum++;
	GameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

Object::Object(const std::string& texID, const AnimationData& animData, float radius)
	: m_texID(texID)
{
	m_sprite = std::make_unique<AnimatedSprite>(m_texID, animData.rows, animData.cols, GameConstants::FPS, animData.symmetrical, animData.animationSpeed);
	m_colVolume = std::make_unique<BoundingCircle>(radius);
	m_objectID = s_objectNum++;
	GameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

Object::Object(const std::string& texID, const AnimationData& animData, const Point& boxSize)
	: m_texID(texID)
{
	m_sprite = std::make_unique<AnimatedSprite>(m_texID, animData.rows, animData.cols, GameConstants::FPS, animData.symmetrical, animData.animationSpeed);
	m_colVolume = std::make_unique<BoundingBox>(boxSize);
	m_objectID = s_objectNum++;
	GameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

Object::Object(const std::string& texID, const AnimationData& animData, float radius, float length, float angle)
	: m_texID(texID)
{
	m_sprite = std::make_unique<AnimatedSprite>(m_texID, animData.rows, animData.cols, GameConstants::FPS, animData.symmetrical, animData.animationSpeed);
	m_colVolume = std::make_unique<BoundingCapsule>(radius, length, angle);
	m_objectID = s_objectNum++;
	GameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

void Object::Render(sf::RenderWindow& window)
{
	m_sprite->Render(window);
#if defined _DEBUG
	m_colVolume->Render(window);
#endif
}

bool Object::Intersects(Object* obj)
{
	return m_colVolume->Intersects(obj->GetColVolume());
}

bool Object::Intersects(DynamicObject* obj)
{
	return m_colVolume->Intersects(obj->GetColVolume());
}

void Object::Reset()
{
	m_active = false;
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
	m_colVolume->Update({ GetPosition().x, GetPosition().y + 3.5f });
}

void Object::SetDirection(bool dir)
{
	m_direction = dir;
	if (m_direction)
	{
		// flip X
		m_sprite->SetScale(GameConstants::Scale);
	}
	else
	{
		//unflip x
		m_sprite->SetScale({ -GameConstants::Scale.x, GameConstants::Scale.y });
	}
}

DynamicObject::DynamicObject(const std::string& texID, float radius)
	: Object(texID, radius)
{
	m_physicsCtrl = std::make_unique<PhysicsController>();
}

DynamicObject::DynamicObject(const std::string& texID, const Point& boxSize)
	: Object(texID, boxSize)
{
	m_physicsCtrl = std::make_unique<PhysicsController>();
}

DynamicObject::DynamicObject(const std::string& texID, float radius, float length, float angle)
	: Object(texID, radius, length, angle)
{
	m_physicsCtrl = std::make_unique<PhysicsController>();
}

DynamicObject::DynamicObject(const std::string& texID, const AnimationData& animData, float circleRadius)
	: Object(texID, animData, circleRadius)
{
	m_physicsCtrl = std::make_unique<PhysicsController>();
}

DynamicObject::DynamicObject(const std::string& texID, const AnimationData& animData, const Point& boxSize)
	: Object(texID, animData, boxSize)
{
	m_physicsCtrl = std::make_unique<PhysicsController>();
}

DynamicObject::DynamicObject(const std::string& texID, const AnimationData& animData, float radius, float length, float angle)
	: Object(texID, animData, radius, length, angle)
{
	m_physicsCtrl = std::make_unique<PhysicsController>();
}

bool DynamicObject::Intersects(DynamicObject* obj)
{
	float tFirst, tLast = 0;
	return GetColVolume()->IntersectsMoving(obj->GetColVolume(), GetVelocity(), obj->GetVelocity(), tFirst, tLast);
}

void DynamicObject::Reset()
{
	Object::Reset();
	SetVelocity(Point());
	m_onGround = false;
}

void DynamicObject::IncrementXVelocity(float x)
{
	m_velocity.x += x;
	auto physicsCtrl = GetPhysicsController();
	if (m_velocity.x > physicsCtrl->GetMaxXVelocity())
		m_velocity.x = physicsCtrl->GetMaxXVelocity();
}

void DynamicObject::DecrementXVelocity(float x)
{
	m_velocity.x -= x;
	auto physicsCtrl = GetPhysicsController();
	if (m_velocity.x < -physicsCtrl->GetMaxXVelocity())
		m_velocity.x = -physicsCtrl->GetMaxXVelocity();
}

void DynamicObject::IncrementYVelocity(float y)
{
	m_velocity.y += y;
	auto physicsCtrl = GetPhysicsController();
	if (m_velocity.y > physicsCtrl->GetMaxYVelocity())
		m_velocity.y = physicsCtrl->GetMaxYVelocity();
}

void DynamicObject::DecrementYVelocity(float y)
{
	m_velocity.y -= y;
	auto physicsCtrl = GetPhysicsController();
	if (m_velocity.y < -physicsCtrl->GetMaxYVelocity())
		m_velocity.y = -physicsCtrl->GetMaxYVelocity();
}

void DynamicObject::SetOnSlope(bool slp)
{
	m_onSlope = slp;
	if (m_onSlope)
		m_onGround = true;
}

void DynamicObject::SetShouldSlideLeft(bool left)
{
	m_shouldSlideLeft = left;
	if (m_shouldSlideLeft)
		m_shouldSlideRight = false;
}

void DynamicObject::SetSlideLeft(bool left)
{
	m_slideLeft = left;
	if (m_slideLeft)
		m_slideRight = false;
}

void DynamicObject::SetShouldSlideRight(bool right)
{
	m_shouldSlideRight = right;
	if (m_shouldSlideRight)
		m_shouldSlideLeft = false;
}

void DynamicObject::SetSlideRight(bool right)
{
	m_slideRight = right;
	if (m_slideRight)
		m_slideLeft = false;
}

void DynamicObject::Move(float x, float y)
{
	GetSprite()->Move(x, y);
	GetColVolume()->Move(x, y);
}

void DynamicObject::Move(const Point& pos)
{
	GetSprite()->Move(pos.x, pos.y);
	GetColVolume()->Move(pos);
}

Direction DynamicObject::GetFacingDirection()
{
	const Point& currentVel = m_velocity;

	Direction currentDir = Direction::DDIR;

	if (currentVel.x != 0.f || currentVel.y != 0.f)
	{
		float vxa = std::abs(currentVel.x);
		float vya = std::abs(currentVel.y);

		if (vxa > vya)
		{
			currentDir = (currentVel.x < 0) ?
				Direction::LDIR : Direction::RDIR;
		}
		else
		{
			currentDir = (currentVel.y < 0) ?
				Direction::UDIR : Direction::DDIR;
		}
	}

	return currentDir;
}

void DynamicObject::CheckForHorizontalBounds(float deltaTime)
{
}