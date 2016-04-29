#include "stdafx.h"
#include "DxFramework.h"

bool DxFramework::init (int boardsize)
{
   HRESULT result;

   // initialize Direct3D
	d3d = Direct3DCreate9( D3D_SDK_VERSION );
	if ( !d3d )
      return false;

	// set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed			= ( !fullscreen() );
	d3dpp.SwapEffect		= D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat	= D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount	= 1;
	d3dpp.BackBufferWidth	= winWidth();
	d3dpp.BackBufferHeight	= winHeight();
	d3dpp.hDeviceWindow		= hwnd();

	// create Direct3D device
	result = d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev );

	if ( !d3ddev || result != D3D_OK )
      return false;

	// get a point to the gack buffer surface
	result = d3ddev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer );

   if ( FAILED( result ) )
      return false;

	// create sprite object
	result = D3DXCreateSprite( d3ddev, &spriteobj );

   if ( FAILED( result ) )
      return false;

   if ( !directInputInit() )
   {
      return false;
   }

   if ( !gamePreInit() )
   {
      return false;
   }

   return true;
}

void DxFramework::update ()
{
   // make sure the Direct3D device is valid
	if ( !d3ddev )
      return;

   directInputUpdate();

   gameRun();
}

void DxFramework::shutdown ()
{
   gameExit();
   gameShutdown();
   
   directInputShutdown();

	if ( d3ddev )
      d3ddev->Release();

	if ( d3d )
      d3d->Release();
}



bool DxFramework::loadTexture ( DxTexture& dxTex, string filename, D3DCOLOR transcolor )
{
	LPDIRECT3DTEXTURE9 texture = NULL;

	// get width and height from bitmap file
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile( filename.c_str(), &info );
	if ( result != D3D_OK ) return false;

	// create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx(
		d3ddev,					// Direct3D device object
		filename.c_str(),		// bitmap filename
		info.Width,				// bitmap image width
		info.Height,			// bitmap image height
		1,						   // mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,		// the type of surface (standard)
		D3DFMT_UNKNOWN,			// surface format (default)
		D3DPOOL_DEFAULT,		// memory class for the texture
		D3DX_DEFAULT,			// image filter
		D3DX_DEFAULT,			// mip filter
		transcolor,				// color key for transparency
		&info,					// bitmp file info (from loaded file)
		NULL,					   // color palette
		&texture );				// destination texture

	// make sure the bitmap texture was loaded correctly
	if ( result != D3D_OK ) return NULL;

   dxTex.create( texture, info );

	return true;
}


D3DXVECTOR2 DxFramework::getBitmapSize ( string filename )
{
	D3DXIMAGE_INFO info;
	D3DXVECTOR2 size = D3DXVECTOR2( 0.0f, 0.0f );
	HRESULT result = D3DXGetImageInfoFromFile( filename.c_str(), &info );

	// ask Scott about this.  Does there really need to be an "if" statement?
	if ( result == D3D_OK )
		size = D3DXVECTOR2( ( float )info.Width, ( float )info.Height );
	else
		size = D3DXVECTOR2( ( float )info.Width, ( float )info.Height );

	return size;
}


void DxFramework::spriteDraw ( DxTexture& texture, int destx, int desty, float scaling, D3DXVECTOR2 scaleCenter, D3DCOLOR color)
{
	D3DXVECTOR3 position( ( float )destx, ( float )desty, 0 );

	RECT rect;
	rect.left = 0;
	rect.top = 0;
   rect.right = texture.width();
   rect.bottom = texture.height();

   D3DXVECTOR2 scale( scaling, scaling );

   D3DXVECTOR2 center( (float)( texture.width() * scaling )/2 + destx, (float)( texture.height() * scaling )/2 + desty );

   D3DXVECTOR3 center3( 0, 0, 0 );

   D3DXMATRIX mat;
   D3DXMatrixTransformation2D( &mat, &scaleCenter, 0, &scale, &center, 0, NULL );

   HRESULT hr = spriteobj->SetTransform( &mat );

   if ( FAILED( hr ) )
      OutputDebugString( "Transform failed.\n" );

   hr = spriteobj->Draw( texture.texture(), &rect, &center3, &position, color );

   if ( FAILED( hr ) )
      OutputDebugString( "Draw failed.\n" );
}


//----------------------------------------------------------------------
// DIRECT INPUT FUNCTIONS
//----------------------------------------------------------------------

bool DxFramework::directInputInit ()
{
	// initialization DirectInput object
	HRESULT result = DirectInput8Create(
		GetModuleHandle( NULL ),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		( void** ) &dinput,
		NULL );

	// initialize the keyboard
	dinput->CreateDevice( GUID_SysKeyboard, &dikeyboard, NULL );
	dikeyboard->SetDataFormat( &c_dfDIKeyboard );
	dikeyboard->SetCooperativeLevel( hwnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	dikeyboard->Acquire();

	// initialize the mouse
	dinput->CreateDevice( GUID_SysMouse, &dimouse, NULL );
	dimouse->SetDataFormat( &c_dfDIMouse );
	dimouse->SetCooperativeLevel( hwnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	dimouse->Acquire();
	d3ddev->ShowCursor( false );

	return true;
}

void DxFramework::directInputUpdate ()
{
	// update mouse
	dimouse->GetDeviceState( sizeof( mouse_state ), ( LPVOID )&mouse_state );

	// update keyboard
	dikeyboard->GetDeviceState( sizeof( keys ), ( LPVOID )&keys );

	// update controllers
	for ( int i = 0; i < 4; i++ )
	{
		ZeroMemory( &controllers[i], sizeof( controllers[i] ) );

		// get the state of the controller
		XINPUT_STATE state;
		DWORD result = XInputGetState( i, &state );

		// store state in global controllers array
		if ( result == 0 ) controllers[i] = state.Gamepad;
	}
}

void DxFramework::directInputShutdown ()
{
	if ( dikeyboard )
	{
		dikeyboard->Unacquire();
		dikeyboard->Release();
		dikeyboard = NULL;
	}

	if ( dimouse )
	{
		dimouse->Unacquire();
		dimouse->Release();
		dimouse = NULL;
	}
}

bool DxFramework::keyDown ( int key )
{
   return ( GetAsyncKeyState( key ) & 0x8000 ) != 0;
}

bool DxFramework::mouseButton ( int button )
{
   if ( button == 0 )
   {
      return ( ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) != 0 );
   }

   if ( button == 1 )
   {
      return ( ( GetAsyncKeyState( VK_RBUTTON ) & 0x8000 ) != 0 );
   }

   if ( button == 2 )
   {
      return ( ( GetAsyncKeyState( VK_MBUTTON ) & 0x8000 ) != 0 );
   }

	return false;
}
int DxFramework::mouseX ()
{
	return mouse_state.lX;
}

int DxFramework::mouseY ()
{
	return mouse_state.lY;
}

POINT DxFramework::mousePos ()
{
   D3DXVECTOR2 pos;

   POINT p;

   GetCursorPos( &p );

   ScreenToClient( hwnd(), &p );

   return p;
}

void DxFramework::xInputVibrate ( int contNum, int amount )
{
}

bool DxFramework::xInputControllerFound ()
{
   return true;
}

LPD3DXFONT DxFramework::makeFont ( string name, int size )
{
   LPD3DXFONT font = NULL;

   D3DXFONT_DESC desc = {
      size,                   // height
      0,                      // width
      0,                      // weight
      0,                      // miplevels
      false,                  // italic
      DEFAULT_CHARSET,        // charset
      OUT_TT_PRECIS,          // output precision
      CLIP_DEFAULT_PRECIS,    // quality
      DEFAULT_PITCH,          // pitch and family
      ""                      // font name
   };

   strcpy_s( desc.FaceName, name.c_str() );
   D3DXCreateFontIndirect( d3ddev, &desc, &font );
   return font;
}

void DxFramework::fontPrint ( LPD3DXFONT font, int x, int y, string text, D3DCOLOR color )
{
   // figure out the text boundary
   RECT rect = { x, y, 0, 0 };
   font->DrawText( spriteInterface(), text.c_str(), text.length(), &rect, DT_LEFT, color );
}