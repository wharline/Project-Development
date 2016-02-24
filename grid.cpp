#include "grid.h"

Grid::Grid ()
{
	m_grid = NULL;
}

Grid::~Grid ()
{
}

void Grid::init ( int rowNum, int colNum )
{
	m_rowNum = rowNum;
	m_colNum = colNum;

	m_grid = new Tile*[m_rowNum];

	for ( int r = 0; r < m_rowNum; r++ )
	{
		m_grid[r] = new Tile[m_colNum];

		for ( int c = 0; c < m_colNum; c++ )
		{
			m_grid[r][c].x = r;
			m_grid[r][c].y = c;
		}
	}
}

Grid::Tile* Grid::getCell ( int rowNum, int colNum )
{
	return &m_grid[rowNum][colNum];
}