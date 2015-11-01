#include "AnimatedPretties.h"


AnimatedPretties::AnimatedPretties(tstring path, index_t index, size_t rows, D3DXVECTOR3 position, int frameNum)
	: Pretties(path, index * frameNum, rows, frameNum, position, RESOURCE_MS)
{
	_frameNum = frameNum;
	((Sprite*)(*_sprites))->setAutoSwitchFrame(false);
}


AnimatedPretties::~AnimatedPretties()
{
}


void AnimatedPretties::draw()
{
	(*_sprites)->render(0, _position,
						ANGLE_NO, SCALE_NO, TRANSLATE_NO, DrawCenter::BOTTOMLEFT,
						COLOR_NO, _index, _index + _frameNum);
}


void AnimatedPretties::update()
{

}