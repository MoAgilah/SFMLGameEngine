#include "NGameObject.h"

#include "../../Engine/Core/Constants.h"
#include "../../Engine/Core/NGameManager.h"

NGameObject::NGameObject(std::shared_ptr<IDrawable> drawable, std::shared_ptr<IBoundingVolume> volume)
	: m_drawable(std::move(drawable)), m_volume(std::move(volume))
{
	m_objectID = s_objectNum++;
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
	this->SetVelocity(Point());
	m_onGround = false;
}