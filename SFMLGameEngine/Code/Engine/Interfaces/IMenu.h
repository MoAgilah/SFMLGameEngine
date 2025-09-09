#pragma once

#include "IMenuCursor.h"
#include "IMenuItem.h"
#include "ISprite.h"
#include "IShape.h"
#include "../UI/MenuNavigation.h"
#include "../../Utilities/Point.h"
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

enum MenuPositionMode
{
	Centered,
	Anchored
};

struct MenuPositionData
{
	MenuPositionMode m_positionMode;
	std::optional<Point> m_centerPoint;
	std::optional<Point> m_anchorBounds;

	MenuPositionData(MenuPositionMode positionMode, const Point& centerOrAnchor)
		: m_positionMode(positionMode)
	{
		switch (m_positionMode)
		{
		case Centered:
			m_centerPoint = centerOrAnchor;
			m_anchorBounds = std::nullopt;
			break;
		case Anchored:
			m_anchorBounds = centerOrAnchor;
			m_centerPoint = std::nullopt;
			break;
		}
	}
};

class IMenu
{
public:
	IMenu(float outlineThickness, const Point& dimensions, const MenuPositionData& menuPositionData);
	virtual ~IMenu() = default;

	virtual void Update(float dt);
	virtual void Render(IRenderer* renderer);

	void SetActiveCells();

	MenuNav* GetMenuNav() { return &m_menuNavigation; }

	Point GetCellSize() const { return m_cellsSize; }

	virtual void AddCursor(ISprite* spr, const MenuNav& menuNav) = 0;
	IMenuCursor* GetCursor(unsigned int cursorNumber);

	void SetPassiveColour(const sf::Color& colour) { m_passiveColour = colour; }

	IMenuItem* GetCell(const std::pair<int, int>& colRow);
	IMenuItem* GetCellByCellNumber(unsigned int cellNumber);

protected:

	virtual void BuildMenuSpace() = 0;
	virtual void BuildColumns() = 0;
	virtual void BuildRows() = 0;

	void ProcessInput();

	virtual void SetActiveTextElement() = 0;

	Point m_cellsSize;
	Point m_dimensions;
	Point m_columnsSize;
	Point m_menuSpaceCenter;
	Point m_menuSpaceTopLeft;
	float m_outlineThickness;
	unsigned int m_prevCellNumber = -1;
	MenuNav m_menuNavigation;
	std::shared_ptr<IBoxShape> m_menuSpace;
	MenuPositionData m_menuPositionData;
	std::optional<Colour> m_passiveColour;
	std::vector<std::shared_ptr<IBoxShape>> m_columns;
	std::vector<std::vector<std::shared_ptr<IMenuItem>>> m_rows;
	std::vector<std::pair<int, int>> m_activeCells;
	std::vector<std::shared_ptr<IMenuCursor>> m_cursors;
};