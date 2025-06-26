#pragma once

#include "SFDrawables.h"
#include "../../../Utilities/Utilities.h"
#include <SFML/Graphics.hpp>

class SFShape : public SFDrawables<sf::Shape>
{
public:
	void Move(float x, float y);
	void Move(const Point& mov);

	sf::Color GetFillColour() { return m_drawable->getFillColor(); }
	void SetFillColour(const sf::Color& col) { m_drawable->setFillColor(col); }

	sf::Color GetOutlineColour() { return m_drawable->getOutlineColor(); }
	void SetOutlineColour(const sf::Color& col) { m_drawable->setOutlineColor(col); }

	float GetOutlineThickness() { return m_drawable->getOutlineThickness(); }
	void SetOutlineThickness(float scale) { m_drawable->setOutlineThickness(scale); }

protected:

	virtual void UpdateShape(const Point& pos) = 0;
};

class SFRect : public SFShape
{
public:
	SFRect(const Point& size);

	void Update(float deltaTime) override;

	void Reset(const Point& size);

	sf::RectangleShape* GetRect() { return static_cast<sf::RectangleShape*>(m_drawable.get()); }

protected:

	void UpdateShape(const Point& pos) override;

private:

	Point m_min;
	Point m_max;
	Point m_extents;
};

class SFCircle : public SFShape
{
public:
	SFCircle(float radius);

	void Update(float deltaTime) override;

	void Reset(float radius);

	sf::CircleShape* GetCircle() { return static_cast<sf::CircleShape*>(m_drawable.get()); }

protected:

	void UpdateShape(const Point& pos) override;

private:

	float m_radius;
};

class SFCapsule : public SFShape
{
public:
	SFCapsule(float radius, float length, float angle);

	void Update(float deltaTime) override;
	void Reset(float radius, float length, float angle);

protected:

	void UpdateShape(const Point& pos) override;

private:

	float m_angle;
	float m_radius;
	float m_length;
	Line m_segment;
	sf::CircleShape m_circle1;
	sf::CircleShape m_circle2;
};