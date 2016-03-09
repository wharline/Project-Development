
#include "stdafx.h"
#include "Unit.h"

Unit::Unit ()
{
   positionX = 0;
   positionY = 0;
   maxMove = 0;
   currentMove = 0;
   maxHealth = 0;
   currentHealth = 0;
   attackDamage = 0;
   locked = true;
   dead = false;
   myGridInterface = NULL;
}

Unit::~Unit ()
{

}
void Unit::initUnit ( GridInterface* pInterface, ClassType unitClass, int allegiance, int x, int y )
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
void Unit::initBalanced ( int x, int y )
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 10;
   currentHealth = maxHealth;
   attackDamage = 5;
}
void Unit::initBruiser ( int x, int y )
{
   maxMove = 3;
   currentMove = maxMove;
   maxHealth = 15;
   currentHealth = maxHealth;
   attackDamage = 10;
}
void Unit::initRanger ( int x, int y )
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 5;
   currentHealth = maxHealth;
   attackDamage = 5;
}
bool Unit::selectPath ( Direction dir )
{
   int y = yPosToMoveTo;
   int x = xPosToMoveTo;

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

   if ( !locked && myGridInterface->canPassThrough( x, y, myAllegiance ) )
   {
      yPosToMoveTo = y;
      xPosToMoveTo = x;
      return true;
   }
   else
   {
      return false;
   }
}

void Unit::moveTo ( int x, int y )
{
   if ( !locked && myGridInterface->isEmpty( x, y ) )
   {
      positionX = x;
      positionY = y;
      myGridInterface->moveToSpace( positionX, positionY );
      lockUnit();
   }
}

void Unit::attack ( Unit* enemyUnit )
{
   if ( !locked )
   {
      enemyUnit->takeDamage(attackDamage);
      lockUnit();
   }
}

void Unit::takeDamage ( int damage )
{
   currentHealth -= damage;

   if ( currentHealth <= 0 )
      unitDie();
}

void Unit::lockUnit ()
{
   locked = true;
}

void Unit::turnStart ()
{
   bool trapped = myGridInterface->isTrapped( positionX, positionY );
   if ( !dead && !trapped )
      locked = false;
   if ( trapped )
   {
      myGridInterface->removeTrap();
   }
}

void Unit::unitDie ()
{
   dead = true;
   locked = true;
}