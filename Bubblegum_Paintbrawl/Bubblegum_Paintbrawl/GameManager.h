#pragma once

#if !defined ( __GAMEMANAGER_H__ )
#define __GAMEMANAGER_H__

#include <vector>
#include "Unit.h"
#include "grid.h"
#include "Bubblegum_Paintbrawl.h"

using namespace std;

class GameManager : public Framework
{
public:
   enum PlayerTurn { player1, player2 };

   GameManager ();
   ~GameManager ();

   bool init ();
   void update ();
   void shutdown ();

private:
   vector<Unit> player1Units;
   vector<Unit> player2Units;

   PlayerTurn myTurn;

   Grid m_grid;
};

#endif