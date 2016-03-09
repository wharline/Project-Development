
#include "stdafx.h"
#include "Unit.h"

Unit::Unit()
{
   int positionX = 0;
   int positionY = 0;
   int maxMove = 0;
   int currentMove = 0;
   int maxHealth = 0;
   int currentHealth = 0;
   int attackDamage = 0;
   bool locked = true;
   myGridInterface = NULL;
}

Unit::~Unit()
{

}
void Unit::initUnit( GridInterface* pInterface, ClassType unitClass, int allegiance, int x, int y)
{
   myGridInterface = pInterface;
   myAllegiance = allegiance;

   positionX = x;
   positionY = y;

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
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 10;
   currentHealth = maxHealth;
   attackDamage = 5;
}
void Unit::initBruiser(int x, int y)
{
   maxMove = 3;
   currentMove = maxMove;
   maxHealth = 15;
   currentHealth = maxHealth;
   attackDamage = 10;
}
void Unit::initRanger(int x, int y)
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 5;
   currentHealth = maxHealth;
   attackDamage = 5;
}
void Unit::move(Direction dir)
{
   int y = positionY;
   int x = positionX;

   switch(dir)
   {
   case North:
      y -= 1;
      break;
   case South:
      y += 1;
      break;
   case East:
      x += 1;
      break;
   case West:
      x -= 1;
      break;
   default:
      break;
   }

   if ( !locked && myGridInterface->isSpaceEmpty( x, y ) )
   {
      positionY = y;
      positionX = x;
      myGridInterface->moveToSpace( positionX, positionY );
      lockUnit();
   }
}
void Unit::attack(Unit* enemyUnit)
{
   if ( !locked )
   {
      enemyUnit->takeDamage(attackDamage);
      lockUnit();
   }
}

void Unit::takeDamage(int damage)
{
   currentHealth -= damage;
}
int Unit::checkAllegiance(Unit& currentUnit)
{
   return myAllegiance;
}
void Unit::lockUnit()
{
   locked = true;
}

void Unit::turnStart ()
{
   locked = false;
}