#pragma once

#pragma once

#include "IRenderable.h"
#include "IUpdatable.h"
#include "ITransforms.h"

class IDrawableNoUpdate : public IRenderable, public ITransforms
{
public:
	virtual ~IDrawableNoUpdate() = default;
};

class IDrawable : public IRenderable, public IUpdatable, public ITransforms
{
public:
	virtual ~IDrawable() = default;
};

template<typename TDrawable>
class CompoundDrawableHolder
{
public:
    virtual ~CompoundDrawableHolder() = default;

    // --- SINGLE DRAWABLE SUPPORT ---

    // Set as the *only* drawable (replaces all)
    void SetDrawable(const std::shared_ptr<TDrawable>& drawable)
    {
        m_drawables.clear();
        if (drawable)
            m_drawables.push_back(drawable);
    }

    // Access first/primary drawable (for single-shape logic)
    TDrawable* GetPrimaryDrawable() {
        return m_drawables.empty() ? nullptr : m_drawables[0].get();
    }
    const TDrawable* GetPrimaryDrawable() const {
        return m_drawables.empty() ? nullptr : m_drawables[0].get();
    }

    template <typename T>
    T* GetPrimaryDrawableAs() {
        return m_drawables.empty() ? nullptr : dynamic_cast<T*>(m_drawables[0].get());
    }
    template <typename T>
    const T* GetPrimaryDrawableAs() const {
        return m_drawables.empty() ? nullptr : dynamic_cast<const T*>(m_drawables[0].get());
    }

    // --- MULTIPLE DRAWABLE SUPPORT ---

    // Add a drawable (compound use)
    void AddDrawable(const std::shared_ptr<TDrawable>& drawable)
    {
        if (drawable)
            m_drawables.push_back(drawable);
    }

    // Access all drawables
    std::vector<std::shared_ptr<TDrawable>>& GetDrawables() { return m_drawables; }
    const std::vector<std::shared_ptr<TDrawable>>& GetDrawables() const { return m_drawables; }

protected:

    std::vector<std::shared_ptr<TDrawable>> m_drawables;

};