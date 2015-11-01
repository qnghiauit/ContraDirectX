#ifndef __HIDDEN_SNIPER_H__
#define __HIDDEN_SNIPER_H__

#include "Mobs.h"
#include "AutoAimingObject.h"
#include "ShootObject.h"


#define HIDDEN_SNIPER_DIRECTION_MEASURE(numOfDirection)	(360 / numOfDirection)
#define HIDDEN_SNIPER_SHOOT_DISTANCE		(RESOLUTION_WIDTH)

#define LINK_HIDDEN_SNIPER					_T("Resource//Mobs//Sniper//")
#define HIDDEN_SNIPER_RESOURCE_LINK			(tstring(LINK_HIDDEN_SNIPER) + EXTENSION_PICTURE)

#define HIDDEN_SNIPER_BOUND_SIZE			Size(10.0, 28.0)
#define HIDDEN_SNIPER_MAX_BULLET_IN_A_ROW	3
#define HIDDEN_SNIPER_RELOAD_TIME			3000
#define HIDDEN_SNIPER_SHOOT_TIME			2000
#define HIDDEN_SNIPER_ACTIVATE_TIME			(DEFAULT_FRAME_TIME * 2)

#define HS_HEIGHT_FOOT_TO_STRAIGHT_GUN		27
#define HS_HEIGHT_FOOT_TO_DOWN45_GUN		13


enum HiddenSniperStatus
{
	HIDDEN_SNIPER_STRAIGHT = 0,
	HIDDEN_SNIPER_UP45,
	HIDDEN_SNIPER_DOWN45,
	HIDDEN_SNIPER_HIDE,
	NUM_OF_HIDDEN_SNIPER_STATUS
};

class HiddenSniper
	: virtual public AutoAimingObject
	, virtual public Mobs
	, virtual public ShootObject
{
public:
	HiddenSniper(D3DXVECTOR3 position, Object* target, Size visiblePercent, int numOfShootDirection);
	virtual ~HiddenSniper();

	virtual void setBasicProperties();

	virtual void update();

	virtual void draw();

	virtual unsigned int getScore();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual void deadEffect();

private:
	bool checkCounter(DWORD countTime);

	virtual int computeShootAngle();

	virtual D3DXVECTOR3 computeShootPoint();

	virtual bool  affect(Object* obj, DWORD key);

	virtual bool isTerminated();

	virtual void updateAsBossSubpart();

	Bullet* _shootedBullet;

	Size _visiblePercent;

	int _numOfShootDirection;
};

#endif // HiddenSniper_h__
