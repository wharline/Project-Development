#include "stdafx.h"
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
			m_grid[r][c].setXPos( r );
			m_grid[r][c].setYPos( c );
		}
	}
}

bool Grid::canPassThrough ( int x, int y, int allegiance )
{
   return m_grid[x][y].canPassThrough( allegiance );
}

bool Grid::isEmpty ( int x, int y )
{
   return m_grid[x][y].isEmpty();
}

bool Grid::isTrapped ( int x, int y )
{
   return m_grid[x][y].isTrapped();
}

void Grid::setTrap ( int x, int y, int trapLevel )
{
}

void Grid::removeTrap ()
{
}


void Grid::moveToSpace ( int x, int y )
{
	m_grid[x][y].setState( Tile::occupied );
}

Tile* Grid::getCell ( int rowNum, int colNum )
{
	return &m_grid[rowNum][colNum];
}