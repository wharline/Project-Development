//-------------------------------------------------
//
// Representation of the grid, i.e. the board in 
//	the strategy game.
//
// Created by: William Harline
//
// version 1
//
//-------------------------------------------------

#pragma once
#if !defined ( __Grid_H__ )
#define __Grid_H__

#include "stdafx.h"
#include <string>
#include "Tile.h"
#include "Unit.h"

class Grid : public GridInterface
{
// public constructors/destructors/assign/copy methods
public:
	Grid ();
	~Grid ();

// public methods
public:
	// Create a new grid with the specified number of rows and columns
	void init ( int rowNum, int colNum );
	bool canPassThrough ( int x, int y, int allegiance );
   bool isEmpty ( int x, int y );
   bool isOccupied ( int x, int y );
   bool isTrapped ( int x, int y );
   void setTrap ( int x, int y, int trapLevel );
   void removeTrap ();
	void moveToSpace ( int x, int y );

// private methods
private:
	Tile* getCell ( int rowNum, int colNum );


// private member data
private:
	Tile** m_grid;
	int m_rowNum;
	int m_colNum;

// private special methods
private:

};

#endif