#pragma once

#include "resource.h"

class WinFramework
{
public:

   WinFramework () {}
   virtual ~WinFramework () {}

   void quitGame ();

   virtual int winWidth () { return 640; }
   virtual int winHeight () { return 480; }

   HWND hwnd () { return hMainWnd; }

   void setHWnd ( HWND hwnd );

private:
   HWND hMainWnd;
};