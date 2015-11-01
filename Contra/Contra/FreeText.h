#ifndef __FREE_TEXT_H__
#define __FREE_TEXT_H__

#include "DX9Framework/Text.h"
#include "GameDefaultConstant.h"

class FreeText
{
public:
	static void initialize(LPDIRECT3DDEVICE9& d3dDevice, LPD3DXSPRITE& spriteHandler);

	template <class T>
	void drawText(const T& Content, D3DXVECTOR3 Position, D3DCOLOR Color = DEFAULT_FONT_COLOR,
		int Size = DEFAULT_FONTSIZE, UINT DT_Type = DT_LEFT, bool converted = true)
	{
		_textHandler->drawText(Content, Position, Color, Size, DT_Type, converted);
	}

	static FreeText& get();

private:
	FreeText();
	~FreeText();

private:
	Text* _textHandler;
	static FreeText* _instance;
	static LPDIRECT3DDEVICE9 _device;
	static LPD3DXSPRITE _spriteHandler;
};

#endif // FreeText_h__
