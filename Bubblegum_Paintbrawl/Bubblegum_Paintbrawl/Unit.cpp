
#include "stdafx.h"
#include "Unit.h"

Unit::Unit()
{
	int positionX = NULL;
	int positionY = NULL;
	int maxMove = NULL;
	int currentMove = NULL;
	int maxHealth = NULL;
	int currentHealth = NULL;
	int attackDamage = NULL;
	bool locked = NULL;
}

Unit::~Unit()
{

}
void Unit::initUnit(ClassType unitClass, int x, int y)
{
	switch(unitClass)
	{
	case Bruiser: initBruiser(x, y); break;
	case Ranger: initRanger( x, y); break;
	case Balanced: initBalanced( x, y); break;
	default: break;
	}
}
void Unit::initBalanced(int x, int y)
{
	positionX = x;
	positionY = y;
	maxMove = 5;
	currentMove = maxMove;
	maxHealth = 10;
	currentHealth = maxHealth;
	attackDamage = 5;
	locked = true;
}
void Unit::initBruiser(int x, int y)
{
	positionX = x;
	positionY = y;
	maxMove = 3;
	currentMove = maxMove;
	maxHealth = 15;
	currentHealth = maxHealth;
	attackDamage = 10;
	locked = false;
}
void Unit::initRanger(int x, int y)
{
	positionX = x;
	positionY = y;
	maxMove = 5;
	currentMove = 5;
	maxHealth = 5;
	currentHealth = maxHealth;
	attackDamage = 5;
	locked = false;
}
void Unit::move(Direction dir)
{
	
	switch(dir)
	{
	case North: positionY -= 1; break;
	case South: positionY += 1; break;
	case East:  positionX += 1; break;
	case West:  positionX -= 1; break;
	default:            break;
	}
}
void Unit::attack(Unit* enemyUnit)
{
	enemyUnit->takeDamage(attackDamage);
}

void Unit::takeDamage(int damage)
{
	currentHealth -= damage;
}
int Unit::checkAllegiance(Unit& currentUnit)
{
	return 0;
}
void Unit::lockUnit()
{
	locked = true;
}




