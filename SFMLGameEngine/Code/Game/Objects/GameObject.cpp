#include "GameObject.h"

#include "../../Engine/Core/Constants.h"
#include "../../Engine/Core/GameManager.h"

GameObject::GameObject(std::shared_ptr<IDrawable> drawable, std::shared_ptr<IBoundingVolume> volume)
	: m_drawable(std::move(drawable)), m_volume(std::move(volume))
{
	m_objectID = s_objectNum++;
	SetScale(GameConstants::Scale);
	GameManager::Get()->GetCollisionMgr()->AddCollidable(this);
}

GameObject::~GameObject()
{
	GameManager::Get()->GetCollisionMgr()->RemoveCollidable(this);
}

void GameObject::Render(IRenderer* renderer)
{
	if (m_drawable)
		m_drawable->Render(renderer);
#if defined _DEBUG
	if (m_volume)
		m_volume->Render(renderer);
#endif
}

bool GameObject::Intersects(IGameObject* obj)
{
	return m_volume->Intersects(obj->GetVolume());
}

bool GameObject::Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast)
{
	return m_volume->IntersectsMoving(obj->GetVolume(), Point(0, 0), obj->GetVelocity(), tFirst, tLast);
}

void GameObject::OnCollisionEnter(IGameObject* obj)
{
	// actions to commit on collision enter
}

void GameObject::OnCollisionStay(IGameObject* obj)
{
	// actions to commit whilst collision continues
}

void GameObject::OnCollisionExit(IGameObject* obj)
{
	// actions to commit on collision exit
}

void GameObject::ResolveCollisions(float time, const Point& separationVector, float relativeHitPosition)
{
}

void GameObject::Reset()
{
	constexpr float ResetYOffset = 3.5f;
	SetActive(m_active);
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());
	m_volume->Update({ GetPosition().x, GetPosition().y + ResetYOffset });
}

void GameObject::SetScale(const Point& scale)
{
	m_drawable->SetScale(scale);
	m_volume->SetScale(scale);
}

DynamicGameObject::DynamicGameObject(std::shared_ptr<IDrawable> drawable, std::shared_ptr<IBoundingVolume> volume)
	: GameObject(drawable, volume)
{}

bool DynamicGameObject::Intersects(IGameObject * obj)
{
	float tFirst, tLast;
	return obj->Intersects(this, tFirst, tLast);
}

bool DynamicGameObject::Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast)
{
	return this->GetVolume()->IntersectsMoving(obj->GetVolume(), GetVelocity(), obj->GetVelocity(), tFirst, tLast);
}

void DynamicGameObject::Reset()
{
	GameObject::Reset();
	this->SetVelocity(Point());
	m_onGround = false;
}