#include "Grid.h"

#include "Tile.h"
#include "../Core/Constants.h"
#include "../Core/GameManager.h"
#include <algorithm>
#include <format>
#include <fstream>
#include <sstream>

Grid::Grid(int rows, int columns, const std::string & tileFilePaths)
	: m_rows(rows), m_columns(columns)
{
	const auto font = GameManager::Get()->GetFontMgr().GetFont("Standard");
	m_grid.reserve(m_rows * m_columns);

	for (int y = 0; y < m_rows; ++y)
	{
		for (int x = 0; x < m_columns; ++x)
			m_grid.emplace_back(std::make_shared<Tile>(x, y, font));
	}

	if (!m_grid.empty())
	{
		LoadTileTypes(tileFilePaths);
		ArrangeTilePositions();
	}
}

void Grid::LoadTileTypes(const std::string& tileFilePaths)
{
	if (!std::filesystem::exists(tileFilePaths))
		throw std::runtime_error("Tile file path does not exist: " + tileFilePaths);

	std::ifstream inFile(tileFilePaths);
	if (!inFile)
		throw std::runtime_error("Failed to open tile file: " + tileFilePaths);

	std::vector<int> types(std::istream_iterator<int>(inFile), {});
	inFile.close();

	if (types.size() != m_grid.size())
		throw std::runtime_error("Mismatch between tile data and grid size.");

	for (size_t i = 0; i < types.size(); ++i)
		m_grid[i]->SetType(types[i]);
}

void Grid::ArrangeTilePositions()
{
	const auto tileSize = m_grid.front()->GetBoundingBox()->GetExtents();
	Point pos = tileSize;

	for (int y = 0; y < m_rows; ++y)
	{
		for (int x = 0; x < m_columns; ++x)
		{
			int index = y * m_columns + x;
			Point tilePos{
				tileSize.x + (x * tileSize.x * 2),
				tileSize.y + (y * tileSize.y * 2)
			};
			m_grid[index]->SetPosition(tilePos);
		}
	}
}

void Grid::Render(sf::RenderWindow& window)
{
	for (const auto& tile : m_grid)
	{
		if (tile->GetActive()) {
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

	auto it = std::find_if(m_grid.begin(), m_grid.end(),
		[id](const auto n) { return n->GetID() == id; });

	if (it != m_grid.end())
		return it->get();

	//extract tile if tile exists
	return nullptr;
}