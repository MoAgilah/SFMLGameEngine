#include "NGameObject.h"

#include "../../Engine/Core/Constants.h"
#include "../../Engine/Core/NGameManager.h"

NGameObject::NGameObject(std::shared_ptr<IDrawable> drawable, std::shared_ptr<IBoundingVolume> volume)
	: m_drawable(std::move(drawable)), m_volume(std::move(volume))
{
	SetScale(GameConstants::Scale);
	//NGameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

NGameObject::~NGameObject()
{
	//NGameManager::Get()->GetCollisionMgr()->RemoveCollidable(this);
}

void NGameObject::Render(IRenderer* renderer)
{
	if (m_drawable)
		m_drawable->Render(renderer);
#if defined _DEBUG
	if (m_volume)
		m_volume->Render(renderer);
#endif
}

bool NGameObject::Intersects(IGameObject* obj)
{
	return m_volume->Intersects(obj->GetVolume());
}

bool NGameObject::Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast)
{
	return m_volume->IntersectsMoving(obj->GetVolume(), Point(0, 0), obj->GetVelocity(), tFirst, tLast);
}

void NGameObject::OnCollisionEnter(IGameObject* obj)
{
	// actions to commit on collision enter
}

void NGameObject::OnCollisionStay(IGameObject* obj)
{
	// actions to commit whilst collision continues
}

void NGameObject::OnCollisionExit(IGameObject* obj)
{
	// actions to commit on collision exit
}

void NGameObject::ResolveCollisions(float time, const Point& separationVector, float relativeHitPosition)
{
}

void NGameObject::Reset()
{
	constexpr float ResetYOffset = 3.5f;
	SetActive(m_active);
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
	m_volume->Update({ GetPosition().x, GetPosition().y + ResetYOffset });
}

void NGameObject::SetScale(const Point& scale)
{
	m_drawable->SetScale(scale);
	m_volume->SetScale(scale);
}

NDynamicGameObject::NDynamicGameObject(std::shared_ptr<IDrawable> drawable, std::shared_ptr<IBoundingVolume> volume)
	: NGameObject(drawable, volume)
{}

bool NDynamicGameObject::Intersects(IGameObject * obj)
{
	float tFirst, tLast;
	return obj->Intersects(this, tFirst, tLast);
}

bool NDynamicGameObject::Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast)
{
	return this->GetVolume()->IntersectsMoving(obj->GetVolume(), GetVelocity(), obj->GetVelocity(), tFirst, tLast);
}

void NDynamicGameObject::Reset()
{
	NGameObject::Reset();
	SetVelocity(Point());
	m_onGround = false;
}

void NDynamicGameObject::SetOnSlope(bool slp)
{
	m_onSlope = slp;
	if (m_onSlope)
		m_onGround = true;
}

void NDynamicGameObject::SetShouldSlideLeft(bool left)
{
	m_shouldSlideLeft = left;
	if (m_shouldSlideLeft)
		m_shouldSlideRight = false;
}

void NDynamicGameObject::SetSlideLeft(bool left)
{
	m_slideLeft = left;
	if (m_slideLeft)
		m_slideRight = false;
}

void NDynamicGameObject::SetShouldSlideRight(bool right)
{
	m_shouldSlideRight = right;
	if (m_shouldSlideRight)
		m_shouldSlideLeft = false;
}

void NDynamicGameObject::SetSlideRight(bool right)
{
	m_slideRight = right;
	if (m_slideRight)
		m_slideLeft = false;
}

Direction NDynamicGameObject::GetFacingDirection()
{
	const Point& vel = m_velocity;
	if (vel.x == 0.f && vel.y == 0.f)
		return Direction::DDIR;

	return (std::abs(vel.x) > std::abs(vel.y))
		? (vel.x < 0 ? Direction::LDIR : Direction::RDIR)
		: (vel.y < 0 ? Direction::UDIR : Direction::DDIR);
}

void NDynamicGameObject::CheckForHorizontalBounds(float deltaTime)
{
}