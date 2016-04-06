#include "stdafx.h"
#include "grid.h"
#include "assert.h"
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
   m_grid[x][y].setTrap( trapLevel );
}

void Grid::removeTrap ()
{
   for ( int r = 0; r < m_rowNum; r++ )
   {
      for ( int c = 0; c < m_colNum; c++ )
      {
         if ( m_grid[r][c].isOccupied() )
            m_grid[r][c].removeTrap();
      }
   }
}

void Grid::checkReachableTiles(int x, int y, int remainingMoves, int allegiance)
{
	// Ensure that the provided coordinates do not breach the bounds of the Grid
	if( ( x < 0 || y < 0 ) || (x > m_colNum || y > m_rowNum ))
	{
		assert(false);
	}
	// Create point to store tile locations with.
	POINT currentPoint;
	currentPoint.x = x;
	currentPoint.y = y;

	// Make reference to current Tile
	Tile& currentTile = m_grid[x][y];
	
	if( currentTile.isAccessible(allegiance) == true ) reachableTiles.push_back(currentPoint);

	// Check Left
	if ( ( x > 0) && (remainingMoves > 0) )
	{
		checkReachableTiles(x - 1, y, remainingMoves - 1, allegiance);
	}
	// Check Top
	if ( ( y > 0) && (remainingMoves > 0) )
	{
		checkReachableTiles(y - 1, y, remainingMoves - 1, allegiance);
	}
	// Check Right
	if ( ( x < ( m_colNum - 1 ) ) && ( remainingMoves > 0 ) )
	{
		checkReachableTiles(x + 1, y, remainingMoves - 1, allegiance);
	}
	// Check Bottom
	if ( ( y < ( m_rowNum - 1 ) ) && ( remainingMoves > 0 ) )
	{
		checkReachableTiles(y + 1, y, remainingMoves - 1, allegiance);
	}

}

void Grid::moveToSpace ( int x, int y )
{
	m_grid[x][y].setState( Tile::occupied );
}

Tile* Grid::getCell ( int rowNum, int colNum )
{
	return &m_grid[rowNum][colNum];
}
