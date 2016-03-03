
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
void Unit::initUnit()
{

}
void Unit::initUnit(int x, int y, int allegiance)
{
	int positionX = x;
	int positionY = y;

	int maxMove = 5;
	int currentMove = maxMove;
	int maxHealth = 10;
	int currentHealth = maxHealth;
	int attackDamage = 1;
	bool locked = false;
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
void Unit::attack(Unit enemyUnit)
{
	enemyUnit.currentHealth -= attackDamage;
}
bool Unit::select()
{
	return 0;
}
void Unit::takeDamage(int damage)
{
	currentHealth -= damage;
}
int Unit::checkAllegiance(Unit currentUnit)
{
	return 0;
}
void Unit::lockUnit()
{
	locked = true;
}



