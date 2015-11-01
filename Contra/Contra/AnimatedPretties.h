#ifndef __ANIMATED_PRETTIES_H__
#define __ANIMATED_PRETTIES_H__

#include "Pretties.h"

class AnimatedPretties : virtual public Pretties
{
public:
	AnimatedPretties(tstring path, index_t index, size_t rows, D3DXVECTOR3 position, int frameNum);
	~AnimatedPretties();

	void draw();
	void update();

private:
	int _frameNum;
};

#endif