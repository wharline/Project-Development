#include "stdafx.h"
#include "Tile.h"

DxTexture Tile::ourEmptyTile;
DxTexture Tile::ourFilledTile;

Tile::Tile ()
{
	mySprite = NULL;
	myState = empty;
	myXPos = myYPos = 0;

   myColor = D3DCOLOR_XRGB( 255, 255, 255 );

   myTrapLevel = occupiedAllegiance = 0;
}

Tile::~Tile ()
{
}

bool Tile::init ()
{
   mySprite = &ourEmptyTile;

   return true;
}

void Tile::setXPos ( int x )
{
	myXPos = x;
}

void Tile::setYPos ( int y )
{
	myYPos = y;
}

void Tile::setState ( TileState state )
{
	myState = state;

	switch ( myState )
   {
   case occupied:
      break;
   case blocked:
      break;
   case empty:
      break;
   case trapped:
      break;
   default:
      break;
   }
}

bool Tile::canPassThrough ( int allegiance )
{
   if ( occupiedAllegiance == allegiance )
   {
      return true;
   }
   else
   {
      return myState == empty;
   }
}

void Tile::setTrap ( int trapLevel )
{
   myTrapLevel = trapLevel;

   myState = trapped;
}

void Tile::removeTrap ()
{
	if ( myTrapLevel > 0 )
		myTrapLevel--;
	if ( myTrapLevel == 0 )
		myState = occupied;
}

bool Tile::isAccessible(int allegiance)
{
	if((!isTrapped() || !isOccupied() || !isBlocked()) && canPassThrough(allegiance))
	{
		return true;
	}
   return false;
}

bool Tile::loadTileImages ( DxTexture emptyTile, DxTexture filledTile )
{
   ourEmptyTile = emptyTile;
   ourFilledTile = filledTile;

   return true;
}

void Tile::setColor ( D3DCOLOR color )
{
   myColor = color;
}