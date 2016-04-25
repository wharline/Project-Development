
#include "stdafx.h"
#include "Unit.h"

Unit::Unit ()
{
   myClass = linebacker;

   positionX = 0;
   positionY = 0;
   prevPosX = 0;
   prevPosY = 0;
   xPosToMoveTo = 0;
   yPosToMoveTo = 0;
   maxMove = 0;
   currentMove = 0;
   maxHealth = 0;
   currentHealth = 0;
   attackDamage = 0;
   attackRange = 0;
   specialRange = 0;
   myAllegiance = 0;
   locked = true;
   dead = false;
   isBlocking = false;

   selectedPathLength = 0;  

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

   positionX = prevPosX = x;
   positionY = prevPosY = y;

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

   myGridInterface->moveToSpace( positionX, positionY, positionX, positionY, myAllegiance );

   return true;
}

bool Unit::initLinebacker ( int x, int y )
{
   maxMove = 3;
   currentMove = maxMove;
   maxHealth = 15;
   currentHealth = maxHealth;
   attackDamage = 10;
   attackRange = 1;
   specialRange = 0;

   return true;
}

bool Unit::initPaintballer ( int x, int y )
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 5;
   currentHealth = maxHealth;
   attackDamage = 5;
   attackRange = 1;
   specialRange = 2;

   return true;
}

bool Unit::initArtist ( int x, int y )
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 10;
   currentHealth = maxHealth;
   attackDamage = 5;
   attackRange = 1;
   specialRange = 1;

   return true;
}

bool Unit::initPrankster ( int x, int y )
{
   maxMove = 5;
   currentMove = maxMove;
   maxHealth = 10;
   currentHealth = maxHealth;
   attackDamage = 5;
   attackRange = 1;
   specialRange = 1;

   return true;
}

void Unit::unitSelected ()
{
   // if not locked show the movement range
   if ( !locked )
      myGridInterface->showRange( positionX, positionY, maxMove, myAllegiance, D3DCOLOR_XRGB( 0, 255, 0 ) );
}

bool Unit::selectPath ( Direction dir )
{
   if ( selectedPathLength >= maxMove )
   {
      return false;
   }

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
      selectedPathLength++;
      currentMove--;
      myGridInterface->spaceSelected( xPosToMoveTo, yPosToMoveTo );
      return true;
   }
   else
   {
      return false;
   }
}

void Unit::cancelPath ()
{
   // reset positions to move to
   positionX = prevPosX;
   positionY = prevPosY;
   xPosToMoveTo = positionX;
   yPosToMoveTo = positionY;
   selectedPathLength = 0;
   currentMove = maxMove;

   // use this to reset tile colors.
   myGridInterface->moveToSpace( positionX, positionY, positionX, positionY, myAllegiance );
}

bool Unit::potentialMove ()
{
   if ( !locked && myGridInterface->isEmpty( positionX, positionY, xPosToMoveTo, yPosToMoveTo ) )
   {
      prevPosX = positionX;
      prevPosY = positionY;

      positionX = xPosToMoveTo;
      positionY = yPosToMoveTo;

      // show the attack range.  Therefore, pass in the enemy's allegiance
      // (if it equals zero, pass in one. If it equals one, pass in zero)
      myGridInterface->showRange( positionX, positionY, attackRange, (myAllegiance == 0 ? 1 : 0), D3DCOLOR_XRGB( 255, 0, 0 ) );

      return true;
   }

   return false;
}

bool Unit::finishMovement ()
{
   if ( !locked && myGridInterface->isEmpty( positionX, positionY, xPosToMoveTo, yPosToMoveTo ) )
   {
      myGridInterface->moveToSpace( prevPosX, prevPosY, positionX, positionY, myAllegiance );

      prevPosX = positionX;
      prevPosY = positionY;
      
      lockUnit();

      return true;
   }

   return false;
}

bool Unit::attack ( Unit* enemyUnit )
{
   if ( !locked )
   {
      enemyUnit->takeDamage(attackDamage);

      return finishMovement();
   }

   return false;
}

bool Unit::activateSpecial ()
{
   // the allegiance of tiles that are included in range
   int allegiance;

   // if it's a paintballer or prankster, choose enemy's allegiance
   if ( myClass == paintballer || myClass == prankster )
   {
      allegiance = ( myAllegiance == 0 ? 1 : 0 );
   }
   else
   {
      allegiance = myAllegiance;
   }

   if ( !locked )
   {
      myGridInterface->showRange( positionX, positionY, specialRange, allegiance, D3DCOLOR_XRGB( 100, 0, 100 ) );
      
      return true;
   }

   return false;
}

void Unit::linebackerSpecial ()
{
   if ( !locked && myClass == linebacker )
   {
      isBlocking = true;
      finishMovement();
   }
}

void Unit::paintballerSpecial ( Unit* otherUnit )
{
   if ( !locked && myClass == paintballer )
   {
      if ( otherUnit->positionX >= this->positionX + specialRange || otherUnit->positionX <= this->positionX - specialRange ||
           otherUnit->positionY >= this->positionY + specialRange || otherUnit->positionY <= this->positionY - specialRange )
      {
         otherUnit->takeDamage( attackDamage );
         finishMovement();
      }
   }
}

void Unit::artistSpecial ( Unit* otherUnit )
{
   if ( !locked && myClass == artist )
   {
      if ( otherUnit->positionX >= this->positionX + specialRange || otherUnit->positionX <= this->positionX - specialRange ||
           otherUnit->positionY >= this->positionY + specialRange || otherUnit->positionY <= this->positionY - specialRange )
      {
         otherUnit->healDamage( 5 );
         finishMovement();
      }
   }
}

void Unit::pranksterSpecial ( int x, int y )
{
   if ( !locked && myClass == prankster )
   {
      if ( x >= positionX + specialRange || x <= positionX - specialRange ||
          y >= positionY + specialRange || y <= positionY - specialRange )
      {
         myGridInterface->setTrap( x, y, 1 );
         finishMovement();
      }
   }
}

void Unit::takeDamage ( int damage )
{
   if ( isBlocking )
      return;

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

   isBlocking = false;

   xPosToMoveTo = positionX;
   yPosToMoveTo = positionY;
}

void Unit::unitDie ()
{
   dead = true;
   locked = true;
   myGridInterface->unitDied( positionX, positionY, myAllegiance );
}