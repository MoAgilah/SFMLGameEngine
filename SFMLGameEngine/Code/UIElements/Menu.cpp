#include "Menu.h"

#include "Game/Constants.h"
#include "Game/GameManager.h"

Menu::Menu(const Point& menuSize, float outlineThickness, const Point& dimensions, const MenuPositionData& menuPositionData)
	: m_menuSpace(menuSize), m_outlineThickness(outlineThickness), m_dimensions(dimensions), m_menuPositionData(menuPositionData)
{
	BuildMenuSpace();
}

void Menu::Update(float deltaTime)
{
	ProcessInput();

	if (m_currCellNumber != m_prevCellNumber)
	{
		if (m_cursor)
			MoveCursor();

		SetActiveTextElement();
	}

	for (auto cellNo : m_activeCells)
	{
		auto cell = GetCell(cellNo);
		if (cell)
		{
			cell->Update(deltaTime);
		}
	}

	m_prevCellNumber = m_currCellNumber;
}

void Menu::Render(sf::RenderWindow& window)
{
	DebugRender(window);

	for (auto& row : m_rows)
	{
		for (auto& cell : row)
			cell.Render(window);
	}

	if (m_cursor)
	{
		m_cursor.value().Render(window);
	}
}

void Menu::SetActiveCells()
{
	for (size_t i = 0; i < m_rows.size(); i++)
	{
		for (size_t j = 0; j < m_rows[i].size(); j++)
		{
			if (m_rows[i][j].GetMenuSlotNumber() >= 0)
			{
				m_activeCells.emplace_back(i, j);
			}
		}
	}
}

Sprite* Menu::GetCursor()
{
	if (m_cursor)
		return &m_cursor.value();

	return nullptr;
}

void Menu::SetCurrCellNumber(int cellNumber)
{
	if (cellNumber < m_activeCells.size())
		m_currCellNumber = cellNumber;
}

MenuItem* Menu::GetCell(const std::pair<int, int>& colRow)
{
	if (colRow.first <= m_rows.size() - 1)
	{
		if (colRow.second <= m_rows[colRow.first].size() - 1)
			return &m_rows[colRow.first][colRow.second];
	}

	return nullptr;
}

MenuItem* Menu::GetCellByCellNumber(unsigned int cellNumber)
{
	if (cellNumber < m_activeCells.size())
	{
		return GetCell(m_activeCells[cellNumber]);
	}
	return nullptr;
}

void Menu::BuildMenuSpace()
{
	m_menuSpace.setOrigin(Point(m_menuSpace.getSize()) / 2.f);

	switch (m_menuPositionData.m_positionMode)
	{
	case MenuPositionMode::Centered:
	{
		// Center menu at m_centerPoint
		m_menuSpace.setPosition(*(m_menuPositionData.m_centerPoint));
		break;
	}
	case MenuPositionMode::Anchored:
	{
		// Compute border area from anchor bounds (like screen size)
		Point border = (*(m_menuPositionData.m_anchorBounds) - m_menuSpace.getSize()) / 2.f;
		border.x += m_menuSpace.getOrigin().x;  // Shift to align left
		m_menuSpace.setPosition(border);
		break;
	}
	}

	m_menuSpace.setOutlineThickness(m_outlineThickness);
	m_menuSpace.setOutlineColor(sf::Color::Red);

	BuildColumns();
	BuildRows();
}

void Menu::BuildColumns()
{
	float columnWidth = m_menuSpace.getSize().x / m_dimensions.x;

	m_columnsSize = Point(columnWidth, m_menuSpace.getSize().y);

	for (size_t i = 0; i < m_dimensions.x; i++)
		m_columns.push_back(sf::RectangleShape(m_columnsSize));

	for (auto& column : m_columns)
	{
		column.setOrigin(m_columnsSize/ 2.f);
		column.setOutlineColor(sf::Color::Yellow);
		column.setOutlineThickness(m_outlineThickness);
	}

	m_menuSpaceTopLeft = m_menuSpace.getPosition() - m_menuSpace.getOrigin();

	m_columns[0].setPosition(m_menuSpaceTopLeft + m_columns[0].getOrigin());

	for (size_t i = 1; i < m_dimensions.x; i++)
		m_columns[i].setPosition(m_columns[i - 1].getPosition() + Point(m_columns[i - 1].getSize().x, 0));
}

void Menu::BuildRows()
{
	float rowHeight = m_menuSpace.getSize().y / m_dimensions.y;
	m_cellsSize = Point(m_columnsSize.x, rowHeight);

	for (size_t i = 0; i < m_dimensions.y; i++)
	{
		std::vector<MenuItem> row;
		for (size_t j = 0; j < m_dimensions.x; j++)
			row.emplace_back(m_cellsSize, m_outlineThickness);  // constructs in-place

		m_rows.push_back(std::move(row));
	}

	if (m_rows.empty())
		return;

	// 1st row
	auto& row = m_rows[0];

	if (row.empty())
		return;

	row[0].SetPosition(m_menuSpaceTopLeft + row[0].GetOrigin());

	for (size_t i = 1; i < row.size(); i++)
		row[i].SetPosition(row[i-1].GetPosition() + sf::Vector2f(row[i-1].GetSize().x, 0));

	// remaining rows
	for (size_t i = 1; i < m_rows.size(); i++)
	{
		for (size_t j = 0; j < m_rows[i].size(); j++)
			m_rows[i][j].SetPosition(m_rows[i - 1][j].GetPosition() + sf::Vector2f(0, m_rows[i - 1][j].GetSize().y));
	}
}

void Menu::DebugRender(sf::RenderWindow& window)
{
	window.draw(m_menuSpace);

	for (auto& col : m_columns)
		window.draw(col);
}

void Menu::ProcessInput()
{
	HandleNavigation();
}

void Menu::HandleNavigation()
{
	auto& inputManager = GameManager::Get()->GetInputManager();

	if (m_verticalScroll)
	{
		HandleDirection(inputManager.GetKeyState(sf::Keyboard::Up), m_canIncMenu, -1);
		HandleDirection(inputManager.GetKeyState(sf::Keyboard::Down), m_canDecMenu, 1);
	}
	else
	{
		HandleDirection(inputManager.GetKeyState(sf::Keyboard::Left), m_canIncMenu, -1);
		HandleDirection(inputManager.GetKeyState(sf::Keyboard::Right), m_canDecMenu, 1);
	}
}

void Menu::HandleDirection(bool isPressed, bool& canMove, int direction)
{
	if (isPressed)
	{
		if (canMove)
		{
			SetCurrCellNumber(m_currCellNumber + direction);
			canMove = false;
		}
	}
	else
	{
		canMove = true;
	}
}

void Menu::MoveCursor()
{
	auto cell = GetCellByCellNumber(m_currCellNumber);
	if (m_cursor)
		m_cursor->SetPosition(cell->GetPosition());
}

void Menu::SetActiveTextElement()
{
	for (auto cellID : m_activeCells)
	{
		auto cell = GetCell(cellID);
		if (cell)
		{
			auto text = cell->GetTextElement();
			if (text)
			{
				if (cell->GetMenuSlotNumber() == m_currCellNumber)
				{
					if (text->IsAnimated())
						dynamic_cast<AnimatedText*>(text)->Resume();

					if (m_passiveColour)
						text->ResetOutlineColour();
				}
				else
				{
					if (text->IsAnimated())
						dynamic_cast<AnimatedText*>(text)->Pause();

					if (m_passiveColour)
						text->SetOutlineColour(*m_passiveColour);
				}
			}
		}
	}
}
