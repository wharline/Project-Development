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

   bool gameInit ();
   void gameRun ();
   void gameExit ();

   int winWidth () { return 1024; }
   int winHeight () { return 728; }

private:
   void playerTurn ( vector<Unit> player );
   bool initPlayers ( vector<Unit>* playersUnits, PlayerTurn player );

   bool initPlayer1 ();
   bool initPlayer2 ();

private:
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

   int myStartLinebackerNum;
   int myStartPaintballerNum;
   int myStartArtistNum;
   int myStartPranksterNum;
   
   
   int myBoardSize;

   float scaleFactor;

   PlayerTurn myTurn;

   Grid m_grid;

   bool myGameIsOver;
};

#endif