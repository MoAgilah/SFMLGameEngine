#include "../Collisions/Grid.h"
#include <format>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../Collisions/Tile.h"

Grid::Grid(int rows, int columns, const std::string& tileFilePaths)
	: m_rows(rows), m_columns(columns)
{
	auto font = GameManager::Get()->GetFontMgr().GetFont("Standard");

	//create grid for entire level
	for (int y = 0; y < m_rows; y++)
	{
		for (int x = 0; x < m_columns; x++)
			m_grid.push_back(std::make_shared<Tile>(x, y, font));
	}

	if (!m_grid.empty())
	{
		SetTileTypes(tileFilePaths);
		SetTilePosition();
	}
}

void Grid::SetTilePosition()
{
	int x = 0;
	int begin = x;

	//first row
	Point pos(m_grid.front()->GetBoundingBox()->GetExtents());
	m_grid[x]->SetPosition(pos);

	for (x = x + 1; x < m_columns; x++)
	{
		pos = Point(pos.x + (m_grid.front()->GetBoundingBox()->GetExtents().x * 2), pos.y);
		m_grid[x]->SetPosition(pos);
	}

	//remaining rows
	for (int i = 0; i < m_rows - 1; i++)
	{
		pos = Point(m_grid[begin]->GetPosition().x, m_grid[begin]->GetPosition().y + (m_grid.front()->GetBoundingBox()->GetExtents().y * 2));
		m_grid[x]->SetPosition(pos);
		begin = x;

		int val = 2 + i;

		for (x = x + 1; x < m_columns * val; x++)
		{
			pos = Point(pos.x + (m_grid.front()->GetBoundingBox()->GetExtents().x * 2), pos.y);
			m_grid[x]->SetPosition(pos);
		}
	}
}

void Grid::Render(sf::RenderWindow & window)
{
	for (auto& tile : m_grid)
	{
		if (tile->GetActive())
		{
#if _DEBUG
			if (GameConstants::DRender)
				tile->Render(window);
#endif
		}
	}
}

Tile* Grid::GetTile(int x, int y)
{
	//create id
	std::string id = std::format("{},{}", x, y);

	auto it = std::find_if(m_grid.begin(), m_grid.end(), [id](auto n) { return n->GetID() == id; });
	if (it != m_grid.end())
		return it->get();

	//extract tile if tile exists
	return nullptr;
}

void Grid::SetTileTypes(const std::string& tileFilePaths)
{
	std::ifstream inFile;
	std::vector<int> types;

	//extract tile types from text file
	inFile.open(tileFilePaths);

	int type;
	while (inFile >> type)
		types.push_back(type);

	inFile.close();

	//assign tile types
	for (size_t i = 0; i < types.size(); i++)
		m_grid[i]->SetType(types[i]);
}