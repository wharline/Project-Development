#pragma once

#if !defined( __DxFRAMEWORK_H__ )
#define __DxFRAMEWORK_H__

#include "Bubblegum_Paintbrawl.h"

// header files
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
using namespace std;

// libraries
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "xinput.lib" )


class DxFramework : public WinFramework
{
public:
   DxFramework () {}
   virtual ~DxFramework () {}

   virtual bool gameInit () = 0;
   virtual void gameRun () = 0;
   virtual void gameExit () = 0;
   
   virtual bool fullscreen () { return false; }

   bool init ();
   void update ();
   void shutdown ();


private:
   // DirectX functions
   LPDIRECT3DSURFACE9 LoadSurface ( string filename );
   void DrawSurface ( LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source );
   D3DXVECTOR2 GetBitmapSize ( string filename );
   LPDIRECT3DTEXTURE9 LoadTexture ( string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB( 0, 0, 0 ) );
   void Sprite_Draw_Frame ( LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framenum, int framew, int frameh, int columns );
   void Sprite_Animate ( int &frame, int startframe, int endframe, int direction, int &starttime, int delay );

   // DirectInput functions
   bool directInputInit ();
   void directInputUpdate ();
   void directInputShutdown ();
   int keyDown ( int key );
   int mouseButton ( int button );
   int mouseX ();
   int mouseY ();
   void xInputVibrate ( int contNum = 0, int amount = 65535 );
   bool xInputControllerFound ();


private:
   // Direct3D objects
   LPDIRECT3D9          d3d;
   LPDIRECT3DDEVICE9    d3ddev;
   LPDIRECT3DSURFACE9   backbuffer;
   LPD3DXSPRITE         spriteobj;

   // DirectInput objects, devices, and states
   LPDIRECTINPUT8 dinput;
   LPDIRECTINPUTDEVICE8 dimouse;
   LPDIRECTINPUTDEVICE8 dikeyboard;
   DIMOUSESTATE mouse_state;
   XINPUT_GAMEPAD controllers[4];
   char keys[256];

};

#endif