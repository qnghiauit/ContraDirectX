#include "Effect.h"



Effect::Effect(const tchar* spritePatch, int nColumns, int nRows, D3DXVECTOR3 position, DWORD delayTime /*= 0*/, int totalframe /*= 0*/)
{
	this->_effectSprite = new Sprite(spritePatch, nColumns, nRows, totalframe);
	this->_delayTime = delayTime;
	this->_position = position;
}


Effect::~Effect()
{
	SAFE_RELEASE(_effectSprite);
}


bool Effect::draw()
{
	if (_delayTime > Timer::getRoundTime())
	{
		_delayTime -= Timer::getRoundTime();
		return false;
	}

	return this->_effectSprite->render(Timer::getRoundTime(), _position, ANGLE_NO,
										SCALE_NO, TRANSLATE_NO, DrawCenter::CENTER);
}


void Effect::setFrameTime(double newTime)
{
	if (_effectSprite != nullptr)
		_effectSprite->setFrameTime(newTime);
}

