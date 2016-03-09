#include "stdafx.h"
#include "Tile.h"

Tile::Tile ()
{
	mySprite = NULL;
	myState = empty;
	myXPos = myYPos = 0;
}

Tile::~Tile ()
{
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
