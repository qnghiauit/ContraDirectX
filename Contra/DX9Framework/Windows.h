//==============================
// Last change: 9/10/2014
// T6_Framework
// T6_Windows v1.1
// This is the class allow you to create a windows and define the device affect on that one.
// 26/9/2014: Fix startDraw and stopDraw functions, beside, added getWindowHandler function.
// 9/10/2014: Add new features about Screen Size and Resolution, Callback function for constructor.
//==============================
#ifndef __WINDOWS_H__
#define __WINDOWS_H__

#include <d3dx9.h>
#include "DefaultConstant.h"
#include "DefaultStructure.h"

typedef LRESULT(CALLBACK *WinProcedurePointer)(HWND, UINT, WPARAM, LPARAM);

class Windows
{
public:
	Windows(HINSTANCE hInstance, LPCTSTR windowsName,
				bool isWindowed,
				int width = SCREEN_WIDTH,
				int height = SCREEN_HEIGHT,
				WinProcedurePointer windowProcedure = 0,
				int backgroundColor = BLACK_BRUSH,
				LPCTSTR cursorName = 0, LPCTSTR iconName = 0);

	~Windows(void);

	bool StartingDevice(Size screenResolution = Size(RESOLUTION_WIDTH, RESOLUTION_HEIGHT),
							bool isWindowed = TRUE,	int nBuffers = 1,
							D3DSWAPEFFECT swapEffect = D3DSWAPEFFECT_COPY);

	Size				getResolution();
	LPD3DXSPRITE&		getSpriteHandler();
	HWND&				getWindowHandler();
	LPDIRECT3D9&		getD3DObject();
	LPDIRECT3DDEVICE9&	getD3DDevice();

	void				setWindowTitle(LPCTSTR newTitle);

	bool				startDraw();
	void				stopDraw();

private:
	
	HINSTANCE			_hInstance;
	HWND				_wndHandler;

	LPDIRECT3D9			_d3dObject;
	LPDIRECT3DDEVICE9	_d3dDevice;
	LPD3DXSPRITE		_spriteHandler;

	Size				_resolution;
	D3DXMATRIX			_originalSpriteHandlerMatrix;
};

#endif