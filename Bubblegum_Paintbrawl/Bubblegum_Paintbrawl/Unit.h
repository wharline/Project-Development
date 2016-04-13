#pragma once

#include "stdafx.h"
#include <string>

#include "Common.h"
#include "DxTexture.h"

class GridInterface
{
public:
	virtual ~GridInterface () {};

	virtual bool canPassThrough ( int x, int y, int allegiance ) = 0;
   virtual bool isEmpty ( int x, int y ) = 0;
   virtual bool isTrapped ( int x, int y ) = 0;
   virtual void setTrap ( int x, int y, int trapLevel ) = 0;
   virtual void removeTrap () = 0;
	virtual void moveToSpace ( int oldX, int oldY, int newX, int newY, int allegiance ) = 0;
   virtual void spaceSelected ( int x, int y ) = 0;
};

class Unit
{
	
public:
	enum Direction {  north, south, east, west };
	enum ClassType { linebacker, paintballer, artist, prankster };

	Unit ();
	~Unit ();

   // pInterface is a pointer to the map.
	bool init ( GridInterface* pInterface, ClassType unitClass, int allegiance, int x, int y, DxTexture& image );
	
   // these are called by the GameManager (i.e. the player)
	bool selectPath ( Direction dir );
   bool finishMovement ();
	void attack ( Unit* enemyUnit );

   void linebackerSpecial ();
   void paintballerSpecial ( Unit* otherUnit );
   void artistSpecial ( Unit* otherUnit );
   void pranksterSpecial ( int x, int y );
   void turnStart ();

   int checkAllegiance () { return myAllegiance; }
	int getHealth () { return currentHealth; }

   bool isDead () { return dead; }
   bool isLocked () { return locked; }

   // for drawing
   DxTexture& texture () { return *myImage; }
   int getXPos () { return positionX * myImage->width(); }
   int getYPos () { return positionY * myImage->height(); }

   // for grid logic
   int getGridXPos () { return positionX; }
   int getGridYPos () { return positionY; }

private:
   bool initLinebacker ( int x, int y );
	bool initPaintballer ( int x, int y );
	bool initArtist ( int x, int y );
   bool initPrankster ( int x, int y );

  	void takeDamage ( int damage );
   void healDamage ( int amountToHeal );
  	void lockUnit ();
   void unitDie ();

private:
   ClassType   myClass;

	int         positionX;
	int         positionY;
   int         xPosToMoveTo;
   int         yPosToMoveTo;
	int         maxMove;
	int         currentMove;
	int         maxHealth;
	int         currentHealth;
	int         attackDamage;
   int         myAllegiance; // the value is equal to the team number
	bool        locked;
   bool        dead;

   // special abilities
   bool isBlocking; //linebacker


   GridInterface* myGridInterface;

   // image
   DxTexture* myImage;
};
