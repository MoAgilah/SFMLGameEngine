#include "Menu.h"

#include "Game/Constants.h"
#include "Game/GameManager.h"

Menu::Menu(const Point& menuSize, float outlineThickness, const Point& dimensions, const MenuPositionData& menuPositionData)
	: m_menuSpace(menuSize), m_outlineThickness(outlineThickness), m_dimensions(dimensions), m_menuPositionData(menuPositionData), m_menuNavigation(sf::Keyboard::Up, sf::Keyboard::Down)
{
	BuildMenuSpace();
}

void Menu::Update(float deltaTime)
{
	ProcessInput();

	if (!m_cursors.empty())
	{
		for (auto& cursor : m_cursors)
		{
			auto menuNav = cursor.GetMenuNav();

			if (menuNav->HasMoved())
			{
				int cellNo = menuNav->GetCurrCursorPos();
				auto cell = GetCellByCellNumber(cellNo);
				if (cell)
				{
					cursor.SetPosition(cell->GetPosition());
					menuNav->SetPrevCursorPos(cellNo);
				}
			}
		}
	}
	else
	{
		if (m_menuNavigation.HasMoved())
		{
			SetActiveTextElement();
		}
	}

	for (const auto& cellNo : m_activeCells)
	{
		auto cell = GetCell(cellNo);
		if (cell)
		{
			cell->Update(deltaTime);
		}
	}
}

void Menu::Render(sf::RenderWindow& window)
{
	DebugRender(window);

	for (auto& row : m_rows)
	{
		for (auto& cell : row)
			cell.Render(window);
	}

	if (!m_cursors.empty())
	{
		for (auto& cursor : m_cursors)
			cursor.Render(window);
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

void Menu::AddCursor(Sprite* spr, const MenuNav& menuNav)
{
	m_cursors.push_back({ spr, menuNav });
}

MenuCursor* Menu::GetCursor(unsigned int cursorNumber)
{
	if (cursorNumber >= 0 && cursorNumber < m_cursors.size())
	{
		return &m_cursors[cursorNumber];
	}

	return nullptr;
}

MenuItem* Menu::GetCell(const std::pair<int, int>& colRow)
{
	if (colRow.first >= 0 && colRow.first < m_rows.size())
	{
		if (colRow.second >= 0 && colRow.second < m_rows[colRow.first].size())
			return &m_rows[colRow.first][colRow.second];
	}

	return nullptr;
}

MenuItem* Menu::GetCellByCellNumber(unsigned int cellNumber)
{
	if (cellNumber >= 0 && cellNumber < m_activeCells.size())
		return GetCell(m_activeCells[cellNumber]);

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
	if (!m_cursors.empty())
	{
		for (auto& cursor : m_cursors)
			cursor.GetMenuNav()->HandleNavigation();
	}
	else
	{
		m_menuNavigation.HandleNavigation();
	}
}

void Menu::SetActiveTextElement()
{
	for (const auto& cellID : m_activeCells)
	{
		auto cell = GetCell(cellID);
		if (cell)
		{
			auto text = cell->GetTextElement();
			if (text)
			{
				if (cell->GetMenuSlotNumber() == m_menuNavigation.GetCurrCursorPos())
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
