// Object.h
#pragma once

#include "../Collisions/BoundingVolume.h"
#include "../Drawables/Sprite.h"
#include "../Game/Constants.h"
#include "../Game/PhysicsController.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

struct SpawnData
{
	bool initialDir = true;
	Point initialPos;
};

struct AnimationData
{
	int rows;
	int cols;
	bool symmetrical;
	float animationSpeed;
};

class DynamicObject;

class Object
{
public:
	explicit Object(const std::string& texID, float radius);
	explicit Object(const std::string& texID, const Point& boxSize);
	explicit Object(const std::string& texID, float radius, float length, float angle);
	explicit Object(const std::string& texID, const AnimationData& animData, float circleRadius);
	explicit Object(const std::string& texID, const AnimationData& animData, const Point& boxSize);
	explicit Object(const std::string& texID, const AnimationData& animData, float radius, float length, float angle);

	virtual ~Object();

	virtual void Update(float deltaTime) = 0;
	virtual void Render(sf::RenderWindow& window);
	virtual bool IsDynamicObject() { return false; }
	virtual bool Intersects(Object* obj);
	virtual bool Intersects(DynamicObject* obj, float& tFirst, float& tLast);
	virtual void OnCollisionEnter(Object* obj);
	virtual void OnCollisionStay(Object* obj);
	virtual void OnCollisionExit(Object* obj);
	virtual void Reset();

	int GetObjectNum() const { return m_objectID; }
	AnimatedSprite* GetAnimSpr() { return static_cast<AnimatedSprite*>(GetSprite()); }
	Sprite* GetSprite() { return m_sprite.get(); }
	BoundingBox* GetBoundingBox() { return static_cast<BoundingBox*>(m_colVolume.get()); }
	BoundingCircle* GetBoundingCircle() { return static_cast<BoundingCircle*>(m_colVolume.get()); }
	BoundingCapsule* GetBoundingCapsule() { return static_cast<BoundingCapsule*>(m_colVolume.get()); }
	BoundingVolume* GetColVolume() { return m_colVolume.get(); }
	virtual void ResolveCollisions(float time, const Point& separationVector, float relativeHitPosition) {}

	const std::string& GetID() const { return m_texID; }
	void SetID(const std::string& texID) { m_texID = texID; }
	virtual bool GetActive() const { return m_active; }
	void SetActive(bool act) { m_active = act; }
	bool GetDirection() const { return m_direction; }
	void SetDirection(bool dir);
	Point GetPosition() const { return m_sprite->GetPosition(); };
	void SetPosition(const Point& pos) { m_sprite->SetPosition(pos); }
	void SetPosition(float x, float y) { m_sprite->SetPosition({ x, y }); }
	Point GetOrigin() const { return m_sprite->GetOrigin(); }
	bool GetInitialDirection() const { return m_spawnData.initialDir; }
	void SetInitialDirection(bool dir) { m_spawnData.initialDir = dir; }
	Point GetInitialPosition() const { return m_spawnData.initialPos; }
	void SetInitialPosition(const Point& pos) { m_spawnData.initialPos = pos; }
	virtual float GetRestitution() { return m_restitution; }
	void SetScale(const Point& scale);
	Point GetScale() const { return m_scale; }

protected:
	Object(const std::string& texID, std::unique_ptr<BoundingVolume> volume, std::unique_ptr<Sprite> sprite);

private:

	int m_objectID = 0;
	static int s_objectNum;
	std::string m_texID;
	bool m_active = false;
	bool m_direction = true;
	float m_restitution = 1;
	Point m_scale = GameConstants::Scale;
	SpawnData m_spawnData;
	std::unique_ptr<Sprite> m_sprite;
	std::unique_ptr<BoundingVolume> m_colVolume;
};

class DynamicObject : public Object
{
public:
	explicit DynamicObject(const std::string& texID, float radius);
	explicit DynamicObject(const std::string& texID, const Point& boxSize);
	explicit DynamicObject(const std::string& texID, float radius, float length, float angle);
	explicit DynamicObject(const std::string& texID, const AnimationData& animData, float circleRadius);
	explicit DynamicObject(const std::string& texID, const AnimationData& animData, const Point& boxSize);
	explicit DynamicObject(const std::string& texID, const AnimationData& animData, float radius, float length, float angle);
	~DynamicObject() override = default;

	bool IsDynamicObject() final { return true; }
	bool Intersects(Object* obj) override;
	bool Intersects(DynamicObject* obj, float& tFirst, float& tLast) override;
	void Reset() override;

	void SetPrevPosition(Point pos) { m_previousPos = pos; }
	void SetPrevPosition(float x, float y) { m_previousPos = Point(x, y); }
	Point GetPrevPosition() const { return m_previousPos; }
	Point GetVelocity() const { return m_velocity; }
	void SetVelocity(const Point& vel) { m_velocity = vel; }
	void SetVelocity(float x, float y) { m_velocity = Point(x, y); }
	float GetXVelocity() const { return m_velocity.x; }
	void SetXVelocity(float x) { m_velocity.x = x; }
	void IncrementXVelocity(float x);
	void DecrementXVelocity(float x);
	float GetYVelocity() const { return m_velocity.y; }
	void SetYVelocity(float y) { m_velocity.y = y; }
	void IncrementYVelocity(float y);
	void DecrementYVelocity(float y);
	bool GetOnGround() const { return m_onGround; }
	void SetOnGround(bool grnd) { m_onGround = grnd; }
	bool GetOnSlope() const { return m_onSlope; }
	void SetOnSlope(bool slp);
	bool GetShouldSlideLeft() const { return m_shouldSlideLeft; }
	void SetShouldSlideLeft(bool left);
	bool GetSlideLeft() const { return m_slideLeft; }
	void SetSlideLeft(bool left);
	bool GetShouldSlideRight() const { return m_shouldSlideRight; }
	void SetShouldSlideRight(bool right);
	bool GetSlideRight() const { return m_slideRight; }
	void SetSlideRight(bool right);
	void Move(float x, float y);
	void Move(const Point& pos);
	Direction GetFacingDirection();
	void CheckForHorizontalBounds(float deltaTime);
	PhysicsController* GetPhysicsController() { return m_physicsCtrl.get(); }

protected:
	DynamicObject(const std::string& texID, std::unique_ptr<BoundingVolume> volume, std::unique_ptr<Sprite> sprite);

private:

	bool m_onSlope = false;
	bool m_shouldSlideLeft = false;
	bool m_slideLeft = false;
	bool m_shouldSlideRight = false;
	bool m_slideRight = false;
	bool m_onGround = false;
	Point m_velocity;
	Point m_previousPos;
	std::unique_ptr<PhysicsController> m_physicsCtrl;
};
