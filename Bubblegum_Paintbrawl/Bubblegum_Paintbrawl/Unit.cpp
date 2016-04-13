
#include "stdafx.h"
#include "Unit.h"

Unit::Unit ()
{
   myClass = linebacker;

   positionX = 0;
   positionY = 0;
   xPosToMoveTo = 0;
   yPosToMoveTo = 0;
   maxMove = 0;
   currentMove = 0;
   maxHealth = 0;
   currentHealth = 0;
   attackDamage = 0;
   myAllegiance = 0;
   locked = true;
   dead = false;

   myGridInterface = NULL;
   myImage = NULL;

   // special abilities
   bool isBlocking = false;
}

Unit::~Unit ()
{
}
bool Unit::init ( GridInterface* pInterface, ClassType unitClass, int allegiance, int x, int y, DxTexture& image )
{
   bool result;

   myGridInterface = pInterface;
   myAllegiance = allegiance;

   myClass = unitClass;

   positionX = x;
   positionY = y;

   myImage = &image;

   switch ( myClass )
   {
   case linebacker:
      result = initLinebacker(x, y);
      break;
   case paintballer:
      result = initPaintballer( x, y);
      break;
   case artist:
      result = initArtist( x, y);
      break;
   case prankster:
      result = initPrankster( x, y );
      break;
   default:
      break;
   }

   myGridInterface->moveToSpace( 0, 0, positionX, positionY, myAllegiance );

   return true;
}

bool Unit::initLinebacker ( int x, int y )
{
   maxMove = 3;
   currentMove = maxMove;
   maxHealth = 15;
   currentHealth = maxHealth;
   attackDamage = 10;

   return true;
}

bool Unit::initPaintballer ( int x, int y )
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 5;
   currentHealth = maxHealth;
   attackDamage = 5;

   return true;
}

bool Unit::initArtist ( int x, int y )
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 10;
   currentHealth = maxHealth;
   attackDamage = 5;

   return true;
}

bool Unit::initPrankster ( int x, int y )
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 10;
   currentHealth = maxHealth;
   attackDamage = 5;

   return true;
}

bool Unit::selectPath ( Direction dir )
{
   int y = yPosToMoveTo;
   int x = xPosToMoveTo;

   switch(dir)
   {
   case north:
      y -= 1;
      break;
   case south:
      y += 1;
      break;
   case east:
      x += 1;
      break;
   case west:
      x -= 1;
      break;
   default:
      break;
   }

   if ( !locked && myGridInterface->canPassThrough( x, y, myAllegiance ) )
   {
      yPosToMoveTo = y;
      xPosToMoveTo = x;
      myGridInterface->spaceSelected( xPosToMoveTo, yPosToMoveTo );
      return true;
   }
   else
   {
      return false;
   }
}

bool Unit::finishMovement ()
{
   if ( !locked && myGridInterface->isEmpty( xPosToMoveTo, yPosToMoveTo ) )
   {
      int prevPosX = positionX;
      int prevPosY = positionY;

      positionX = xPosToMoveTo;
      positionY = yPosToMoveTo;

      myGridInterface->moveToSpace( prevPosX, prevPosY, positionX, positionY, myAllegiance );
      lockUnit();
      
      return true;
   }

   return false;
}

void Unit::attack ( Unit* enemyUnit )
{
   if ( !locked )
   {
      enemyUnit->takeDamage(attackDamage);
      lockUnit();
   }
}

void Unit::linebackerSpecial ()
{
   if ( !locked && myClass == linebacker )
      isBlocking = true;
}

void Unit::paintballerSpecial ( Unit* otherUnit )
{
   if ( !locked && myClass == paintballer )
   {
      if ( otherUnit->positionX > this->positionX + 2 || otherUnit->positionX < this->positionX - 2 ||
           otherUnit->positionY > this->positionY + 2 || otherUnit->positionY < this->positionY - 2 )
      {
         otherUnit->takeDamage( attackDamage );
      }
   }
}

void Unit::artistSpecial ( Unit* otherUnit )
{
   if ( !locked && myClass == artist )
   {
      if ( otherUnit->positionX > this->positionX + 1 || otherUnit->positionX < this->positionX - 1 ||
           otherUnit->positionY > this->positionY + 1 || otherUnit->positionY < this->positionY - 1 )
      {
         otherUnit->healDamage( 5 );
      }
   }
}

void Unit::pranksterSpecial ( int x, int y )
{
   if ( !locked && myClass == prankster )
   {
      if ( x > positionX + 1 || x < positionX - 1 || y > positionY + 1 || y < positionY - 1 )
      {
         myGridInterface->setTrap( x, y, 1 );
      }
   }
}

void Unit::takeDamage ( int damage )
{
   currentHealth -= damage;

   if ( currentHealth <= 0 )
      unitDie();
}

void Unit::healDamage ( int amountToHeal )
{
   if ( !dead )
   {
      currentHealth += amountToHeal;
      if ( currentHealth > maxHealth )
      {
         currentHealth = maxHealth;
      }
   }
}

void Unit::lockUnit ()
{
   locked = true;
}

void Unit::turnStart ()
{
   if ( dead )
   {
      return;
   }
   bool trapped = myGridInterface->isTrapped( positionX, positionY );
   if ( !trapped )
   {
      locked = false;
   }
   else
   {
      myGridInterface->removeTrap();
   }

   xPosToMoveTo = positionX;
   yPosToMoveTo = positionY;
}

void Unit::unitDie ()
{
   dead = true;
   locked = true;
}