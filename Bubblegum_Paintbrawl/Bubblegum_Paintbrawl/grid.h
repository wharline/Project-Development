//-------------------------------------------------
//
// Representation of the grid, i.e. the board in 
//	the strategy game.
//
//-------------------------------------------------

#pragma once
#if !defined ( __Grid_H__ )
#define __Grid_H__

#include "stdafx.h"
#include <string>
#include "Tile.h"
#include "Unit.h"
#include <vector>

class Grid : public GridInterface
{
	// public constructors/destructors/assign/copy methods
public:
	Grid ();
	~Grid ();

	// public methods
public:
	// Create a new grid with the specified number of rows and columns
	bool init ( int rowNum, int colNum );
   void shutdown ();
	bool canPassThrough ( int x, int y, int allegiance );
	bool isEmpty ( int x, int y );
	bool isTrapped ( int x, int y );
	void setTrap ( int x, int y, int trapLevel );
	void removeTrap ();
	void moveToSpace ( int x, int y );
	void checkReachableTiles(int x, int y, int remainingMoves);

   DxTexture& getTileTexture ( int rowNum, int colNum );

	// private methods
private:
	Tile& getCell ( int rowNum, int colNum );


	// private member data
private:
	Tile** m_grid;
	int m_rowNum;
	int m_colNum;
	std::vector<POINT> reachableTiles;

	// private special methods
private:

};

#endif
