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
		empty,
      trapped
	};

public:
	Tile ();
	~Tile ();

	int x () { return myXPos; }
	int y () { return myYPos; }

	void setXPos ( int x );
	void setYPos ( int y );

	bool canPassThrough ( int allegiance );
   bool isEmpty () { return myState == empty; }
   bool isOccupied () { return myState == occupied; }
	bool isBlocked () { return myState == blocked; }
   bool isTrapped () { return myState == trapped; }
   void setTrap ( int trapLevel );
   void removeTrap ();


	void setState ( TileState state );

private:
	int myXPos, myYPos;
	TileState myState;

   int myTrapLevel; // equal to zero if not trapped

   int occupiedAllegiance; // value of team number that has a unit on this tile.  Zero if no unit is on tile.

	void* mySprite;

};

#endif