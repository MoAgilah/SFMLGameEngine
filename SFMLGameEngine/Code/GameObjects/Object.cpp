#include "Object.h"

#include "../Game/GameManager.h"
#include <format>
#include <iostream>

int Object::s_objectNum = 0;

Object::Object(const std::string& texID, std::unique_ptr<BoundingVolume> volume, std::unique_ptr<Sprite> sprite)
	: m_texID(texID), m_sprite(std::move(sprite)), m_colVolume(std::move(volume))
{
	SetScale(m_scale);
	m_objectID = s_objectNum++;
	GameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

Object::Object(const std::string& texID, float radius)
	: Object(texID, std::make_unique<BoundingCircle>(radius), std::make_unique<Sprite>(texID))
{}

Object::Object(const std::string& texID, const Point& boxSize)
	: Object(texID, std::make_unique<BoundingBox>(boxSize), std::make_unique<Sprite>(texID))
{}


Object::Object(const std::string& texID, float radius, float length, float angle)
	: Object(texID, std::make_unique<BoundingCapsule>(radius, length, angle), std::make_unique<Sprite>(texID))
{}

Object::Object(const std::string& texID, const AnimationData& animData, float radius)
	: Object(texID, std::make_unique<BoundingCircle>(radius),
		std::make_unique<AnimatedSprite>(texID, animData.rows, animData.cols, GameConstants::FPS, animData.symmetrical, animData.animationSpeed))
{}

Object::Object(const std::string& texID, const AnimationData& animData, const Point& boxSize)
	: Object(texID, std::make_unique<BoundingBox>(boxSize),
		std::make_unique<AnimatedSprite>(texID, animData.rows, animData.cols, GameConstants::FPS, animData.symmetrical, animData.animationSpeed))
{}

Object::Object(const std::string& texID, const AnimationData& animData, float radius, float length, float angle)
	: Object(texID, std::make_unique<BoundingCapsule>(radius, length, angle),
		std::make_unique<AnimatedSprite>(texID, animData.rows, animData.cols, GameConstants::FPS, animData.symmetrical, animData.animationSpeed))
{}

Object::~Object()
{
	GameManager::Get()->GetCollisionMgr()->RemoveCollidable(this);
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

bool Object::Intersects(DynamicObject* obj, float& tFirst, float& tLast)
{
	return m_colVolume->IntersectsMoving(obj->GetColVolume(), Point(0, 0), obj->GetVelocity(), tFirst, tLast);
}

void Object::OnCollisionEnter(Object* obj)
{
	// actions to commit on collision enter
}

void Object::OnCollisionStay(Object* obj)
{
	// actions to commit whilst collision continues
}

void Object::OnCollisionExit(Object* obj)
{
	// actions to commit on collision exit
}

void Object::Reset()
{
	constexpr float ResetYOffset = 3.5f;
	m_active = false;
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
	m_colVolume->Update({ GetPosition().x, GetPosition().y + ResetYOffset });
}

void Object::SetDirection(bool dir)
{
	m_direction = dir;
	m_sprite->SetScale(dir ? m_scale : Point(-m_scale.x, m_scale.y));
}

void Object::SetScale(const Point& scale)
{
	m_scale = scale;
	m_sprite->SetScale(scale);
	m_colVolume->SetScale(scale);
}

DynamicObject::DynamicObject(const std::string& texID, float radius)
	: Object(texID, radius), m_physicsCtrl(std::make_unique<PhysicsController>())
{}

DynamicObject::DynamicObject(const std::string& texID, const Point& boxSize)
	: Object(texID, boxSize), m_physicsCtrl(std::make_unique<PhysicsController>())
{}

DynamicObject::DynamicObject(const std::string& texID, float radius, float length, float angle)
	: Object(texID, radius, length, angle), m_physicsCtrl(std::make_unique<PhysicsController>())
{}

DynamicObject::DynamicObject(const std::string& texID, const AnimationData& animData, float circleRadius)
	: Object(texID, animData, circleRadius), m_physicsCtrl(std::make_unique<PhysicsController>())
{}

DynamicObject::DynamicObject(const std::string& texID, const AnimationData& animData, const Point& boxSize)
	: Object(texID, animData, boxSize), m_physicsCtrl(std::make_unique<PhysicsController>())
{}

DynamicObject::DynamicObject(const std::string& texID, const AnimationData& animData, float radius, float length, float angle)
	: Object(texID, animData, radius, length, angle), m_physicsCtrl(std::make_unique<PhysicsController>())
{}

bool DynamicObject::Intersects(Object* obj)
{
	float tFirst, tLast;
	return obj->Intersects(this, tFirst, tLast);
}

bool DynamicObject::Intersects(DynamicObject* obj, float& tFirst, float& tLast)
{
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
	Move(pos.x, pos.y);
}

Direction DynamicObject::GetFacingDirection()
{
	const Point& vel = m_velocity;
	if (vel.x == 0.f && vel.y == 0.f)
		return Direction::DDIR;

	return (std::abs(vel.x) > std::abs(vel.y))
		? (vel.x < 0 ? Direction::LDIR : Direction::RDIR)
		: (vel.y < 0 ? Direction::UDIR : Direction::DDIR);
}

void DynamicObject::CheckForHorizontalBounds(float deltaTime)
{
}

DynamicObject::DynamicObject(const std::string& texID, std::unique_ptr<BoundingVolume> volume, std::unique_ptr<Sprite> sprite)
	: Object(texID,std::move(volume),std::move(sprite)), m_physicsCtrl(std::make_unique<PhysicsController>())
{}
