#pragma once

#include "stdafx.h"
#include <string>

class Unit
{
	
public:
	enum Direction{ North, South, East, West};
	enum ClassType{Bruiser, Ranger, Balanced};

	Unit();
	~Unit();

	void initUnit(ClassType unitClass, int x, int y);
	void initBruiser(int x, int y);
	void initRanger(int x, int y);
	void initBalanced(int x, int y);

	void move(Direction dir);
	void attack(Unit* enemyUnit);
	void takeDamage(int damage);
	int checkAllegiance(Unit& currentUnit);
	int getHealth(){return currentHealth;}
	void lockUnit();

private:
	int positionX;
	int positionY;
	int maxMove;
	int currentMove;
	int maxHealth;
	int currentHealth;
	int attackDamage;
	bool locked;
};
