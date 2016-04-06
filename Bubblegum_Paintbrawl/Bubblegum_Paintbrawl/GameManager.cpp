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

   linebackerImage = LoadTexture( "../Assets/BPB_-_SpriteCharacters01/BPB - Linebacker1.png" );
   if ( !linebackerImage )
      return false;

   paintballerImage = LoadTexture( "../Assets/BPB_-_SpriteCharacters01/BPB - Paintballer1.png" );
   if ( !paintballerImage )
      return false;

   artistImage = LoadTexture( "../Assets/BPB_-_SpriteCharacters01/BPB - Artist1.png" );
   if ( !artistImage )
      return false;

   pranksterImage = LoadTexture( "../Assets/BPB_-_SpriteCharacters01/BPB - Trapper1.png" );
   if ( !pranksterImage )
      return false;

   initUnits( &player1Units, player1 );
   initUnits( &player2Units, player2 );

   return true;
}

bool GameManager::initUnits ( vector<Unit>* playersUnits, PlayerTurn player )
{
   for ( int i = 0; i < myStartLinebackerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::linebacker, player, i, myBoardSize - 3, linebackerImage );
      playersUnits->push_back( unit );
   }
   for ( int i = 0; i < myStartPaintballerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::paintballer, player, i, myBoardSize - 2, paintballerImage );
      playersUnits->push_back( unit );
   }
   for ( int i = 0; i < myStartArtistNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::artist, player, i, myBoardSize - 1, artistImage );
      playersUnits->push_back( unit );
   }
   for ( int i = 0; i < myStartPranksterNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::prankster, player, i, myBoardSize, pranksterImage );
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
   if ( SUCCEEDED( dxDevice()->BeginScene() ) )
   {
      // start drawing
      spriteInterface()->Begin( D3DXSPRITE_ALPHABLEND );

      // draw player1's units
      for ( int i = 0; i < (int)player1Units.size(); i++ )
      {
         Unit& unit = player1Units.at( i );

         Sprite_Draw_Frame( unit.texture(), unit.getXPos(), unit.getYPos(), 0, 20, 20, 1 );
      }

      // stop drawing
      spriteInterface()->End();
      dxDevice()->EndScene();
      dxDevice()->Present( NULL, NULL, NULL, NULL );
   }

   // quit game by pressing esc
   if ( keyDown( DIK_ESCAPE ) )
   {
      quitGame();
   }
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