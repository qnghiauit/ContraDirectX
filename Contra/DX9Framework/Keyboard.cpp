#include "Keyboard.h"

// -----------------------------------------------
// Desc: Create an interface to get the keyboard status.
// -----------------------------------------------
Keyboard::Keyboard(HINSTANCE hInstance, HWND windowHandler)
{
	_inputDevice			= 0;
	_inputDevice_Keyboard	= 0;
	_hInstance				= hInstance;
	_windowHandler			= windowHandler;

	if (!createKeyboard())
		WARNING_BOX(WARNING_CANNOT_START_KEYBOARD);
}


// -----------------------------------------------
// Desc: Release the interface after use (close game).
// -----------------------------------------------
Keyboard::~Keyboard()
{
	if(_inputDevice_Keyboard != 0)
	{
		_inputDevice_Keyboard->Unacquire();
		_inputDevice_Keyboard->Release();
		_inputDevice_Keyboard = 0;
	}

	if(_inputDevice != 0)
	{
		_inputDevice->Release();
		_inputDevice = 0;
	}
}


// -----------------------------------------------
// Desc: Create keyboard interface.
// -----------------------------------------------
bool Keyboard::createKeyboard()
{
	HRESULT result = DI_OK;
	
	result = result | DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_inputDevice, 0);
	result = result | _inputDevice->CreateDevice(GUID_SysKeyboard, &_inputDevice_Keyboard, 0);

	result = result | _inputDevice_Keyboard->SetDataFormat(&c_dfDIKeyboard);
	result = result | _inputDevice_Keyboard->SetCooperativeLevel(_windowHandler, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	result = result | _inputDevice_Keyboard->Acquire();

	if (result != DI_OK)
		return false;
	return true;
}


// -----------------------------------------------
// Desc: Store the old keyboard status then get the new one.
// -----------------------------------------------
void Keyboard::update()
{
	memcpy(_keyboardStatusPrevious, _keyboardStatus, sizeof(_keyboardStatus));

	HRESULT result;
	int i = sizeof(_keyboardStatus);
	result = _inputDevice_Keyboard->GetDeviceState(sizeof(_keyboardStatus), (LPVOID)&_keyboardStatus);

	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			_inputDevice_Keyboard->Acquire();
		else
			WARNING_BOX(WARNING_CANNOT_READ_KEYBOARD);
	}
}


// -----------------------------------------------
// Desc: Check if the key is DOWN, UP, just PRESS or just RELEASE.
// -----------------------------------------------
KeyStatus Keyboard::getKeyStatus(int key)
{
	if (CHECK_IF_STATUS_IS_DOWN(_keyboardStatus[key])) {
		if (CHECK_IF_STATUS_IS_DOWN(_keyboardStatusPrevious[key]))
			return KeyStatus::KEY_DOWN;
		else
			return KeyStatus::KEY_PRESS;
	}
	else {
		if (CHECK_IF_STATUS_IS_DOWN(_keyboardStatusPrevious[key]))
			return KeyStatus::KEY_RELEASE;
		else
			return KeyStatus::KEY_UP;
	}
}