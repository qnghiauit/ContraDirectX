#include "Windows.h"

Windows::Windows (HINSTANCE hInstance,LPCTSTR windowsName,bool isWindowed,
						int width, int height, WinProcedurePointer WndProc,
						int backgroundColor, LPCTSTR cursorName, LPCTSTR iconName)	
{
	_resolution.width = width;
	_resolution.height = height;

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	this->_hInstance	= hInstance;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.hInstance		= this->_hInstance;
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground	= (HBRUSH)backgroundColor + 1;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.lpszClassName	= windowsName;
	if (cursorName == 0)
		wcex.hCursor = LoadCursor(_hInstance, DEFAULT_CURSOR);
	else
		wcex.hCursor	= (HCURSOR)	LoadImage(0, cursorName, IMAGE_CURSOR,	RESOLUTION_CURSOR_W,
		RESOLUTION_CURSOR_H, LR_SHARED|LR_LOADFROMFILE);

	if (iconName == 0) {
		wcex.hIcon	= LoadIcon(_hInstance, DEFAULT_ICON);
		wcex.hIconSm = LoadIcon(_hInstance, DEFAULT_ICON);
	}
	else {
		wcex.hIcon		= (HICON)	LoadImage(0, iconName,	IMAGE_ICON,	RESOLUTION_ICON_W,
			RESOLUTION_ICON_H, LR_SHARED | LR_LOADFROMFILE);
		wcex.hIconSm	= (HICON)	LoadImage(0, iconName,	IMAGE_ICON,	RESOLUTION_ICON_W,
			RESOLUTION_ICON_H, LR_SHARED | LR_LOADFROMFILE);
	}

	RegisterClassEx(&wcex);

	this->_wndHandler	= CreateWindow (windowsName, windowsName,
		isWindowed ? WS_OVERLAPPEDWINDOW : 
		(WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE),
		0, 0, 
		width + 2 * SIZE_BORDER, 
		height + SIZE_BORDER + SIZE_TITLEBAR,
		0, 0, 
		this->_hInstance, 0);

	if (!this->_wndHandler) {
		WARNING_BOX (WARNING_CANNOT_CREATE_WINDOWS);
		return;
	}

	UpdateWindow(_wndHandler);
	ShowWindow(_wndHandler, SW_SHOW);
}



Windows::~Windows(void)
{
	if (_d3dDevice  != 0)		_d3dDevice ->Release();
	if (_d3dObject  != 0)		_d3dObject ->Release();
	if (_spriteHandler != 0)	_spriteHandler->Release();
}



Size Windows::getResolution()
{
	return _resolution;
}



LPD3DXSPRITE& Windows::getSpriteHandler()
{
	return _spriteHandler;
}



HWND& Windows::getWindowHandler()
{
	return _wndHandler;
}



LPDIRECT3D9& Windows::getD3DObject()
{
	return _d3dObject;
}



LPDIRECT3DDEVICE9& Windows::getD3DDevice()
{
	return _d3dDevice;
}



bool Windows::startDraw()
{
	if (_d3dDevice == 0) {
		WARNING_BOX(WARNING_UNDETECTED_DEVICE);
		return false;
	}
	else {
		_d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0xFF000000, 1.0f, 0);
		_d3dDevice->BeginScene();
		_spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		return true;
	}
}



void Windows::stopDraw()
{
	_spriteHandler->End();
	_d3dDevice->EndScene();
	_d3dDevice->Present(0, 0, 0, 0);
}



bool Windows::StartingDevice(Size screenResolution, bool isWindowed,
								int nBuffers, D3DSWAPEFFECT swapEffect)
{
	_d3dObject = 0;
	_d3dDevice = 0;

	if ((_d3dObject = Direct3DCreate9(D3D_SDK_VERSION)) == 0) {
		return false;
	}

	D3DPRESENT_PARAMETERS theParameter;
	ZeroMemory(&theParameter, sizeof(theParameter));

	theParameter.Windowed			= isWindowed;
	theParameter.BackBufferFormat	= isWindowed ? D3DFMT_UNKNOWN : D3DFMT_X8R8G8B8;
	theParameter.SwapEffect			= swapEffect;
	theParameter.BackBufferWidth	= (UINT)screenResolution.width;
	theParameter.BackBufferHeight	= (UINT)screenResolution.height;
	theParameter.BackBufferCount	= nBuffers;
	theParameter.hDeviceWindow		= this->_wndHandler;

	HRESULT CreateDeviceResult = _d3dObject->CreateDevice(
		D3DADAPTER_DEFAULT,  D3DDEVTYPE_HAL, _wndHandler,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &theParameter, &_d3dDevice);

	if (FAILED(CreateDeviceResult)) {
		return false;
	}

	D3DXCreateSprite(_d3dDevice, &_spriteHandler);
	D3DXMatrixIdentity(&_originalSpriteHandlerMatrix);
	return true;
}


void Windows::setWindowTitle(LPCTSTR newTitle)
{
	SetWindowText(_wndHandler, newTitle);
}
