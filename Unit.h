#pragma once

#include "stdafx.h"
#include <string>

class Unit
{
	enum Direction{ North, South, East, West};
public:
	Unit();
	~Unit();
	void initUnit();
	void initUnit(int x, int y, int allegiance);
	void move(Direction dir);
	void attack(Unit enemyUnit);
	bool select();
	void takeDamage(int damage);
	int checkAllegiance(Unit currentUnit);
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