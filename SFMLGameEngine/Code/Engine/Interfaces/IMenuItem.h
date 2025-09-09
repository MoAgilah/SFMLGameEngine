#pragma once

#include "IText.h"
#include "ISprite.h"
#include "IShape.h"
#include "../../Utilities/Point.h"
#include <memory>

class IMenuItem
{
public:
	IMenuItem()
		: m_menuSlotNumber(-1), m_textElement(nullptr), m_spriteElement(nullptr)
	{}
	virtual ~IMenuItem() = default;

	virtual void Update(float deltaTime)
	{
		if (m_textElement)
			m_textElement->Update(deltaTime);

		if (m_spriteElement)
			m_spriteElement->Update(deltaTime);
	}

	virtual void Render(IRenderer* renderer)
	{
#if defined _DEBUG
		m_cellSpace->Render(renderer);
#endif

		if (m_textElement)
			m_textElement->Render(renderer);

		if (m_spriteElement)
			m_spriteElement->Render(renderer);
	}

	int GetMenuSlotNumber() const { return m_menuSlotNumber; }
	void SetMenuSlotNumber(int slotNumber) { m_menuSlotNumber = slotNumber; }

	virtual Point GetPosition() const = 0;
	virtual void SetPosition(const Point& position) = 0;

	virtual Point GetOrigin() const = 0;
	virtual Point GetSize() const = 0;

	virtual IText* AddTextElement(std::shared_ptr<IText> text) = 0;
	IText* GetTextElement()
	{
		if (m_textElement)
			return m_textElement.get();

		return nullptr;
	}

	virtual ISprite* AddSpriteElement(std::shared_ptr<ISprite> spr) = 0;
	ISprite* GetSpriteElement()
	{
		if (m_spriteElement)
			return m_spriteElement.get();

		return nullptr;
	}

protected:

	int m_menuSlotNumber;
	std::shared_ptr<IBoxShape> m_cellSpace;
	std::shared_ptr<IText> m_textElement;
	std::shared_ptr<ISprite> m_spriteElement;
};
