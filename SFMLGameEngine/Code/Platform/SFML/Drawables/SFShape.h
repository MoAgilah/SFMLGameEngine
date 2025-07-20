#pragma once

#include "SFDrawables.h"
#include "../../../Utilities/Utilities.h"
#include "../../../Engine/Interfaces/IShape.h"
#include <SFML/Graphics.hpp>

template<typename TShape>
class SFShape : public SFDrawables<TShape>, public IShape, public IMoveable
{
public:
    void Move(float x, float y) override { this->Move(Point(x, y)); }
    void Move(const Point& pos) override { this->SetPosition(this->GetPosition() + pos); }
    virtual void Update(const Point& pos) = 0;

    Colour GetFillColour() override { return this->GetPrimaryDrawableAs<TShape>()->getFillColor(); }
    void SetFillColour(const Colour& col) override { this->GetPrimaryDrawableAs<TShape>()->setFillColor(col); }

    Colour GetOutlineColour() override { return this->GetPrimaryDrawableAs<TShape>()->getOutlineColor(); }
    void SetOutlineColour(const Colour& col) override { this->GetPrimaryDrawableAs<TShape>()->setOutlineColor(col); }

    float GetOutlineThickness() override { return this->GetPrimaryDrawableAs<TShape>()->getOutlineThickness(); }
    void SetOutlineThickness(float scale) override { this->GetPrimaryDrawableAs<TShape>()->setOutlineThickness(scale); }

    void SetRotation(float rotation) override { this->GetPrimaryDrawableAs<TShape>()->setRotation(sf::degrees(rotation)); }
};

// --- Rectangle Shape ---
class SFRect : public SFShape<sf::RectangleShape>
{
public:
    SFRect();
    SFRect(const Point& size, const Point& pos);

    void Update(const Point& pos) override;
    void Reset(const Point& size);

    sf::RectangleShape* GetRect() { return this->GetPrimaryDrawableAs<sf::RectangleShape>(); }

    Point GetSize() override { return GetRect()->getSize(); }
    void SetSize(const Point& size) override { GetRect()->setSize(size); }
};

// --- Circle Shape ---
class SFCircle : public SFShape<sf::CircleShape>, public ICircleShape
{
public:
    SFCircle();
    SFCircle(float radius, const Point& pos);

    void Update(const Point& pos) override;
    void Reset(float radius);

    sf::CircleShape* GetCircle() { return this->GetPrimaryDrawableAs<sf::CircleShape>(); }

    float GetRadius() override { return GetCircle()->getRadius(); }
    void SetRadius(float radius) override { GetCircle()->setRadius(radius); }
};

class SFCapsule : public SFShape<sf::Shape>
{
public:
    SFCapsule();
    SFCapsule(float radius, float length, float angle, const Point& pos);

    void Update(const Point& pos) override;
    void Reset(float radius, float length, float angle);

    // Getters for native drawables
    sf::RectangleShape* GetBody();
    sf::CircleShape* GetEndCap1();
    sf::CircleShape* GetEndCap2();

    float GetRadius() const { return m_radius; }
    float GetLength() const { return m_length; }
    float GetAngle() const { return m_angle; }
    const Line& GetSegment() const { return m_segment; }

private:
    float m_angle = 0.f;
    float m_radius = 0.f;
    float m_length = 0.f;
    Line m_segment;
};
