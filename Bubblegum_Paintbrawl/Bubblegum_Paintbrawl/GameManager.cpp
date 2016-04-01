#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager ()
{
}

GameManager::~GameManager ()
{
}

bool GameManager::gameInit ()
{
   myTurn = player1;

   myGameIsOver = false;

   // initialize grid
   myBoardSize = 16;
   m_grid.init( myBoardSize, myBoardSize );

   // initialize units
   myStartLinebackerNum = 1;
   myStartPaintballerNum = 1;
   myStartArtistNum = 1;
   myStartPranksterNum = 1;

   initUnits( &player1Units, player1 );
   initUnits( &player2Units, player2 );

   return true;
}

bool GameManager::initUnits ( vector<Unit>* playersUnits, PlayerTurn player )
{
   for ( int i = 0; i < myStartLinebackerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::linebacker, player, i, myBoardSize - 3 );
      playersUnits->push_back( unit );
   }
   for ( int i = 0; i < myStartPaintballerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::paintballer, player, i, myBoardSize - 2);
      playersUnits->push_back( unit );
   }
   for ( int i = 0; i < myStartArtistNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::artist, player, i, myBoardSize - 1 );
      playersUnits->push_back( unit );
   }
   for ( int i = 0; i < myStartPranksterNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::prankster, player, i, myBoardSize );
      playersUnits->push_back( unit );
   }

   return true;
}

void GameManager::gameRun ()
{
   // iterate through all units for each player and determine if all are dead.
   // if all are dead, that player lost.
   playerTurn( player1Units );
   playerTurn( player2Units );

   // render
}

void GameManager::gameExit ()
{
   m_grid.shutdown();
}

void GameManager::playerTurn ( vector<Unit> player )
{
   // check if player has lost
   for ( int i = 0; i < (int)player.size(); i++ )
   {
      if ( player.at( i ).isDead() )
      {
         myGameIsOver = true;
      }
   }
}