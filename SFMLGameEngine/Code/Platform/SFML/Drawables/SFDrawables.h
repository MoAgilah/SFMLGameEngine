#pragma once

#include "../Renderer/SFRenderer.h"
#include "../../../Engine/Interfaces/IDrawable.h"
#include <memory>

template <typename T>
class SFDrawables : public IDrawable, public CompoundDrawableHolder<T>
{
public:
    virtual ~SFDrawables() = default;

    // Render only the primary drawable for base case
    void Render(IRenderer* renderer) override
    {
        auto* sfRenderer = dynamic_cast<SFRenderer*>(renderer);
        auto* drawable = this->GetPrimaryDrawableAs<T>();
        if (!sfRenderer || !drawable)
            return;

        sf::RenderWindow* window = static_cast<sf::RenderWindow*>(sfRenderer->GetWindow()->GetNativeHandle());
        if (window)
        {
            window->draw(*drawable);
        }
    }

    // Now these all work on the primary
    void SetPosition(const Point& pos) override
    {
        auto* drawable = this->GetPrimaryDrawableAs<T>();
        if (drawable) drawable->setPosition(pos);
    }

    Point GetPosition() override
    {
        auto* drawable = this->GetPrimaryDrawableAs<T>();
        return drawable ? Point(drawable->getPosition()) : Point();
    }

    void SetScale(const Point& scl) override
    {
        m_scale = scl;
        auto* drawable = this->GetPrimaryDrawableAs<T>();
        if (drawable) drawable->setScale(scl);
    }

    Point GetScale() override
    {
        return m_scale;
    }

    Point GetOrigin() override
    {
        auto* drawable = this->GetPrimaryDrawableAs<T>();
        return drawable ? Point(drawable->getOrigin()) : Point();
    }

    void SetOrigin(const Point& ori) override
    {
        auto* drawable = this->GetPrimaryDrawableAs<T>();
        if (drawable) drawable->setOrigin(ori);
    }

    Point GetSize() override
    {
        auto* drawable = this->GetPrimaryDrawableAs<T>();
        if (drawable)
        {
            auto bounds = drawable->getGlobalBounds();
            return bounds.size;
        }
        return Point();
    }

    void SetSize(const Point& size) override
    {
        // Implement if needed for specific shape types
    }
};
