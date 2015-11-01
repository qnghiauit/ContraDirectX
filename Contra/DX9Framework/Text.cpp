#include "Text.h" 

D3DXMATRIX Text::_cameraConvertMatrix;

// -----------------------------------------------
// Desc: Set the basic PROPERTIES such as font name, size, device to draw.
// -----------------------------------------------
Text::Text(LPDIRECT3DDEVICE9 d3dDevice,LPD3DXSPRITE& spriteHandler)
{
	_d3dDevice		= d3dDevice;
	_fontFace		= DEFAULT_FONTFACE;
	_fontSize		= DEFAULT_FONTSIZE;
	_spriteHandler	= spriteHandler;
	D3DXMatrixIdentity(&_cameraConvertMatrix);

	AddFontResourceEx(DEFAULT_FONTPATH, FR_PRIVATE, 0);

	initializeFont();
}


// -----------------------------------------------
// Desc: Create a new font suit with current PROPERTIESs.
// -----------------------------------------------
void Text::initializeFont()
{
	D3DXCreateFont(
		_d3dDevice,
		_fontSize,
		0,400,0,false,
		DEFAULT_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_PITCH,
		_fontFace
		,&_font);
}


// -----------------------------------------------
// Desc: Set a new size for the font (processed in Points).
// -----------------------------------------------
void Text::setFontSize(int newSize)
{
	_font->Release();
	_fontSize = newSize;
	initializeFont();
}


// -----------------------------------------------
// Desc: Release Text-draw object after use (close Game).
// -----------------------------------------------
Text::~Text(void)
{
	_font->Release();
	RemoveFontResource(DEFAULT_FONTPATH);
}


// -----------------------------------------------
// Desc: Setted new matrix to convert draw text position.
// -----------------------------------------------
void Text::setPositioncConvertMatrix(const D3DXMATRIX& newMatrix)
{
	Text::_cameraConvertMatrix = newMatrix;
}