#pragma once

#include "../../Engine/Interfaces/IGameObject.h"

class NGameObject : public virtual IGameObject
{
public:
	NGameObject(std::shared_ptr<IDrawable> drawable, std::shared_ptr<IBoundingVolume> volume);
	virtual ~NGameObject();

	virtual void Update(float deltaTime) = 0;

	void Render(IRenderer* renderer) override;

	bool Intersects(IGameObject* obj) override;

	bool Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast) override;

	void OnCollisionEnter(IGameObject* obj) override;
	void OnCollisionStay(IGameObject* obj) override;
	void OnCollisionExit(IGameObject* obj) override;

	void ResolveCollisions(float time, const Point& separationVector, float relativeHitPosition) override;

	void Reset() override;

	IDrawable* GetDrawable() override { return m_drawable.get(); }
	IBoundingVolume* GetVolume() override { return m_volume.get(); }

	virtual float GetRestitution() { return m_restitution; }
	virtual void SetRestitution(float restitution) { m_restitution = restitution; }

	Point GetPosition() const { return m_drawable->GetPosition(); }
	void SetPosition(const Point& pos) { m_drawable->SetPosition(pos); }
	void SetPosition(float x, float y) { m_drawable->SetPosition({ x, y }); }
	Point GetOrigin() const { return m_drawable->GetOrigin(); }
	void SetScale(const Point& scale);
	Point GetScale() const { return m_drawable->GetScale(); }

	bool GetInitialDirection() const { return m_spawnData.initialDir; }
	void SetInitialDirection(bool dir) { m_spawnData.initialDir = dir; }
	Point GetInitialPosition() const { return m_spawnData.initialPos; }
	void SetInitialPosition(const Point& pos) { m_spawnData.initialPos = pos; }

protected:


	float m_restitution = 1;
	NSpawnData m_spawnData;
	std::shared_ptr<IDrawable> m_drawable;
	std::shared_ptr<IBoundingVolume> m_volume;
};

class NDynamicGameObject : public virtual IDynamicGameObject, public NGameObject
{
public:
	NDynamicGameObject(std::shared_ptr<IDrawable> drawable, std::shared_ptr<IBoundingVolume> volume);

	bool Intersects(IGameObject* obj) override;

	bool Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast) override;

	void Reset() override;

	IDrawable* GetDrawable() override { return NGameObject::GetDrawable(); }
	IBoundingVolume* GetVolume() override { return NGameObject::GetVolume(); }

	void SetPrevPosition(Point pos) { m_previousPos = pos; }
	void SetPrevPosition(float x, float y) { m_previousPos = Point(x, y); }
	Point GetPrevPosition() const { return m_previousPos; }

	float GetXVelocity() const { return m_velocity.x; }
	void SetXVelocity(float x) { m_velocity.x = x; }
	void IncrementXVelocity(float x) { m_velocity.x += x; }
	void DecrementXVelocity(float x) { m_velocity.x -= x; }

	float GetYVelocity() const { return m_velocity.y; }
	void SetYVelocity(float y) { m_velocity.y = y; }
	void IncrementYVelocity(float y) { m_velocity.y += y; }
	void DecrementYVelocity(float y) { m_velocity.y -= y; }

protected:

	Point m_previousPos;
};