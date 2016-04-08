#pragma once

// Common header files to bew used throughout project

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
