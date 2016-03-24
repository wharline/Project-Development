#pragma once

#include "stdafx.h"
#include <string>

class GridInterface
{
public:
	virtual ~GridInterface () {};

	virtual bool canPassThrough ( int x, int y, int allegiance ) = 0;
   virtual bool isEmpty ( int x, int y ) = 0;
   virtual bool isTrapped ( int x, int y ) = 0;
   virtual void setTrap ( int x, int y, int trapLevel ) = 0;
   virtual void removeTrap () = 0;
	virtual void moveToSpace ( int x, int y ) = 0;
};

class Unit
{
	
public:
	enum Direction {  north, south, east, west };
	enum ClassType { linebacker, paintballer, artist, prankster };

	Unit ();
	~Unit ();

   // pInterface is a pointer to the map.
	bool initUnit ( GridInterface* pInterface, ClassType unitClass, int allegiance, int x, int y );
	bool initLinebacker ( int x, int y );
	bool initPaintballer ( int x, int y );
	bool initArtist ( int x, int y );
   bool initPrankster ( int x, int y );

   // these are called by the GameManager (i.e. the player)
	bool selectPath ( Direction dir );
   void moveTo ( int x, int y );
	void attack ( Unit* enemyUnit );
   void specialAbility ();
   void turnStart ();

   int checkAllegiance () { return myAllegiance; }
	int getHealth () { return currentHealth; }

private:
  	void takeDamage ( int damage );
  	void lockUnit ();
   void unitDie ();

private:
   ClassType   myClass;

	int         positionX;
   int         xPosToMoveTo;
   int         yPosToMoveTo;
	int         positionY;
	int         maxMove;
	int         currentMove;
	int         maxHealth;
	int         currentHealth;
	int         attackDamage;
   int         myAllegiance; // the value is equal to the team number
	bool        locked;
   bool        dead;

   GridInterface* myGridInterface;
};
