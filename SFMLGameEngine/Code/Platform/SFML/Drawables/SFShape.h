#pragma once

#include "SFDrawables.h"
#include "../../../Utilities/Traits.h"
#include "../../../Utilities/Utilities.h"
#include "../../../Engine/Interfaces/IShape.h"
#include <SFML/Graphics.hpp>

template<typename TShape>
class SFShape : public SFDrawables<TShape>, public IShape, public IMoveable
{
public:
    void Move(float x, float y) override { this->GetPrimaryDrawableAs<TShape>()->move(Point(x, y)); }
    void Move(const Point& vel) override { this->Move(vel.x, vel.y); }

    Colour GetFillColour() override { return this->GetPrimaryDrawableAs<TShape>()->getFillColor(); }
    void SetFillColour(const Colour& col) override { this->GetPrimaryDrawableAs<TShape>()->setFillColor(col); }

    Colour GetOutlineColour() override { return this->GetPrimaryDrawableAs<TShape>()->getOutlineColor(); }
    void SetOutlineColour(const Colour& col) override { this->GetPrimaryDrawableAs<TShape>()->setOutlineColor(col); }

    float GetOutlineThickness() override { return this->GetPrimaryDrawableAs<TShape>()->getOutlineThickness(); }
    void SetOutlineThickness(float scale) override { this->GetPrimaryDrawableAs<TShape>()->setOutlineThickness(scale); }

    void SetRotation(float rotation) override { this->GetPrimaryDrawableAs<TShape>()->setRotation(sf::degrees(rotation)); }
};

class SFTriangle : public SFShape<sf::ConvexShape>, public ITriangleShape
{
public:
    SFTriangle();
    SFTriangle(const std::array<Point, 3>& points, const Point& pos);

    void Update(const Point& pos) override;
    void Render(IRenderer* renderer) override;

    void Reset(const std::array<Point, 3>& points);

    sf::ConvexShape* GetTriangle() { return this->GetPrimaryDrawableAs<sf::ConvexShape>(); }

    Point GetPoint(int idx) override;
    Line GetLine(int start, int end) override;

    // Returns the three points of the triangle
    std::array<Point, 3> GetPoints() const override;

    // Sets the three points of the triangle
    void SetPoints(const std::array<Point, 3>& points) override;
};

// --- Rectangle Shape ---
class SFRect : public SFShape<sf::RectangleShape>, public IBoxShape
{
public:
    SFRect();
    SFRect(const Point& size, const Point& pos);

    void Update(const Point& pos) override;
    void Render(IRenderer* renderer) override;

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
    void Render(IRenderer* renderer) override;

    void Reset(float radius);

    sf::CircleShape* GetCircle() { return this->GetPrimaryDrawableAs<sf::CircleShape>(); }

    float GetRadius() override { return GetCircle()->getRadius(); }
    void SetRadius(float radius) override { GetCircle()->setRadius(radius); }
};

class SFCapsule : public SFShape<sf::Shape>, public ICapsuleShape
{
public:
    SFCapsule();
    SFCapsule(float radius, float length, float angle, const Point& pos);

    void Update(const Point& pos) override;
    void Render(IRenderer* renderer) override;

    void Reset(float radius, float length, float angle);

    // Getters for native drawables
    sf::RectangleShape* GetBody();
    sf::CircleShape* GetEndCap1();
    sf::CircleShape* GetEndCap2();

    void Move(float x, float y) override;

    void SetSize(const Point& size) override { GetBody()->setSize(size); }
};

template <>
struct CapsuleTraits<SFCapsule>
{
    using CircleType = SFCircle;
    using BoxType = SFRect;
};