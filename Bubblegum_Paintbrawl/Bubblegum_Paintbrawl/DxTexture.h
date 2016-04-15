#pragma once

#if !defined ( __DxTEXTURE_H__ )
#define __DxTEXTURE_H__

#include "Common.h"

class DxTexture
{
public:
   DxTexture ();
   ~DxTexture ();

   bool create ( LPDIRECT3DTEXTURE9 texture, D3DXIMAGE_INFO info );

   int width () { return (int)myInfo.Width; }
   int height () { return (int)myInfo.Height; }

   LPDIRECT3DTEXTURE9& texture () { return myTexture; }

private:
   LPDIRECT3DTEXTURE9 myTexture;
   D3DXIMAGE_INFO myInfo;
};

#endif