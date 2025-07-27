#include "SFGrid.h"

SFGrid::SFGrid(int rows, int columns, const std::string& fontName, const std::string& tileFilePaths)
	: IGrid(rows, columns)
{
	Init<SFTile>(fontName, tileFilePaths);
}