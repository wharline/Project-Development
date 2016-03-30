#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager ()
{
}

GameManager::~GameManager ()
{
}

bool GameManager::init ()
{
   myTurn = player1;

   return true;
}

void GameManager::update ()
{
   // iterate through all units for each player and determine if all are dead.
   // if all are dead, that player lost.
   
   // end of turn
   if ( myTurn == player1 )
      myTurn = player2;
   else
      myTurn = player1;
}

void GameManager::shutdown ()
{
}