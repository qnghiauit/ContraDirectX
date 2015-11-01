#include "FreeText.h"


FreeText* FreeText::_instance = nullptr;

LPD3DXSPRITE FreeText::_spriteHandler = nullptr;

LPDIRECT3DDEVICE9 FreeText::_device = nullptr;


FreeText::FreeText()
{
	if (_spriteHandler != nullptr && _device != nullptr)
	{
		_textHandler = new Text(_device, _spriteHandler);
	}
	else
		WARNING_BOX(WARNING_INITIALIZE_FREE_TEXT);
}


FreeText::~FreeText()
{
}

void FreeText::initialize(LPDIRECT3DDEVICE9& d3dDevice, LPD3DXSPRITE& spriteHandler)
{
	_device = d3dDevice;
	_spriteHandler = spriteHandler;
}


FreeText& FreeText::get()
{
	if (_instance == nullptr)
	{
		_instance = new FreeText();
	}

	return *_instance;
}
