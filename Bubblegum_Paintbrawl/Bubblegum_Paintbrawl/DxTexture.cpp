#include "stdafx.h"
#include "DxTexture.h"

DxTexture::DxTexture ()
{
   myTexture = NULL;
}

DxTexture::~DxTexture ()
{
}

bool DxTexture::create ( LPDIRECT3DTEXTURE9 texture, D3DXIMAGE_INFO info )
{
   myTexture = texture;
   myInfo = info;

   return true;
}