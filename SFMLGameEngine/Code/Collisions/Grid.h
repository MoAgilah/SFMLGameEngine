#pragma once

#include "../Game/Constants.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

class Tile;

class Grid {
public:
	Grid(int rows, int columns, const std::string& tileFilePaths = GameConstants::TileFilePaths);
	~Grid() = default;

	void Render(sf::RenderWindow& window);
	Tile* GetTile(int x, int y);
	const std::vector<std::shared_ptr<Tile>>& GetGrid() const { return m_grid; }

private:
	void LoadTileTypes(const std::string& tileFilePaths);
	void ArrangeTilePositions();

	int m_rows = 0;
	int m_columns = 0;
	std::vector<std::shared_ptr<Tile>> m_grid;
};