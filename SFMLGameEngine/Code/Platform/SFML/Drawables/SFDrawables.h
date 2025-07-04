#pragma once

#include "../../../Engine/Interfaces/IRenderable.h"
#include "../../../Engine/Interfaces/ITransforms.h"
#include "../../../Engine/Interfaces/IUpdatable.h"
#include "../Renderer/SFRenderer.h"
#include <memory>

template <typename T>
class SFDrawables : public IRenderable, public IUpdatable, public ITransforms
{
public:
	virtual ~SFDrawables() = default;

	void Render(IRenderer* renderer) override
	{
		auto* sfRenderer = dynamic_cast<SFRenderer*>(renderer);
		if (!sfRenderer || !m_drawable)
			return;

		// Get native handle as sf::RenderWindow*
		sf::RenderWindow* window = static_cast<sf::RenderWindow*>(sfRenderer->GetWindow()->GetNativeHandle());
		if (window)
		{
			window->draw(*m_drawable);
		}
	}

	virtual void SetPosition(const Point& pos) override
	{
		m_drawable->setPosition(pos);
	}

	virtual Point GetPosition() override
	{
		return m_drawable->getPosition();
	}

	virtual void SetScale(const Point& scl) override
	{
		m_scale = scl;
		m_drawable->setScale(scl);
	}

	virtual Point GetScale() override
	{
		return m_scale;
	}

	Point GetOrigin() override
	{
		return m_drawable->getOrigin();
	}

	void SetOrigin(const Point& ori) override
	{
		m_drawable->setOrigin(ori);
	}

	Point GetSize() override
	{
		auto bounds = m_drawable->getGlobalBounds();
		return bounds.size;
	}

protected:

	std::shared_ptr<T> m_drawable;
};