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
	bool init ( GridInterface* pInterface, ClassType unitClass, int allegiance, int x, int y );
	
   // these are called by the GameManager (i.e. the player)
	bool selectPath ( Direction dir );
   void moveTo ( int x, int y );
	void attack ( Unit* enemyUnit );

   void linebackerSpecial ();
   void paintballerSpecial ( Unit* otherUnit );
   void artistSpecial ( Unit* otherUnit );
   void pranksterSpecial ( int x, int y );
   void turnStart ();

   int checkAllegiance () { return myAllegiance; }
	int getHealth () { return currentHealth; }

   bool isDead () { return dead; }

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
};
