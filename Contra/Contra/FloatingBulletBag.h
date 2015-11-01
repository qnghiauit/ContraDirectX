#ifndef __FLOATING_BULLET_BAG_H__
#define __FLOATING_BULLET_BAG_H__

#include "DestroyableObject.h"
#include "MoveObject.h"
#include "BulletFalcon.h"
#include "Map.h"

#define FLOATING_BULLET_BAG_SIZE				Size(24.0, 16.0)
#define FLOATING_BULLET_BAG_LINK				(tstring(_T("Resource//Miscellaneous//FloatingBulletBag")) + EXTENSION_PICTURE)
#define FLOATING_BULLET_BAG_RESOURCE_ROWS		1
#define FLOATING_BULLET_BAG_RESOURCE_COLUMNS	1
#define FLOATING_BULLET_REDIRECTING_SPEED		(360.0)
#define FLOATING_BULLET_REDIRECTING_CAP			(100.0)
#define FLOATING_BULLET_BAG_MOVE_SPEED			(DEFAULT_MOVE_SPEED)

enum FloatingBulletBagStatus
{
	FBS_WAIT = 0,
	FBS_FLY
};

#define AXIS_X 0
#define AXIS_Y 1

class FloatingBulletBag : public virtual DestroyableObject
						, public virtual MoveObject
{
public:
	FloatingBulletBag(D3DXVECTOR3 position, BulletFalconType containFalconType, D3DXVECTOR3 velocity);
	virtual ~FloatingBulletBag();

	virtual void update();

	virtual void setBasicProperties();

	virtual void draw();

	virtual void deadEffect();

	virtual bool affect(Object* obj, DWORD key);

	virtual bool isTerminated();

			D3DXVECTOR3 computeVelocity();

private:
	BulletFalconType _containFalconType;
	bool _isTerminated;
	int _currentDirectAngle;
	FloatingBulletBagStatus _bagState;
	int _flyStraightAxis;
};

#endif // FloatingBulletBag_h__
