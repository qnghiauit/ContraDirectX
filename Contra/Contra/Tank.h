#ifndef __TANK_H__
#define __TANK_H__

#include "Mobs.h"
#include "TankGun.h"
#include "MoveObject.h"

#define TANK_RESOURCE_LINK	(tstring(_T("Resource//Mobs//Tank//Tank")) + EXTENSION_PICTURE)
#define TANK_RESOURCE_ROWS	2
#define TANK_RESOURCE_COLUMNS	2
#define SCORE_TANK	1000
#define TANK_GUN_POS_FIX(scaleX)	(D3DXVECTOR3(24.0F * scaleX, 57.0F, 0.0F))
#define TANK_FADE_COLORS	DWORD colors[] = {0xFFE44434, 0xFFFF8C7C, 0xFFFFAC4C, 0xFF808080}
#define TANK_NUM_OF_COLORS	4
#define TANK_LIFE_POINTS	200
#define TANK_BOUND_SIZE		Size(96.0F, 65.0F)
#define TANK_MOVE_SPEED(scaleX)		D3DXVECTOR3(30.0F * scaleX, 0.0F, 0.0F)
#define TANK_MOVE_DISTANCE	(RESOLUTION_WIDTH / 2)
#define TANK_MOVE_TIME		(TANK_MOVE_DISTANCE / TANK_MOVE_SPEED(1).x * 1000)
#define TANK_STAND_TIME		8000 // 8s

enum TankStatus
{
	TANK_MOVE = 0,
	TANK_STAND
};

class Tank :
	public Mobs,
	public MoveObject
{
public:
	Tank(D3DXVECTOR3 pos, Object* target);
	virtual ~Tank();

	virtual unsigned int getScore();

	virtual void draw();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual bool affect(Object* obj, DWORD key);

	virtual void setBasicProperties();

	virtual void deadEffect();

	virtual void update();

	virtual bool isTerminated();

private:
	TankGun* _gun;
	int _drawIndex;
	DWORD _timeCounter;
};

#endif // Tank_h__
