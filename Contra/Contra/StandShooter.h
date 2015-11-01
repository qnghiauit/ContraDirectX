#ifndef __STAND_SHOOTER_H__
#define __STAND_SHOOTER_H__

#include "Mobs.h"
#include "ShootObject.h"

#define STAND_SHOOTER_RESOURCE_LINK	(tstring(_T("Resource//Mobs//StandShooter")) + EXTENSION_PICTURE)
#define STAND_SHOOTER_RESOURCE_ROWS	1
#define STAND_SHOOTER_RESOURCE_COLUMNS	2
#define STAND_SHOOTER_SHOOTPOINT_FIX	D3DXVECTOR3(16.0F, 24.0F, 0.0F)
#define SCORE_STAND_SHOOTER		500
#define STAND_SHOOTER_SHOOT_DELAY	3000	// 3s
#define STAND_SHOOTER_LIFE_POINTS	20
#define STAND_SHOOTER_BOUND_SIZE	Size(32.0F, 32.0F)
#define STAND_SHOOTER_CANON_TRANSLATES	D3DXVECTOR2 translate[] = {D3DXVECTOR2(0.0F, 0.0F), \
																   D3DXVECTOR2(2.0F, 0.0F)}

class StandShooter :
	public Mobs,
	public ShootObject
{
public:
	StandShooter(D3DXVECTOR3 pos);
	virtual ~StandShooter();

	virtual unsigned int getScore();

	virtual void draw();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual bool affect(Object* obj, DWORD key);

	virtual void setBasicProperties();

	virtual void deadEffect();

	virtual int computeShootAngle();

	virtual D3DXVECTOR3 computeShootPoint();

	virtual void update();

	virtual bool isTerminated();

private:
	int _currentCanonTranslate;
};

#endif // StandShooter_h__
