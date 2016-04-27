#pragma once

#if !defined ( __GAMEMANAGER_H__ )
#define __GAMEMANAGER_H__

#include <vector>
#include "Unit.h"
#include "grid.h"
#include "DxFramework.h"

using namespace std;

class GameManager : public DxFramework
{
public:
   enum PlayerTurn { player1, player2 };

   GameManager ();
   ~GameManager ();

   bool gamePreInit ();
   bool gameInit (int boardsize);
   void gameRun ();
   void gameExit ();
   void gameShutdown ();

   // input true to force turn to end.  Otherwise it checks if it's naturally the end of the turn.
   bool endTurn ( bool force = false );

   int winWidth () { return 1024; }
   int winHeight () { return 728; }

private:
   void playerTurn ( vector<Unit>& player, vector<Unit>& enemyPlayer );
   bool initPlayers ( vector<Unit>* playersUnits, PlayerTurn player );

   bool initPlayer1 ();
   bool initPlayer2 ();

   Unit* selectUnit ( vector<Unit>& player );
   POINT selectSpace ();

   void displaySidebar ( vector<Unit>& player );

   void releaseTexture ( LPDIRECT3DTEXTURE9& texture );
   void releaseFont ( LPD3DXFONT& font );


private:
   bool initialized;

   vector<Unit> player1Units;
   vector<Unit> player2Units;

   DxTexture linebackerImage1;
   DxTexture paintballerImage1;
   DxTexture artistImage1;
   DxTexture pranksterImage1;

   DxTexture linebackerImage2;
   DxTexture paintballerImage2;
   DxTexture artistImage2;
   DxTexture pranksterImage2;

   DxTexture filledTileImage;
   DxTexture emptyTileImage;

   LPD3DXFONT fontArial24;

   int myStartLinebackerNum;
   int myStartPaintballerNum;
   int myStartArtistNum;
   int myStartPranksterNum;

   D3DXVECTOR2 tileSize;
   
   
   int myBoardSize;
   float scaleFactor;

   PlayerTurn myTurn;

   Grid m_grid;

   bool myGameIsOver;
   bool myTurnStart;

   bool keyPressed;

   Unit* selectedUnit;
   Unit* enemyUnit;

   bool selectedUnitMove;
   bool showAttackRange;
   bool showSpecialRange;
};

#endif