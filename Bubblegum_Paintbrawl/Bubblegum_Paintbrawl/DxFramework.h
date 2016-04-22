#pragma once

#if !defined( __DxFRAMEWORK_H__ )
#define __DxFRAMEWORK_H__

#include "Bubblegum_Paintbrawl.h"

#include "Common.h"
#include "DxTexture.h"

class DxFramework : public WinFramework
{
public:
   DxFramework () {}
   virtual ~DxFramework () {}

   virtual bool gameInit (int boardsize) = 0;
   virtual void gameRun () = 0;
   virtual void gameExit () = 0;
   
   virtual bool fullscreen () { return false; }

   bool init (int boardsize);
   void update ();
   void shutdown ();


protected:
   // DirectX functions
   LPDIRECT3DSURFACE9 LoadSurface ( string filename );
   void DrawSurface ( LPDIRECT3DSURFACE9 dest, float x, float y, LPDIRECT3DSURFACE9 source );
   D3DXVECTOR2 GetBitmapSize ( string filename );
   bool LoadTexture ( DxTexture& dxTex, string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB( 241, 242, 243 ) );
   void Sprite_Draw_Frame ( DxTexture& texture, int destx, int desty, float scaling, D3DXVECTOR2 scaleCenter, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ) );
   void Sprite_Animate ( int &frame, int startframe, int endframe, int direction, int &starttime, int delay );

   // get the device
   LPDIRECT3DDEVICE9 dxDevice () { return d3ddev; }
   LPD3DXSPRITE spriteInterface () { return spriteobj; }

   // DirectInput functions
   bool directInputInit ();
   void directInputUpdate ();
   void directInputShutdown ();
   bool keyDown ( int key );

   // 0 means left button, 1 is right button, and 2 is middle button
   bool mouseButton ( int button );
   int mouseX ();
   int mouseY ();

   void xInputVibrate ( int contNum = 0, int amount = 65535 );
   bool xInputControllerFound ();

   // get the mouse position in the game window
   POINT mousePos ();

   // for printing text to the screen
   LPD3DXFONT makeFont ( string name, int size );
   void fontPrint ( LPD3DXFONT font, int x, int y, string text, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ) );


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