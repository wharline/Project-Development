#pragma once

#if !defined ( __Tile_H__ )
#define __Tile_H__

#include "stdafx.h"

class Tile
{
public:
	enum TileState
	{
		occupied,
		blocked,
		empty
	};

public:
	Tile ();
	~Tile ();

	int x () { return myXPos; }
	int y () { return myYPos; }

	void setXPos ( int x );
	void setYPos ( int y );

	bool isOccupied () { return myState == occupied; }
	bool isBlocked () { return myState == blocked; }
	bool isEmpty () { return myState == empty; }

	void setState ( TileState state );

private:
	int myXPos, myYPos;
	TileState myState;

	void* mySprite;

};

#endif