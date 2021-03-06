#pragma once

#if !defined ( __Tile_H__ )
#define __Tile_H__

#include "Common.h"
#include "DxTexture.h"

class Tile
{
public:
	enum TileState
	{
		occupied,
		blocked,
		empty,
		trapped,
      occupiedTrap,
	};

public:
	Tile ();
	~Tile ();

   bool init ();

	int x () { return myXPos; }
	int y () { return myYPos; }

	void setXPos ( int x );
	void setYPos ( int y );

	bool canPassThrough ( int allegiance );
	bool isEmpty () { return myState == empty; }
	bool isOccupied () { return ( myState == occupied || myState == occupiedTrap ); }
	bool isBlocked () { return myState == blocked; }
	bool isTrapped () { return ( myState == trapped || myState == occupiedTrap ); }
	void setTrap ( int trapLevel );


	bool isAccessible(int allegiance);
	void removeTrap ();

	void setState ( TileState state, int allegiance = -1 );
   TileState getState () { return myState; }

   DxTexture& texture () { return *mySprite; }
   D3DCOLOR color ();
   void toggleSelected ( bool on );
   void toggleShowRange ( bool on );

   void setColor ( D3DCOLOR color );
   void setRangeColor ( D3DCOLOR color );

   static bool loadTileImages ( DxTexture& emptyTile, DxTexture& filledTile );

private:
	int myXPos, myYPos;
	TileState myState;

	int myTrapLevel; // equal to zero if not trapped

	int occupiedAllegiance; // value of team number that has a unit on this tile.  Zero if no unit is on tile.

	DxTexture* mySprite;
   D3DCOLOR myColor;
   D3DCOLOR rangeColor;

   bool selected;
   bool showRange;

   // tile images
   static DxTexture ourEmptyTile;
   static DxTexture ourFilledTile;

   // prevent copying
private:
   Tile ( const Tile& other );
   Tile& operator= ( const Tile& other );
};

#endif