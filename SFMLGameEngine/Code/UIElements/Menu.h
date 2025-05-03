#pragma once

#include "MenuItem.h"
#include "../Drawables/Sprite.h"
#include "../Utilities/Point.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>

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

class Menu
{
public:
	Menu(const Point& menuSize, float outlineThickness, const Point& dimensions, const MenuPositionData& menuPositionData);
	~Menu() = default;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void SetHorizontalScrolling() { m_verticalScroll = false; }

	void SetActiveCells();

	Point GetCellSize() const { return m_cellsSize; }

	void SetCursor(Sprite* spr) { m_cursor = std::shared_ptr<Sprite>(spr); }
	Sprite* GetCursor();

	void SetCurrCellNumber(int cellNumber);

	void SetPassiveColour(const sf::Color& colour) { m_passiveColour = colour; }

	MenuItem* GetCell(const std::pair<int,int>& colRow);
	MenuItem* GetCellByCellNumber(unsigned int cellNumber);

private:

	void BuildMenuSpace();
	void BuildColumns();
	void BuildRows();

	void DebugRender(sf::RenderWindow& window);

	void ProcessInput();

	void MoveCursor();
	void SetActiveTextElement();

	void HandleNavigation();

	void HandleDirection(bool isPressed, bool& canMove, int direction);

	bool m_canIncMenu = true;
	bool m_canDecMenu = true;
	bool m_verticalScroll = true;

	Point m_cellsSize;
	Point m_dimensions;
	Point m_columnsSize;
	Point m_menuSpaceCenter;
	Point m_menuSpaceTopLeft;
	float m_outlineThickness;
	unsigned int m_currCellNumber;
	unsigned int m_prevCellNumber = -1;
	sf::RectangleShape m_menuSpace;
	std::shared_ptr<Sprite> m_cursor;
	MenuPositionData m_menuPositionData;
	std::optional<sf::Color> m_passiveColour;
	std::vector<sf::RectangleShape> m_columns;
	std::vector<std::vector<MenuItem>> m_rows;
	std::vector<std::pair<int, int>> m_activeCells;
};