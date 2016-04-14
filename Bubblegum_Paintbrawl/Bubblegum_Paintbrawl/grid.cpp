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

bool Grid::init ( int rowNum, int colNum )
{
	m_rowNum = rowNum;
	m_colNum = colNum;

	m_grid = new Tile*[m_rowNum];

	for ( int r = 0; r < m_rowNum; r++ )
	{
		m_grid[r] = new Tile[m_colNum];

		for ( int c = 0; c < m_colNum; c++ )
		{
         m_grid[r][c].init();

			m_grid[r][c].setXPos( r );
			m_grid[r][c].setYPos( c );
		}
	}

   dummyTile.init();

   return true;
}

void Grid::shutdown ()
{
   for ( int r = 0; r < m_rowNum; r++ )
   {
      delete[] m_grid[r];
   }
   delete[] m_grid;
}

bool Grid::canPassThrough ( int x, int y, int allegiance )
{
   if ( x < 0 || x >= m_rowNum )
      return false;
   if ( y < 0 || y >= m_colNum )
      return false;

   return getCell( x, y ).canPassThrough( allegiance );
}

bool Grid::isEmpty ( int x, int y )
{
   if ( x < 0 || x >= m_rowNum )
      return false;
   if ( y < 0 || y >= m_colNum )
      return false;

   return getCell( x, y ).isEmpty();
}

bool Grid::isTrapped ( int x, int y )
{
   if ( x < 0 || x >= m_rowNum )
      return false;
   if ( y < 0 || y >= m_colNum )
      return false;

   return getCell( x, y ).isTrapped();
}

void Grid::setTrap ( int x, int y, int trapLevel )
{
   if ( x < 0 || x >= m_rowNum )
      return;
   if ( y < 0 || y >= m_colNum )
      return;

   getCell( x, y ).setTrap( trapLevel );
}

void Grid::removeTrap ()
{
   // remove a trap on a tile if that tile is occupied
   for ( int r = 0; r < m_rowNum; r++ )
   {
      for ( int c = 0; c < m_colNum; c++ )
      {
         if ( getCell( r, c ).isOccupied() )
            getCell( r, c ).removeTrap();
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
		checkReachableTiles(x, y - 1, remainingMoves - 1, allegiance);
	}
	// Check Right
	if ( ( x < ( m_colNum - 1 ) ) && ( remainingMoves > 0 ) )
	{
		checkReachableTiles(x + 1, y, remainingMoves - 1, allegiance);
	}
	// Check Bottom
	if ( ( y < ( m_rowNum - 1 ) ) && ( remainingMoves > 0 ) )
	{
		checkReachableTiles(x, y + 1, remainingMoves - 1, allegiance);
	}

}

void Grid::moveToSpace ( int oldX, int oldY, int newX, int newY, int allegiance )
{
   if ( newX < 0 || newX >= m_rowNum )
      return;
   if ( newY < 0 || newY >= m_colNum )
      return;

   getCell( oldX, oldY ).setState( Tile::empty );
	getCell( newX, newY ).setState( Tile::occupied, allegiance );

   // change tile color back to white
   for ( int r = 0; r < m_rowNum; r++ )
   {
      for ( int c = 0; c < m_colNum; c++ )
      {
         getCell( r, c ).setColor( D3DCOLOR_XRGB( 255, 255, 255 ) );
      }
   }
}

void Grid::showRange ( int x, int y, int range, D3DCOLOR color )
{
   // visit tiles around x and y coordinates within the range given
   checkReachableTiles( x, y, range, -1 );

   for ( int i = 0; i < (int)reachableTiles.size(); i++ )
   {
      Tile& tile = getCell( reachableTiles.at( i ).x, reachableTiles.at( i ).y );

      tile.setColor( color );
   }

   reachableTiles.clear();
}

Tile& Grid::getCell ( int rowNum, int colNum )
{
   if ( rowNum < 0 || rowNum >= m_rowNum )
      return dummyTile;
   if ( colNum < 0 || colNum >= m_colNum )
      return dummyTile;

   Tile& tile = m_grid[rowNum][colNum];
	
   return tile;
}

DxTexture& Grid::getTileTexture ( int rowNum, int colNum )
{
   Tile& tile = getCell( rowNum, colNum );

   DxTexture& tex = tile.texture();

   return tex;
}

D3DCOLOR Grid::tileColor ( int x, int y )
{
   if ( x < 0 || x >= m_rowNum )
      return D3DCOLOR_XRGB( 0, 0, 0 );
   if ( y < 0 || y >= m_colNum )
      return D3DCOLOR_XRGB( 0, 0, 0 );

   return getCell( x, y ).color();
}

void Grid::spaceSelected ( int x, int y )
{
   if ( x < 0 || x >= m_rowNum )
      return;
   if ( y < 0 || y >= m_colNum )
      return;

   getCell( x, y ).setColor( D3DCOLOR_XRGB( 100, 100, 100 ) );
}