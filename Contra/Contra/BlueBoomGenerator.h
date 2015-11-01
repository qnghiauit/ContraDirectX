#ifndef __BLUE_BOOM_GENERATOR_H__
#define __BLUE_BOOM_GENERATOR_H__

#include "Object.h"
#include "BlueBoom.h"
#include "Map.h"

#define BB_GENERATOR_DELAY	2000

class BlueBoomGenerator :
	public Object
{
public:
	BlueBoomGenerator(D3DXVECTOR3 pos);
	virtual ~BlueBoomGenerator();

	virtual void setBasicProperties();

	virtual void update();

	virtual void draw();

	virtual bool affect(Object* obj, DWORD key);

	virtual bool isTerminated();

private:
	DWORD _timeCounter;
};

#endif // BlueBoomGenerator_h__
