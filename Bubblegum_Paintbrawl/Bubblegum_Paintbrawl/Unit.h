#pragma once

#include "stdafx.h"
#include <string>

class GridInterface
{
public:
	virtual ~GridInterface () {};

	virtual bool isSpaceEmpty ( int x, int y ) = 0;
	virtual void moveToSpace ( int x, int y ) = 0;
};

class Unit
{
	
public:
	enum Direction{ North, South, East, West};
	enum ClassType{Bruiser, Ranger, Balanced};

	Unit();
	~Unit();

   // pInterface is a pointer to the map.
	void initUnit(GridInterface* pInterface, ClassType unitClass, int allegiance, int x, int y);
	void initBruiser(int x, int y);
	void initRanger(int x, int y);
	void initBalanced(int x, int y);

   // these are called by the GameManager (i.e. the player)
	void move(Direction dir);
	void attack(Unit* enemyUnit);
   void turnStart();

	int checkAllegiance(Unit& currentUnit);
	int getHealth(){return currentHealth;}

private:
  	void takeDamage(int damage);
  	void lockUnit();

private:
	int positionX;
	int positionY;
	int maxMove;
	int currentMove;
	int maxHealth;
	int currentHealth;
	int attackDamage;
   int myAllegiance; // the value is equal to the team number
	bool locked;

   GridInterface* myGridInterface;
};
