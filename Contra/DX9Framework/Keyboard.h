//==============================
// Last change: 26/9/2014
// T6_Framework
// T6_Keyboard v1.0
// This is the class allow you to connect to your keyboard and get it's status.
//==============================
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <Windows.h>
#include <dinput.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "DefaultConstant.h"

#define CHECK_IF_STATUS_IS_DOWN(status) (status & 0x80)

enum KeyStatus
{
	KEY_DOWN = 0,
	KEY_PRESS,
	KEY_RELEASE,
	KEY_UP,
	NUM_OF_KEY_STATUS
};

class Keyboard
{
public:
	Keyboard(HINSTANCE hInstance, HWND windowHandler);
	~Keyboard();

	void update();
	KeyStatus getKeyStatus(int key);

private:
	bool createKeyboard();

private:
	HWND					_windowHandler;
	HINSTANCE				_hInstance;
	IDirectInput8*			_inputDevice;	
	IDirectInputDevice8*	_inputDevice_Keyboard;

	BYTE					_keyboardStatus[256];
	BYTE					_keyboardStatusPrevious[256];
};

#endif