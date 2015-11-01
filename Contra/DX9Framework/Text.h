//==============================
// Last change: 27/9/2014
// T6_Framework
// T6_Text v1.0
// With this class, you can define a text-processing object, which allow you
// to draw text onto your windows.
//==============================
#ifndef __TEXT_H__
#define __TEXT_H__

#include <d3dx9.h>
#include <string>
#include <iostream>
#include <Windows.h>

#include "DefaultConstant.h"
#include "DefaultStructure.h"
#include "DefaultMethods.h"

#define PIXEL_TO_POINT96(A) ((A/2*72/96))
#define POINT_TO_PIXEL96(A) ((A*2*96/72))

#define DEFAULT_FONT_COLOR 0xFFBCBCBC

class Text
{
public:
	Text(LPDIRECT3DDEVICE9 d3dDevice, LPD3DXSPRITE& spriteHandler);
	~Text(void);

	static void	setPositioncConvertMatrix(const D3DXMATRIX& newMatrix);


	// -----------------------------------------------
	// Desc: Draw any kind of object onto screen, base on passed position.
	// -----------------------------------------------
	template <class T> 
	void drawText(const T& Content, D3DXVECTOR3 Position, D3DCOLOR Color = DEFAULT_FONT_COLOR,
			 int Size = DEFAULT_FONTSIZE, UINT DT_Type = DT_LEFT, bool converted = true)
	{
		if(Size != _fontSize)
			setFontSize(Size);

		D3DXVECTOR3 pos(Position);

		if (converted) {
			D3DXVec3TransformCoord(&pos, &pos, &_cameraConvertMatrix);
		}
		
		RECT drawField;

		drawField = { pos.x, pos.y, 0, 0 };

		_font->DrawText(_spriteHandler, Default::tostr(Content).c_str(),
			-1, &drawField, DT_CALCRECT, Color);

		if (DT_Type == DT_CENTER)
		{
			LONG w = drawField.right - drawField.left;
			drawField.left -= (LONG)(w / 2.0F);
			drawField.right -= (LONG)(w / 2.0F);
		}
		else if (DT_Type == DT_RIGHT)
		{
			LONG w = drawField.right - drawField.left;
			drawField.left -= w;
			drawField.right -= w;
		}

		_font->DrawText(_spriteHandler, Default::tostr(Content).c_str(),
			-1, &drawField, DT_Type, Color);
	}


	// -----------------------------------------------
	// Desc: Draw any kind of object onto screen, base on passed draw field.
	// -----------------------------------------------
	template <class T> 
	void drawText(const T& Content, RECT* drawField, D3DCOLOR Color = DEFAULT_FONT_COLOR,
		int Size = DEFAULT_FONTSIZE, UINT DT_Type = DT_LEFT)
	{
		if(Size != m_fontSize)
			SetFontSize(Size);

		m_font->DrawText(*_spriteHandler, tostr(Content),
						-1, drawField, DT_Type,Color);
	}
	
private:
	void setFontSize(int newSize);
	void initializeFont();

private:
	LPD3DXSPRITE _spriteHandler;
	static D3DXMATRIX _cameraConvertMatrix;
	LPDIRECT3DDEVICE9 _d3dDevice;
	LPD3DXFONT _font;
	LPCTSTR _fontFace;
	int _fontSize;
};

#endif