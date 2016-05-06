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

   myTrapLevel = 0;
   occupiedAllegiance = -1;
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

void Tile::setState ( TileState state, int allegiance )
{
   // if tile is blocked, don't allow state change
   if ( myState == blocked )
      return;

   // if a unit is trapped on the tile, don't allow state change
   // setting state to occupied signifies that trap is gone
   if ( ( myState == occupiedTrap || myState == trapped ) && state != occupied )
      return;

   if ( myState == trapped && state == occupied )
      state = occupiedTrap;

	switch ( state )
   {
   case occupied:
   case occupiedTrap:
      occupiedAllegiance = allegiance;
      break;
   case blocked:
      occupiedAllegiance = allegiance;
      setColor( D3DCOLOR_XRGB( 0, 0, 255 ) );
      break;
   case empty:
      setColor( D3DCOLOR_XRGB( 255, 255, 255 ) );
      occupiedAllegiance = allegiance;
      break;
   case trapped:
      setColor( D3DCOLOR_XRGB( 255, 100, 100 ) );
      break;
   default:
      break;
   }

   myState = state;
}

bool Tile::canPassThrough ( int allegiance )
{
   if ( occupiedAllegiance == allegiance )
   {
      return true;
   }
   else
   {
      return ( myState == empty || myState == trapped );
   }
}

void Tile::setTrap ( int trapLevel )
{
   myTrapLevel = trapLevel;

   setState( trapped );
}

void Tile::removeTrap ()
{
	if ( myTrapLevel > 0 )
		myTrapLevel--;
	if ( myTrapLevel == 0 )
		setState( occupied );
}

bool Tile::isAccessible(int allegiance)
{
	if( ( !isOccupied() || !isBlocked() ) && canPassThrough(allegiance) )
	{
		return true;
	}
   return false;
}

bool Tile::loadTileImages ( DxTexture& emptyTile, DxTexture& filledTile )
{
   ourEmptyTile = emptyTile;
   ourFilledTile = filledTile;

   return true;
}

void Tile::setColor ( D3DCOLOR color )
{
   if ( myState == empty || myState == occupied )
   {
      if ( myColor == D3DCOLOR_XRGB( 255, 100, 100 ) )
         OutputDebugString("Changed trapped tile color.\n");

      myColor = color;
   }
}

D3DCOLOR Tile::color ()
{
   if ( selected )
   {
      return D3DCOLOR_XRGB( 100, 100, 100 );
   }
   else
   {
      return myColor;
   }
}

void Tile::toggleSelected ( bool on )
{
   if ( on )
      selected = true;
   else
      selected = false;
}