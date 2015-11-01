#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "GameDefaultConstant.h"
#include "Timer.h"

#include "DX9Framework/Sprite.h"

class Effect
{
public:
	Effect(const tchar* spritePatch, int nColumns, int nRows,
			D3DXVECTOR3 position, DWORD delayTime = 0, int totalframe = 0);
	~Effect();

	void setFrameTime(double newTime);
	bool draw();

private:
	Sprite*	_effectSprite;
	DWORD		_delayTime;
	D3DXVECTOR3 _position;
};

#endif // Effect_h__
