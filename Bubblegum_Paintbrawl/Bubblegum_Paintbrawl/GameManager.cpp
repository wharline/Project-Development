#include "stdafx.h"
#include "GameManager.h"
#include <cassert>
#include <string>
#include <Commdlg.h>
#include <iostream>
#include <fstream>

using namespace std;

// define keycodes for keyboard input that aren't already defined by windows
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

GameManager::GameManager ()
{
   initialized = false;
}

GameManager::~GameManager ()
{
}

bool GameManager::gamePreInit ()
{
   bool result;

   // load player1's sprite images
   result = loadTexture( linebackerImage1, "../Assets/BPB_-_SpriteCharacters01/BPB - Linebacker1.png" );
   if ( !result )
      return false;

   result = loadTexture( paintballerImage1, "../Assets/BPB_-_SpriteCharacters01/BPB - Paintballer1.png" );
   if ( !result )
      return false;

   result = loadTexture( artistImage1, "../Assets/BPB_-_SpriteCharacters01/BPB - Artist1.png" );
   if ( !result )
      return false;

   result = loadTexture( pranksterImage1, "../Assets/BPB_-_SpriteCharacters01/BPB - Trapper1.png" );
   if ( !result )
      return false;


   // load player2's sprite images
   result = loadTexture( linebackerImage2, "../Assets/BPB_-_SpriteCharacters02/BPB - Linebacker2.png" );
   if ( !result )
      return false;

   result = loadTexture( paintballerImage2, "../Assets/BPB_-_SpriteCharacters02/BPB - Paintballer2.png" );
   if ( !result )
      return false;

   result = loadTexture( artistImage2, "../Assets/BPB_-_SpriteCharacters02/BPB - Artist2.png" );
   if ( !result )
      return false;

   result = loadTexture( pranksterImage2, "../Assets/BPB_-_SpriteCharacters02/BPB - Trapper2.png" );
   if ( !result )
      return false;


   // load the tile images
   result = loadTexture( filledTileImage, "../Assets/BPB_-_Terrain01/BPB - FilledSpace01.png" );
   if ( !result )
      return false;

   result = loadTexture( emptyTileImage, "../Assets/BPB_-_Terrain01/BPB - EmptySpace01.png" );
   if ( !result )
      return false;

   Tile::loadTileImages( emptyTileImage, filledTileImage );

   tileSize.x = (float)emptyTileImage.width();
   tileSize.y = (float)emptyTileImage.height();

   // load splash screen
   result = loadTexture( splashScreen, "../Assets/Splash_Screen/splatter.png" );
   if ( !result )
      return false;

   // load fonts
   fontArial24 = makeFont( "Arial", 24 );

   return true;
}

bool GameManager::gameInit (int boardsize)
{
   selectedUnit = NULL;
   enemyUnit = NULL;

   
   myTurnStart = true;

   keyPressed = false;

   selectedUnitMove = false;
   showAttackRange = false;
   showSpecialRange = false;

   winningPlayer = -1;
   numOfTurnsTaken = 0;

   myTurn = player1;

   myGameIsOver = false;

   // initialize grid
   //myBoardSize = boardsize;
   //m_grid.init( myBoardSize, myBoardSize );

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
   //initPlayers( &player1Units, player1 );
   //initPlayers( &player2Units, player2 );

   initialized = true;

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
      unit.init( &m_grid, Unit::linebacker, player1, i, myBoardSize - 3 - 1, 15, linebackerImage1 );
      player1Units.push_back( unit );
   }

   // initialize paintballers
   for ( int i = 0; i < myStartPaintballerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::paintballer, player1, i, myBoardSize - 2 - 1, 5, paintballerImage1 );
      player1Units.push_back( unit );
   }

   // initialize artists
   for ( int i = 0; i < myStartArtistNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::artist, player1, i, myBoardSize - 1 - 1, 10, artistImage1 );
      player1Units.push_back( unit );
   }

   // initialize pranksters
   for ( int i = 0; i < myStartPranksterNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::prankster, player1, i, myBoardSize - 1, 10, pranksterImage1 );
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
      unit.init( &m_grid, Unit::linebacker, player2, myBoardSize - i - 1, 0, 15, linebackerImage2 );
      player2Units.push_back( unit );
   }

   // initialize paintballers
   for ( int i = 0; i < myStartPaintballerNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::paintballer, player2, myBoardSize - i - 1, 1, 5, paintballerImage2 );
      player2Units.push_back( unit );
   }

   // initialize artists
   for ( int i = 0; i < myStartArtistNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::artist, player2, myBoardSize - i - 1, 2, 10, artistImage2 );
      player2Units.push_back( unit );
   }

   // initialize pranksters
   for ( int i = 0; i < myStartPranksterNum; i++ )
   {
      Unit unit;
      unit.init( &m_grid, Unit::prankster, player2, myBoardSize - i - 1, 3, 10, pranksterImage2 );
      player2Units.push_back( unit );
   }

   return true;
}

void GameManager::preInitGameRun ()
{
   HRESULT hr = dxDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 240, 250, 250 ), 1.0f, 0 );
   if ( hr != D3D_OK )
   {
      OutputDebugString( "Clear didn't work.\n" );
   }


   // render
   if ( SUCCEEDED( dxDevice()->BeginScene() ) )
   {
      // start drawing
      spriteInterface()->Begin( D3DXSPRITE_ALPHABLEND );

      // draw splash screen
      spriteDraw( splashScreen, 0, 0, ( (float)winWidth() / (float)splashScreen.width() ), D3DXVECTOR2( 0, 0 ) );

      float scale = 3.0f;

      // draw units for splash screen
      spriteDraw( linebackerImage1, 0, (int)( winHeight() - ( linebackerImage1.height() * scale ) ), scale, D3DXVECTOR2( 0, winHeight() - ( linebackerImage1.height() * scale ) ) );
      spriteDraw( paintballerImage1, linebackerImage1.width() * 2, (int)( winHeight() - ( linebackerImage1.height() * scale ) ), scale, D3DXVECTOR2( 0, winHeight() - ( linebackerImage1.height() * scale ) ) );
      spriteDraw( artistImage1, linebackerImage1.width() * 4, (int)( winHeight() - ( linebackerImage1.height() * scale ) ), scale, D3DXVECTOR2( 0, winHeight() - ( linebackerImage1.height() * scale ) ) );
      spriteDraw( pranksterImage1, linebackerImage1.width() * 6, (int)( winHeight() - ( linebackerImage1.height() * scale ) ), scale, D3DXVECTOR2( 0, winHeight() - ( linebackerImage1.height() * scale ) ) );

      spriteDraw( linebackerImage2, (int)( winWidth() - ( linebackerImage2.width() * scale ) ), (int)( winHeight() - ( linebackerImage1.height() * scale ) ), scale, D3DXVECTOR2( winWidth() - ( linebackerImage2.width() * scale ), winHeight() - ( linebackerImage1.height() * scale ) ) );
      spriteDraw( paintballerImage2, (int)( winWidth() - ( ( linebackerImage2.width() * scale ) + linebackerImage2.width() * 2 ) ), (int)( winHeight() - ( linebackerImage1.height() * scale ) ), scale, D3DXVECTOR2( winWidth() - ( linebackerImage2.width() * scale ), winHeight() - ( linebackerImage1.height() * scale ) ) );
      spriteDraw( artistImage2, (int)( winWidth() - ( ( linebackerImage2.width() * scale ) + linebackerImage2.width() * 4 ) ), (int)( winHeight() - ( linebackerImage1.height() * scale ) ), scale, D3DXVECTOR2( winWidth() - ( linebackerImage2.width() * scale ), winHeight() - ( linebackerImage1.height() * scale ) ) );
      spriteDraw( pranksterImage2, (int)( winWidth() - ( ( linebackerImage2.width() * scale ) + linebackerImage2.width() * 6 ) ), (int)( winHeight() - ( linebackerImage1.height() * scale ) ), scale, D3DXVECTOR2( winWidth() - ( linebackerImage2.width() * scale ), winHeight() - ( linebackerImage1.height() * scale ) ) );

      // stop drawing
      hr = spriteInterface()->End();
      hr = dxDevice()->EndScene();
      dxDevice()->Present( NULL, NULL, NULL, NULL );
   }

   // quit game by pressing esc
   if ( keyDown( VK_ESCAPE ) )
   {
      quitGame();
   }
}

void GameManager::gameRun ()
{
   // if game has not been initialized, draw splash screen and exit out of gameRun()
   if ( !initialized )
   {
      preInitGameRun();
      return;
   }

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

   HRESULT hr = dxDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
   if ( hr != D3D_OK )
   {
      OutputDebugString( "Clear didn't work.\n" );
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

            spriteDraw( tileTex, r * tileTex.width(), c * tileTex.height(), scaleFactor, D3DXVECTOR2( 0, 0 ), m_grid.tileColor( r, c ) );
         }
      }

      // draw player1's units
      for ( int i = 0; i < (int)player1Units.size(); i++ )
      {
         Unit& unit = player1Units.at( i );

         spriteDraw( unit.texture(), unit.getXPos(), unit.getYPos(), scaleFactor, D3DXVECTOR2( 0, 0 ) );
      }

      // draw player2's units
      for ( int i = 0; i < (int)player2Units.size(); i++ )
      {
         Unit& unit = player2Units.at( i );

         spriteDraw( unit.texture(), unit.getXPos(), unit.getYPos(), scaleFactor, D3DXVECTOR2( 0, 0 ) );
      }

      // show the side menu
      switch ( myTurn )
      {
      case player1:
         displaySidebar( player1Units );
         break;
      case player2:
         displaySidebar( player2Units );
         break;
      default:
         break;
      }

      // stop drawing
      hr = spriteInterface()->End();
      hr = dxDevice()->EndScene();
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
   // clear all units and release all images and fonts
   player1Units.clear();
   player2Units.clear();

   m_grid.shutdown();
}

void GameManager::gameShutdown ()
{
   releaseTexture( linebackerImage1.texture() );
   releaseTexture( paintballerImage1.texture() );
   releaseTexture( artistImage1.texture() );
   releaseTexture( pranksterImage1.texture() );

   releaseTexture( linebackerImage2.texture() );
   releaseTexture( paintballerImage2.texture() );
   releaseTexture( artistImage2.texture() );
   releaseTexture( pranksterImage2.texture() );

   releaseTexture( filledTileImage.texture() );
   releaseTexture( emptyTileImage.texture() );

   releaseFont( fontArial24 );
}

void GameManager::releaseTexture ( LPDIRECT3DTEXTURE9& texture )
{
   if ( texture )
   {
      texture->Release();
   }
}

void GameManager::releaseFont ( LPD3DXFONT& font )
{
   if ( font )
   {
      font->Release();
   }
}

// The full encompassment of a single players turn
void GameManager::playerTurn ( vector<Unit>& player, vector<Unit>& enemyPlayer )
{

   // don't allow a player to take a turn if the game is over
   if ( myGameIsOver )
      return;

   if ( player.empty() )
      return;

   // Check if player has lost
   bool allUnitsDead = true;
   for ( int i = 0; i < (int)player.size(); i++ )
   {
      allUnitsDead = allUnitsDead && player.at( i ).isDead();
   }
   if ( ( allUnitsDead && !myGameIsOver ) || ( numOfTurnsTaken >= 10 ) )
   {
      // what to do on gameover event
      myGameIsOver = true;
      int deadPlayer = player.at( 0 ).checkAllegiance();

      if ( numOfTurnsTaken >= 10 )
      {
         winningPlayer = none;
      }
      else if ( deadPlayer == player1 )
      {
         winningPlayer = player2;
      }
      else
      {
         winningPlayer = player1;
      }
      return;
   }

   endTurn();

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
      else if ( showAttackRange && !showSpecialRange )
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
                  numOfTurnsTaken = 0;
               }
            }
         }
         // didn't click on an enemy unit, so cancel attack and movement
         else
         {
            showAttackRange = false;
            selectedUnitMove = false;
         }
      }
      // special ability was activated.
      else if ( showAttackRange && showSpecialRange )
      {
         Unit::ClassType unitClass;
         unitClass = selectedUnit->getClassType();

         Unit* otherUnit;
         bool result = false;

         // determine unit's class and activate that class's special ability
         switch ( unitClass )
         {
         case Unit::linebacker:
            otherUnit = selectUnit( player );
            if ( !otherUnit )
            {
               // reset movement and attack
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               otherUnit = NULL;

               break;
            }
            result = m_grid.isPointInReachableTiles( otherUnit->getGridXPos(), otherUnit->getGridYPos() );
            if ( result )
            {
               // initiate the linebacker's special move and deselect him.
               selectedUnit->linebackerSpecial();

               // deselect units
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               selectedUnit = NULL;
               otherUnit = NULL;
            }
            else
            {
               // reset movement and attack
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               otherUnit = NULL;

            }
            break;
         case Unit::paintballer:
            otherUnit = selectUnit( enemyPlayer );
            if ( !otherUnit )
            {
               // no enemy unit was selected, so reset movement and attack for paintballer unit
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               otherUnit = NULL;

               break;
            }
            result = m_grid.isPointInReachableTiles( otherUnit->getGridXPos(), otherUnit->getGridYPos() );
            if ( result )
            {
               selectedUnit->paintballerSpecial( otherUnit );

               // special was successful so deselect units
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               selectedUnit = NULL;
               otherUnit = NULL;
            }
            else
            {
               // reset movement and attack
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               otherUnit = NULL;
            }
            break;
         case Unit::artist:
            otherUnit = selectUnit( player );
            if ( !otherUnit )
            {
               // no unit to heal so reset movement and attack
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               otherUnit = NULL;

               break;
            }
            result = m_grid.isPointInReachableTiles( otherUnit->getGridXPos(), otherUnit->getGridYPos() );
            if ( result )
            {
               selectedUnit->artistSpecial( otherUnit );

               // healing was successful so deselect units
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               selectedUnit = NULL;
               otherUnit = NULL;
            }
            else
            {
               // reset movement and attack
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               otherUnit = NULL;
            }
            break;
         case Unit::prankster:
            POINT space;
            space = selectSpace();
            result = m_grid.isPointInReachableTiles( space.x, space.y );
            if ( result )
            {
               selectedUnit->pranksterSpecial( space.x, space.y );

               // tile was trapped so deselect units
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               selectedUnit = NULL;
               otherUnit = NULL;
            }
            else
            {
               // reset movement and attack
               selectedUnitMove = false;
               showAttackRange = false;
               showSpecialRange = false;
               otherUnit = NULL;
            }
            break;
         default:
            break;
         }
      }
   }

   // check if I got a unit
   if ( selectedUnit )
   {
      // move when the key is pressed once, but not held
      if ( !keyPressed )
      {
         if ( keyDown( VK_W ) && !selectedUnitMove )
         {
            selectedUnit->selectPath( Unit::north );
            keyPressed = true;
         }
         else if ( keyDown( VK_D ) && !selectedUnitMove )
         {
            selectedUnit->selectPath( Unit::east );
            keyPressed = true;
         }
         else if ( keyDown( VK_A ) && !selectedUnitMove )
         {
            selectedUnit->selectPath( Unit::west );
            keyPressed = true;
         }
         else if ( keyDown( VK_S ) )
         {
            if ( !selectedUnitMove )
            {
               selectedUnit->selectPath( Unit::south );
               keyPressed = true;
            }
            // if unit has started movement, then use S key to activate special ability
            else
            {
               bool result = selectedUnit->activateSpecial();
               showSpecialRange = result;

            }

         }
         else if ( keyDown( VK_SPACE ) )
         {
            // move unit to potential position to see attack range
            if ( !selectedUnitMove )
            {
               bool result = selectedUnit->potentialMove();
               selectedUnitMove = result;
               showAttackRange = result;
            }
            // finish movement
            else
            {
               bool result = selectedUnit->finishMovement();
               if ( result )
               {
                  selectedUnitMove = false;
                  showAttackRange = false;
                  showSpecialRange = false;
                  selectedUnit = NULL;
               }
            }

            keyPressed = true;
         }
      }
      else
      {
         // check that the keys are not being held before setting whether the key is pressed to false
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

bool GameManager::endTurn ( bool force )
{
   // get the player whose turn it is
   vector<Unit>* player = NULL;

   if ( myTurn == player1 )
   {
      player = &player1Units;
   }
   else
   {
      player = &player2Units;
   }


   // check if the player's turn is over if it's not the start of the turn
   bool turnOver = true;
   if ( myTurnStart )
   {
      turnOver = false;
   }
   for ( int i = 0; i < (int)player->size(); i++ )
   {
      // if forcing the turn to end, use finishMovement() to lock all units for that player
      if ( force )
      {
         player->at( i ).finishMovement();
      }

      turnOver &= player->at( i ).isLocked();
   }

   // if the turn is over, switch turns
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
         numOfTurnsTaken++;
      }
   }

   // unlock all units for the player at beginning of turn
   if ( myTurnStart )
   {
      for ( int i = 0; i < (int)player->size(); i++ )
      {
         player->at( i ).turnStart();
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

   return true;
}

Unit* GameManager::selectUnit ( vector<Unit>& player )
{
   // get mouse position
   POINT mPos;

   mPos = mousePos();

   // convert the mouse position to a space on the grid
   float posX;
   float posY;

   posX = (float)mPos.x / scaleFactor;
   posY = (float)mPos.y / scaleFactor;

   posX /= tileSize.x;
   posY /= tileSize.y - 1;

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
         return &player.at( i );
      }
   }

   // didn't find a unit for that player at that space
   return NULL;
}

POINT GameManager::selectSpace ()
{
   // get mouse position
   POINT mPos;

   mPos = mousePos();

   // convert the mouse position to a space on the grid
   float posX;
   float posY;

   posX = (float)mPos.x / scaleFactor;
   posY = (float)mPos.y / scaleFactor;

   posX /= tileSize.x;
   posY /= tileSize.y - 1;

   // round y poxition (x position works without rounding)
   if ( (int)( posY * 10 ) % 10 >= 5 )
   {
      posY += 1;
   }

   POINT space;

   space.x = (int)posX;
   space.y = (int)posY;

   return space;
}

void GameManager::displaySidebar ( vector<Unit>& player )
{
   // set boundary of area to display
   RECT displayRect = { (long)( myBoardSize * tileSize.x * scaleFactor + 10 ), 10, (long)( winWidth() - 10 ), winHeight() - 10 };
   string displayText = "";
   float boardWidth = myBoardSize * tileSize.x * scaleFactor;


   // get unit's name
   Unit::ClassType classType = Unit::none;

   if ( selectedUnit && winningPlayer < 0 )
   {
      classType = selectedUnit->getClassType();

      switch ( classType )
      {
      case Unit::linebacker:
         displayText += "Linebacker\n";
         break;
      case Unit::paintballer:
         displayText += "Paintballer\n";
         break;
      case Unit::artist:
         displayText += "Artist\n";
         break;
      case Unit::prankster:
         displayText += "Prankster\n";
         break;
      default:
         break;
      }

      // skip down past picture
      displayText += "\n\n\n\n\n\n\n\n\n\n";
      // add in health
      char n[256];
      sprintf_s( n, "%d\n\n", selectedUnit->getHealth() );
      displayText += "Health: ";
      displayText += n;

      // add in attack power
      sprintf_s( n, "%d    ", selectedUnit->getAttackPower() );
      displayText += "Atk: ";
      displayText += n;

      // add in movement range
      sprintf_s( n, "%d\n\n", selectedUnit->getMoveRange() );
      displayText += "Mov: ";
      displayText += n;

      // add in special ability name and description
      displayText += "Special: ";

      switch ( classType )
      {
      case Unit::linebacker:
         displayText += "Bubble Block\n---------------------\n";
         displayText += "Negate the next incoming attack.\n\n";
         break;
      case Unit::paintballer:
         displayText += "Paint Bomb\n-------------------\n";
         displayText += "Ranged attack that can cross occupied spaces.\n\n";
         break;
      case Unit::artist:
         displayText += "Fresh Paint\n--------------------\n";
         displayText += "Return health to self or allies.\n\n";
         break;
      case Unit::prankster:
         displayText += "Gum Trap\n-----------------\n";
         displayText += "Convert a space into a trap that locks the next enemy to cross it.\n\n";
         break;
      default:
         break;
      }



      // display unit's picture
      float scale = ( ( winWidth() - boardWidth )/4 * 3 ) / selectedUnit->texture().width();
      int destx = (int)( boardWidth + ( winWidth() - boardWidth )/2 );
      int desty = (int)( scale * selectedUnit->texture().height() ) / 3 * 2;


      D3DXVECTOR2 center( (float)( selectedUnit->texture().width() )/2 + destx,
         (float)( selectedUnit->texture().height() )/2 + desty );

      spriteDraw( selectedUnit->texture(), destx, desty, scale, center );

   }
   else if ( winningPlayer < 0 )
   {
      displayText += "PLAYER TURN\n-------------------\n\n\n";
   }
   else
   {
      switch ( winningPlayer )
      {
      case player1:
         displayText += "Player 1 Wins!!!\n-------------------\n\n\n";
         break;
      case player2:
         displayText += "Player 2 Wins!!!\n-------------------\n\n\n";
         break;
      default:
         displayText += "No Winners...\n-------------------\n\n\n";
         break;
      }
   }

   // add in team name and remaining units
   int destx;
   int desty;

   int xOffset = (int)( ( winWidth() - boardWidth ) / 3 );
   int yOffset = (int)( tileSize.y * 2 * 2 );
   int offset = 0;

   destx = (int)( ( ( winWidth() - boardWidth ) / 3 ) + boardWidth - ( linebackerImage1.width() * 2 ) );
   desty = (int)( winHeight() - ( tileSize.y * 2 * 4 ) );

   D3DXVECTOR2 center( (float)( linebackerImage1.width() )/2 + destx,
      (float)( linebackerImage1.height() )/2 + desty );

   switch ( classType )
   {
   case Unit::linebacker:
      offset += 5;
      break;
   case Unit::paintballer:
      offset += 5;
      break;
   case Unit::artist:
      break;
   case Unit::prankster:
      offset += 15;
      break;
   default:
      offset -= ( winHeight() / 4 );
      break;
   }

   int numOfLinebackers = 0;
   int numOfPaintballers = 0;
   int numOfArtists = 0;
   int numOfPranksters = 0;

   char n[256];

   switch ( myTurn )
   {
   case player1:
      displayText += "BLUE TEAM\n---------\nUnits Remaining\n\n";

      spriteDraw( linebackerImage1, destx, desty + offset, 2, center );

      destx += xOffset;
      center.x += xOffset;

      spriteDraw( paintballerImage1, destx, desty + offset, 2, center );

      destx -= xOffset;
      center.x -= xOffset;

      desty += yOffset;
      center.y += yOffset;

      spriteDraw( artistImage1, destx, desty + offset, 2, center );

      destx += xOffset;
      center.x += xOffset;

      spriteDraw( pranksterImage1, destx, desty + offset, 2, center );


      break;
   case player2:
      displayText += "ORANGE TEAM\n---------\nUnits Remaining\n\n";

      spriteDraw( linebackerImage2, destx, desty + offset, 2, center );

      destx += xOffset;
      center.x += xOffset;

      spriteDraw( paintballerImage2, destx, desty + offset, 2, center );

      destx -= xOffset;
      center.x -= xOffset;

      desty += yOffset;
      center.y += yOffset;

      spriteDraw( artistImage2, destx, desty + offset, 2, center );

      destx += xOffset;
      center.x += xOffset;

      spriteDraw( pranksterImage2, destx, desty + offset, 2, center );

      break;
   default:
      break;
   }

   for ( int i = 0; i < (int)player.size(); i++ )
   {
      if ( !player.at( i ).isDead() )
      {
         if ( player.at( i ).getClassType() == Unit::linebacker )
            numOfLinebackers++;

         if ( player.at( i ).getClassType() == Unit::paintballer )
            numOfPaintballers++;

         if ( player.at( i ).getClassType() == Unit::artist )
            numOfArtists++;

         if ( player.at( i ).getClassType() == Unit::prankster )
            numOfPranksters++;
      }
   }

   sprintf_s( n, "      x %d", numOfLinebackers );
   displayText += n;
   displayText += "              ";

   sprintf_s( n, "x %d", numOfPaintballers );
   displayText += n;
   displayText += "\n\n\n";

   sprintf_s( n, "      x %d", numOfArtists );
   displayText += n;
   displayText += "              ";

   sprintf_s( n, "x %d", numOfPranksters );
   displayText += n;

   // display text
   fontArial24->DrawText( NULL, displayText.c_str(), displayText.length(), &displayRect, DT_CENTER | DT_WORDBREAK, D3DCOLOR_XRGB( 255, 255, 255 ) );
}

bool GameManager::parseFile(string fileName)
{
   ifstream fileReadIn;
   int temp = 0;
   int temp2 = 0;
   string check = "";
   bool endFile = false;
   
   fileReadIn.open( fileName.c_str(), std::ifstream::in );

   while( !endFile )
   {
      fileReadIn >>  check;

      // check if file is bad
      if ( fileReadIn.eof() )
         return false;

      if( check == "@Turn" )
      {
         int turn;
         fileReadIn >> turn;

         turn -= 1;
         myTurn = ( turn == 0 ? player1 : player2 );
      }

      if ( check == "@TurnsTaken" )
      {
         fileReadIn >> numOfTurnsTaken;
      }

      if( check == "@MapDims" )
      {
         fileReadIn >> myBoardSize;

         if ( myBoardSize <= 16 )
         {
            myBoardSize = 16;
         }
         else
         {
            myBoardSize = 32;
         }

         m_grid.init( myBoardSize, myBoardSize );
      }

      if(check == "@Map")
      {
         for(int row = 0; row < myBoardSize; row++)
         {
            for(int column = 0; column < myBoardSize; column++)
            {
               char ID;

               fileReadIn >> ID;

               if ( fileReadIn.fail() )
               {
                  OutputDebugString( "Read in of map data failed!\n" );
               }

               Unit unit;
               int health = -1;

               if ( fileReadIn.peek() == ',' )
               {
                  fileReadIn.ignore( 1, ',' );
                  fileReadIn >> health;
               }



               switch(ID)
               {
               // player 2 units
               case 'A':
                  unit.init( &m_grid, Unit::linebacker, player2, column, row, ( health == -1 ? 15 : health ), linebackerImage2 );
                  player2Units.push_back( unit );
                  break;
               case 'B':
                  unit.init( &m_grid, Unit::paintballer, player2, column, row, ( health == -1 ? 5 : health ), paintballerImage2 );
                  player2Units.push_back( unit );
                  break;
               case 'C':
                  unit.init( &m_grid, Unit::artist, player2, column, row, ( health == -1 ? 10 : health ), artistImage2 );
                  player2Units.push_back( unit );
                  break;
               case 'D':
                  unit.init( &m_grid, Unit::prankster, player2, column, row, ( health == -1 ? 10 : health ), pranksterImage2 );
                  player2Units.push_back( unit );
                  break;
               
               // player 1 units
               case '1':
                  unit.init( &m_grid, Unit::linebacker, player1, column, row, ( health == -1 ? 15 : health ), linebackerImage1 );
                  player1Units.push_back( unit );
                  break;
               case '2':
                  unit.init( &m_grid, Unit::paintballer, player1, column, row, ( health == -1 ? 5 : health ), paintballerImage1 );
                  player1Units.push_back( unit );
                  break;
               case '3':
                  unit.init( &m_grid, Unit::artist, player1, column, row, ( health == -1 ? 10 : health ), artistImage1 );
                  player1Units.push_back( unit );
                  break;
               case '4':
                  unit.init( &m_grid, Unit::prankster, player1, column, row, ( health == -1 ? 10 : health ), pranksterImage1 );
                  player1Units.push_back( unit );
                  break;

               // tile states
               case 'T':
                  m_grid.setTileState( column, row, Tile::trapped );
                  break;
               case 'X':
                  m_grid.setTileState( column, row, Tile::blocked );
                  break;
               case 'O':
                  m_grid.setTileState( column, row, Tile::occupiedTrap );
                  break;
               default:
                  break;
               }

               // if a unit is dead, set the tile underneath it to be blocked
               if ( health == 0 )
               {
                  m_grid.setTileState( column, row, Tile::blocked );
               }

            }
         }
      }

      if(check == "@end")
      {	
         endFile = true;
      }
   }
   fileReadIn.close();
   return true;
}

bool GameManager::writeFile ( string fileName )
{
   ofstream fileWrite( fileName.c_str(), ios_base::trunc );

   if ( fileWrite.fail() )
      return false;

   fileWrite << "@Turn" << "\t\t" << myTurn + 1 << "\n\n";

   fileWrite << "@TurnsTaken" << "\t\t" << numOfTurnsTaken << "\n\n";

   if ( fileWrite.fail() )
      return false;

   fileWrite << "@MapDims" << "\t\t" << myBoardSize << "\n\n";

   fileWrite << "@Map";

   for ( int row = 0; row < myBoardSize; row++ )
   {
      // put each row on a new line
      fileWrite << "\n";

      for ( int column = 0; column < myBoardSize; column++ )
      {
         Tile::TileState state = m_grid.getTileState( column, row );

         switch ( state )
         {

         case Tile::empty:
            fileWrite << "0 ";
            break;

         case Tile::trapped:
            fileWrite << "T ";
            break;

         case Tile::occupied:
         case Tile::blocked:
            // get unit type that's occupying space

            // search player1's units
            for ( int i = 0; i < (int)player1Units.size(); i++ )
            {
               if ( column == player1Units.at( i ).getGridXPos() && row == player1Units.at( i ).getGridYPos() )
               {
                  Unit::ClassType classType = player1Units.at( i ).getClassType();

                  switch ( classType )
                  {
                  case Unit::linebacker:
                     fileWrite << "1," << player1Units.at( i ).getHealth() << " ";
                     break;
                  case Unit::paintballer:
                     fileWrite << "2," << player1Units.at( i ).getHealth() << " ";
                     break;
                  case Unit::artist:
                     fileWrite << "3," << player1Units.at( i ).getHealth() << " ";
                     break;
                  case Unit::prankster:
                     fileWrite << "4," << player1Units.at( i ).getHealth() << " ";
                     break;
                  default:
                     break;
                  }
               }
            }

            // search player2's units
            for ( int i = 0; i < (int)player2Units.size(); i ++ )
            {
               if ( column == player2Units.at( i ).getGridXPos() && row == player2Units.at( i ).getGridYPos() )
               {
                  Unit::ClassType classType = player2Units.at( i ).getClassType();

                  switch ( classType )
                  {
                  case Unit::linebacker:
                     fileWrite << "A," << player2Units.at( i ).getHealth() << " ";
                     break;
                  case Unit::paintballer:
                     fileWrite << "B," << player2Units.at( i ).getHealth() << " ";
                     break;
                  case Unit::artist:
                     fileWrite << "C," << player2Units.at( i ).getHealth() << " ";
                     break;
                  case Unit::prankster:
                     fileWrite << "D," << player2Units.at( i ).getHealth() << " ";
                     break;
                  default:
                     break;
                  }
               }
            }

            break;

         case Tile::occupiedTrap:
            fileWrite << "O ";
            break;

         default:
            break;
         }
      }
   }

   fileWrite << "\n\n";

   fileWrite << "@end";

   fileWrite.close();
   return true;
}

bool GameManager::loadLevel ()
{
   // get file
   BOOL b;
   string filename;
   char buffer [ MAX_PATH ] = {0};

   bool result = false;

   OPENFILENAME ofn;
   memset( &ofn, 0, sizeof(ofn));
   ofn.lStructSize   = sizeof( ofn );
   ofn.lpstrFilter   = "Level Files\0*.level\0\0";
   ofn.nFilterIndex  = 1;
   ofn.lpstrFile     = buffer;
   ofn.nMaxFile      = MAX_PATH;
   ofn.lpstrInitialDir = "..\\Assets\\Saves";
   ofn.lpstrTitle    = "Select a Level File to load!";
   ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_FORCESHOWHIDDEN | OFN_NOCHANGEDIR | OFN_SHAREAWARE;
   b = GetOpenFileName( &ofn );
   if ( b )
   {
      filename = ofn.lpstrFile;

      // parse the file and load if successful
      result = parseFile( filename );
   }
   else
   {
      filename = "";

      result = false;
   }

   if ( result )
   {
      return result;
   }
   else
   {
      parseFile( "../Assets/Levels/TestMap16.level" );

      return result;
   }

}

bool GameManager::saveLevel ()
{
   BOOL b;
   string filename;
   char buffer [ MAX_PATH ] = {0};

   OPENFILENAME ofn;
   memset( &ofn, 0, sizeof(ofn));
   ofn.lStructSize   = sizeof( ofn );
   ofn.lpstrFilter   = "Level Files\0*.level\0\0";
   ofn.nFilterIndex  = 1;
   ofn.lpstrFile     = buffer;
   ofn.nMaxFile      = MAX_PATH;
   ofn.lpstrInitialDir = "..\\Assets\\Saves";
   ofn.lpstrTitle    = "Save a level!";
   ofn.Flags = OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_FORCESHOWHIDDEN | OFN_NOCHANGEDIR | OFN_SHAREAWARE;
   b = GetSaveFileName( &ofn );

   if ( b )
   {
      filename = ofn.lpstrFile;

      // write to the file if successful
      writeFile( filename );
      return true;
   }
   else
   {
      filename = "";
      return false;
   }
}