#include "stdafx.h"
#include "DxFramework.h"

bool DxFramework::init ()
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
	d3ddev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer );

   if ( !directInputInit() )
   {
      return false;
   }

	if ( !gameInit() )
      return false;

   return true;
}

void DxFramework::update ()
{
   // make sure the Direct3D device is valid
	if ( !d3ddev ) return;

   directInputUpdate();

   gameRun();
}

void DxFramework::shutdown ()
{
   gameExit();
   
	if ( d3ddev ) d3ddev->Release();
	if ( d3d ) d3d->Release();

   directInputShutdown();
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
		ZeroMemory( &controllers[i], sizeof( XINPUT_STATE ) );

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

int DxFramework::keyDown ( int key )
{
   return 0;
}

int DxFramework::mouseButton ( int button )
{
   return 0;
}
int DxFramework::mouseX ()
{
	return mouse_state.lX;
}

int DxFramework::mouseY ()
{
	return mouse_state.lY;
}

void DxFramework::xInputVibrate ( int contNum, int amount )
{
}

bool DxFramework::xInputControllerFound ()
{
   return true;
}
