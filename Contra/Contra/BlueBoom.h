#ifndef __BLUE_BOOM__
#define __BLUE_BOOM__

#include "MoveObject.h"
#include "Mobs.h"
#include "EffectManager.h"

#define BLUE_BOOM_RESOURCE_LINK	(tstring(_T("Resource//Miscellaneous//BlueBoom")) + EXTENSION_PICTURE)
#define BLUE_BOOM_RESOURCE_ROWS	1
#define BLUE_BOOM_RESOURCE_COLUMNS	1
#define BLUE_BOOM_EXPLODE_TIME	(EFFECT_EXPLODE_SMALL_FRAME_TIME * EFFECT_EXPLODE_SMALL_COLUMNS * EFFECT_EXPLODE_SMALL_ROWS)
#define BLUE_BOOM_EXPLODE_RANGE	Size(32.0f, 32.0f)
#define BLUE_BOOM_THROW_SIZE	Size(16.0f, 16.0f)
#define BLUE_BOOM_ROOL_SPEED	1440.0f
#define BLUE_BOOM_START_VELO	D3DXVECTOR3(40.0f, 150.0f, 0.0f)

enum BlueBoomState
{
	BLUE_BOOM_THROW = 0,
	BLUE_BOOM_EXPLODE
};

class BlueBoom :
	public MoveObject,
	public Mobs
{
public:
	BlueBoom(D3DXVECTOR3 pos);
	virtual ~BlueBoom();

	virtual void update();

	virtual void draw();

	virtual bool isTerminated();

	virtual bool affect(Object* obj, DWORD key);

	virtual void setBasicProperties();

	virtual unsigned int getScore();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual void activate();

	virtual void deadEffect();

private:
	float _currentAngle;
	DWORD _timeCounter;
};

#endif // BlueBoom_h__
