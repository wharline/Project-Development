#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager ()
{
}

GameManager::~GameManager ()
{
}

bool GameManager::gameInit (int boardsize)
{
   myTurn = player1;

   myGameIsOver = false;

   bool result;

   // load player1's sprite images
   result = LoadTexture( linebackerImage1, "../Assets/BPB_-_SpriteCharacters01/BPB - Linebacker1.png" );
   if ( !result )
      return false;

   result = LoadTexture( paintballerImage1, "../Assets/BPB_-_SpriteCharacters01/BPB - Paintballer1.png" );
   if ( !result )
      return false;

   result = LoadTexture( artistImage1, "../Assets/BPB_-_SpriteCharacters01/BPB - Artist1.png" );
   if ( !result )
      return false;

   result = LoadTexture( pranksterImage1, "../Assets/BPB_-_SpriteCharacters01/BPB - Trapper1.png" );
   if ( !result )
      return false;

   
   // load player2's sprite images
   result = LoadTexture( linebackerImage2, "../Assets/BPB_-_SpriteCharacters02/BPB - Linebacker2.png" );
   if ( !result )
      return false;

   result = LoadTexture( paintballerImage2, "../Assets/BPB_-_SpriteCharacters02/BPB - Paintballer2.png" );
   if ( !result )
      return false;

   result = LoadTexture( artistImage2, "../Assets/BPB_-_SpriteCharacters02/BPB - Artist2.png" );
   if ( !result )
      return false;

   result = LoadTexture( pranksterImage2, "../Assets/BPB_-_SpriteCharacters02/BPB - Trapper2.png" );
   if ( !result )
      return false;


   // load the tile images
   DxTexture filledTile;
   DxTexture emptyTile;

   result = LoadTexture( filledTile, "../Assets/BPB_-_Terrain01/BPB - FilledSpace01.png" );
   if ( !result )
      return false;

   result = LoadTexture( emptyTile, "../Assets/BPB_-_Terrain01/BPB - EmptySpace01.png" );
   if ( !result )
      return false;

   Tile::loadTileImages( emptyTile, filledTile );


   // initialize grid
   myBoardSize = boardsize;
   m_grid.init( myBoardSize, myBoardSize );

   // Adjust scale to screen size
   if ( winWidth() < winHeight() )
      scaleFactor = (float)winWidth() / (float)( myBoardSize * emptyTile.width() );
   else
      scaleFactor = (float)winHeight() / (float)( myBoardSize * emptyTile.height() );

   // initialize units
   myStartLinebackerNum = 1;
   myStartPaintballerNum = 1;
   myStartArtistNum = 1;
   myStartPranksterNum = 1;

   // initialize each player's units
   initPlayers( &player1Units, player1 );
   initPlayers( &player2Units, player2 );

   return true;
}

bool GameManager::initPlayers ( vector<Unit>* playersUnits, PlayerTurn player )
{
   if ( player == player1 )
      initPlayer1();

   if ( player == player2 )
      initPlayer2();

   return true;
}

bool GameManager::initPlayer1 ()
{
   // since the y position is an index starting at 0,
   //    the variable myBoardSize must be subtracted 1 more than the desired position

   // initialize linebackers
   for ( int i = 0; i < myStartLinebackerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::linebacker, player1, i, myBoardSize - 3 - 1, linebackerImage1 );
      player1Units.push_back( unit );
   }

   // initialize paintballers
   for ( int i = 0; i < myStartPaintballerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::paintballer, player1, i, myBoardSize - 2 - 1, paintballerImage1 );
      player1Units.push_back( unit );
   }

   // initialize artists
   for ( int i = 0; i < myStartArtistNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::artist, player1, i, myBoardSize - 1 - 1, artistImage1 );
      player1Units.push_back( unit );
   }

   // initialize pranksters
   for ( int i = 0; i < myStartPranksterNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::prankster, player1, i, myBoardSize - 1, pranksterImage1 );
      player1Units.push_back( unit );
   }

   return true;
}

bool GameManager::initPlayer2 ()
{
   // since the y position is an index starting at 0,
   //    the variable myBoardSize must be subtracted 1 more than the desired position

   // initialize linebackers
   for ( int i = 0; i < myStartLinebackerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::linebacker, player1, myBoardSize - i - 1, 0, linebackerImage2 );
      player2Units.push_back( unit );
   }

   // initialize paintballers
   for ( int i = 0; i < myStartPaintballerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::paintballer, player1, myBoardSize - i - 1, 1, paintballerImage2 );
      player2Units.push_back( unit );
   }

   // initialize artists
   for ( int i = 0; i < myStartArtistNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::artist, player1, myBoardSize - i - 1, 2, artistImage2 );
      player2Units.push_back( unit );
   }

   // initialize pranksters
   for ( int i = 0; i < myStartPranksterNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::prankster, player1, myBoardSize - i - 1, 3, pranksterImage2 );
      player2Units.push_back( unit );
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

      // draw the tiles (this must be the first thing drawn, otherwise the tiles will appear on top of other sprites)
      for ( int r = 0; r < myBoardSize; r++ )
      {
         for ( int c = 0; c < myBoardSize; c++ )
         {
            DxTexture& tileTex = m_grid.getTileTexture( r, c );

            Sprite_Draw_Frame( tileTex, r * tileTex.width(), c * tileTex.height(), scaleFactor );
         }
      }

      // draw player1's units
      for ( int i = 0; i < (int)player1Units.size(); i++ )
      {
         Unit& unit = player1Units.at( i );

         Sprite_Draw_Frame( unit.texture(), unit.getXPos(), unit.getYPos(), scaleFactor );
      }

      // draw player2's units
      for ( int i = 0; i < (int)player2Units.size(); i++ )
      {
         Unit& unit = player2Units.at( i );

         Sprite_Draw_Frame( unit.texture(), unit.getXPos(), unit.getYPos(), scaleFactor );
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

// The full encompassment of a single players turn
void GameManager::playerTurn ( vector<Unit> player )
{
   
   // Check if player has lost
   for ( int i = 0; i < (int)player.size(); i++ )
   {
      if ( player.at( i ).isDead() )
      {
         myGameIsOver = true;
      }
   }

   //TO-DO Fill out the rest of the players turn
}