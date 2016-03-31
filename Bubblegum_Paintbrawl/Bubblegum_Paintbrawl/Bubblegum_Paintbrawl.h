#pragma once

#include "resource.h"

class Framework
{
public:

   Framework () {}
   virtual ~Framework () {}

   void quitGame ();

   virtual int winWidth () { return 640; }
   virtual int winHeight () { return 480; }
};