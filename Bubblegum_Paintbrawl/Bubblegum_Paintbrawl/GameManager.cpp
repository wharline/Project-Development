#include "stdafx.h"
#include "GameManager.h"
#include <cassert>

// define keycodes for keyboard input that aren't already defined by windows
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

GameManager::GameManager ()
{
}

GameManager::~GameManager ()
{
}

bool GameManager::gameInit (int boardsize)
{
   selectedUnit = NULL;
   enemyUnit = NULL;

   fontArial24 = NULL;
   fontGaramond36 = NULL;
   fontTimesNewRoman40 = NULL;


   myTurnStart = true;

   keyPressed = false;

   selectedUnitMove = false;
   showAttackRange = false;

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
   result = LoadTexture( filledTileImage, "../Assets/BPB_-_Terrain01/BPB - FilledSpace01.png" );
   if ( !result )
      return false;

   result = LoadTexture( emptyTileImage, "../Assets/BPB_-_Terrain01/BPB - EmptySpace01.png" );
   if ( !result )
      return false;

   Tile::loadTileImages( emptyTileImage, filledTileImage );

   tileSize.x = (float)emptyTileImage.width();
   tileSize.y = (float)emptyTileImage.height();

   // load fonts
   fontArial24 = makeFont( "Arial", 24 );
   fontGaramond36 = makeFont( "Garamond", 36 );
   fontTimesNewRoman40 = makeFont( "Times New Roman", 40 );


   // initialize grid
   myBoardSize = boardsize;
   m_grid.init( myBoardSize, myBoardSize );

   // Adjust scale to screen size
   if ( winWidth() < winHeight() )
      scaleFactor = (float)winWidth() / (float)( myBoardSize * emptyTileImage.width() );
   else
      scaleFactor = (float)winHeight() / (float)( myBoardSize * emptyTileImage.height() );

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
      unit.init( &m_grid, Unit::linebacker, player2, myBoardSize - i - 1, 0, linebackerImage2 );
      player2Units.push_back( unit );
   }

   // initialize paintballers
   for ( int i = 0; i < myStartPaintballerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::paintballer, player2, myBoardSize - i - 1, 1, paintballerImage2 );
      player2Units.push_back( unit );
   }

   // initialize artists
   for ( int i = 0; i < myStartArtistNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::artist, player2, myBoardSize - i - 1, 2, artistImage2 );
      player2Units.push_back( unit );
   }

   // initialize pranksters
   for ( int i = 0; i < myStartPranksterNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::prankster, player2, myBoardSize - i - 1, 3, pranksterImage2 );
      player2Units.push_back( unit );
   }

   return true;
}

void GameManager::gameRun ()
{
   // iterate through all units for each player and determine if all are dead.
   // if all are dead, that player lost.
   if ( myTurn == player1 )
   {
      playerTurn( player1Units, player2Units );
   }
   if ( myTurn == player2 )
   {
      playerTurn( player2Units, player1Units );
   }

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

            Sprite_Draw_Frame( tileTex, r * tileTex.width(), c * tileTex.height(), scaleFactor, m_grid.tileColor( r, c ) );
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

      // draw text
      fontPrint( fontArial24, (int)(myBoardSize * scaleFactor) + 10, 50,
         "This is the Arial 24 font" );

      fontPrint( fontGaramond36, (int)(myBoardSize * scaleFactor) + 10, 100,
         "This is the Garamond 36 font", D3DCOLOR_XRGB( 255, 0, 255 ) );

      fontPrint( fontTimesNewRoman40, (int)(myBoardSize * scaleFactor) + 10, 150,
         "This is the Times New Roman 40 font", D3DCOLOR_XRGB( 0, 255, 0 ) );

      RECT rect = { (long)( myBoardSize * tileSize.x * scaleFactor + 10 ), 250, (long)( myBoardSize * tileSize.x * scaleFactor + 100 ), 700 };
      D3DCOLOR white = D3DCOLOR_XRGB( 255, 255, 255 );
      string text = "This is a long string that will be wrapped inside a rectangle.";
      fontArial24->DrawText( spriteInterface(), text.c_str(), text.length(), &rect, DT_WORDBREAK, white );

      // stop drawing
      spriteInterface()->End();
      dxDevice()->EndScene();
      dxDevice()->Present( NULL, NULL, NULL, NULL );
   }

   // quit game by pressing esc
   if ( keyDown( VK_ESCAPE ) )
   {
      quitGame();
   }
}

void GameManager::gameExit ()
{
   player1Units.clear();
   player2Units.clear();

   linebackerImage1.texture()->Release();
   paintballerImage1.texture()->Release();
   artistImage1.texture()->Release();
   pranksterImage1.texture()->Release();

   linebackerImage2.texture()->Release();
   paintballerImage2.texture()->Release();
   artistImage2.texture()->Release();
   pranksterImage2.texture()->Release();

   filledTileImage.texture()->Release();
   emptyTileImage.texture()->Release();

   if ( fontArial24 )
      fontArial24->Release();
   if ( fontGaramond36 )
      fontGaramond36->Release();
   if ( fontTimesNewRoman40 )
      fontTimesNewRoman40->Release();

   m_grid.shutdown();
}

// The full encompassment of a single players turn
void GameManager::playerTurn ( vector<Unit>& player, vector<Unit>& enemyPlayer )
{
   
   // Check if player has lost
   bool allUnitsDead = true;
   for ( int i = 0; i < (int)player.size(); i++ )
   {
      allUnitsDead = allUnitsDead && player.at( i ).isDead();
   }
   if ( allUnitsDead && !myGameIsOver )
   {
      // what to do on gameover event
      myGameIsOver = true;
      int deadPlayer = player.at( 0 ).checkAllegiance();

      if ( deadPlayer == player1 )
      {
         MessageBox( 0, "Player 1 has lost", "Game Over", 0 );
      }
      else
      {
         MessageBox( 0, "Player 2 has lost", "Game Over", 0 );
      }
      return;
   }

   // check if the player's turn is over
   bool turnOver = true;
   for ( int i = 0; i < (int)player.size(); i++ )
   {
      turnOver = turnOver && player.at( i ).isLocked();
   }
   if ( turnOver )
   {
      // turn is now over
      if ( myTurn == player1 )
      {
         myTurn = player2;
      }
      else
      {
         myTurn = player1;
      }
   }

   // unlock all units for the player at beginning of turn
   if ( myTurnStart )
   {
      for ( int i = 0; i < (int)player.size(); i++ )
      {
         player.at( i ).turnStart();
      }
   }

   // set the start of turn for the next player
   if ( turnOver )
   {
      myTurnStart = true;
   }
   else
   {
      myTurnStart = false;
   }

   //TO-DO Fill out the rest of the players turn

   // select a unit
   if ( mouseButton( 0 ) )
   {
      // a unit's attack range is not shown
      if ( !showAttackRange )
      {
         // if a unit was already selected, cancel that unit before selecting another.
         if ( selectedUnit )
         {
            selectedUnit->cancelPath();
         }
         selectedUnit = selectUnit( player );
         
         // show unit's movement range
         if ( selectedUnit )
         {
            selectedUnit->unitSelected();
         }
      }
      // a unit is selected and its attack range is being shown
      else
      {
         // select an enemy's unit to attack within attack range
         enemyUnit = selectUnit( enemyPlayer );
         if ( enemyUnit )
         {
            // check that enemy is within attack range
            bool result = m_grid.isPointInReachableTiles( enemyUnit->getGridXPos(), enemyUnit->getGridYPos() );
            if ( result )
            {
               result = selectedUnit->attack( enemyUnit );
               if ( result )
               {
                  selectedUnitMove = false;
                  showAttackRange = false;
                  selectedUnit = NULL;
               }
            }
         }
         else
         {
            showAttackRange = false;
            selectedUnitMove = false;
         }
      }
   }

   // check if I got a unit
   if ( selectedUnit )
   {
      // move when the key is pressed once, but not held
      if ( !keyPressed )
      {
         if ( keyDown( VK_W ))
         {
            selectedUnit->selectPath( Unit::north );
            keyPressed = true;
         }
         else if ( keyDown( VK_D ) )
         {
            selectedUnit->selectPath( Unit::east );
            keyPressed = true;
         }
         else if ( keyDown( VK_A ) )
         {
            selectedUnit->selectPath( Unit::west );
            keyPressed = true;
         }
         else if ( keyDown( VK_S ) )
         {
            selectedUnit->selectPath( Unit::south );
            keyPressed = true;
         }
         else if ( keyDown( VK_SPACE ) )
         {
            // move unit to potential position to see attack range
            if ( !selectedUnitMove )
            {
               bool result = selectedUnit->potentialMove();
               selectedUnitMove = showAttackRange = result;
            }
            // finish movement
            else
            {
               bool result = selectedUnit->finishMovement();
               if ( result )
               {
                  selectedUnitMove = false;
                  showAttackRange = false;
                  selectedUnit = NULL;
               }
            }

            keyPressed = true;
         }
      }
      else
      {
         // check that the keys are not being held before setting it to false
         if ( !keyDown( VK_W ) && !keyDown( VK_D ) && !keyDown( VK_A ) && !keyDown( VK_S ) && !keyDown( VK_SPACE ) )
         {
            keyPressed = false;
         }
      }

      // when space is pressed, check surrounding region for units to attack
      // range is displayed in red
      // left click on an enemy unit to attack, right click on a space to activate special
}
}

Unit* GameManager::selectUnit ( vector<Unit>& player )
{
   // get mouse position
   POINT mPos;

   mPos = mousePos();

   // convert the mouse position to a space on the grid
   float posX;
   float posY;

   posX = ( ( mPos.x / scaleFactor ) / tileSize.x );
   posY = ( ( mPos.y / scaleFactor ) / tileSize.y );

   // round y poxition (x position works without rounding)
   if ( (int)( posY * 10 ) % 10 >= 5 )
   {
      posY += 1;
   }

   // find a unit for current player at the mouse position
   for ( int i = 0; i < (int)player.size(); i++ )
   {
      if ( (int)posX == player.at( i ).getGridXPos() && (int)posY == player.at( i ).getGridYPos() )
      {
         return &player.at( i ); // seems to be returning a copy, not a reference
      }
   }

   // didn't find a unit for that player at that space
   return NULL;
}