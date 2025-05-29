#pragma once

#include "../Game/Constants.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>

class Tile;

class Grid
{
public:
	Grid(int rows, int columns, const std::string& tileFilePaths = GameConstants::TileFilePaths);
	~Grid() = default;

	void Render(sf::RenderWindow& window);

	Tile* GetTile(int x, int y);
	std::vector<std::shared_ptr<Tile>> GetGrid() { return m_grid; }

private:

	void SetTileTypes(const std::string& tileFilePaths);
	void SetTilePosition();

	int m_rows = 0;
	int m_columns = 0;
	std::vector<std::shared_ptr<Tile>> m_grid;
};