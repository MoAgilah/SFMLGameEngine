#pragma once

#include "Object.h"
#include "../Game/Timer.h"

class Player;

class StaticCollectable : public Object
{
public:
	StaticCollectable(const std::string& texID, const Point& boxSize, const Point& initPos);
	StaticCollectable(const std::string& texID, const AnimationData& animData, const Point& boxSize, const Point& initPos);
	~StaticCollectable() override = default;

	bool GetActive() const final { return !GetCollected() && Object::GetActive(); }

	void SetCollected() { m_collected = true; }
	bool GetCollected() const { return m_collected; }

	virtual void Collect(Object* player) = 0;

private:

	bool m_collected = false;
};

class DynamicCollectable : public DynamicObject
{
public:
	DynamicCollectable(const std::string& texID, const Point& boxSize, const Point& initPos);
	DynamicCollectable(const std::string& texID, const AnimationData& animData, const Point& boxSize, const Point& initPos);
	~DynamicCollectable() override = default;

	bool GetActive() const final { return !GetCollected() && Object::GetActive(); }

	void SetCollected() { m_collected = true; }
	bool GetCollected() const { return m_collected; }

	virtual void Collect(Object* player) = 0;

private:

	bool m_collected = false;
};