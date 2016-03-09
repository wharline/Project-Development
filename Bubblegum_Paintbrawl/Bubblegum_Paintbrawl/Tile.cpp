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