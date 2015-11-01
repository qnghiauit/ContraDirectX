#ifndef __SNIPER_H__
#define __SNIPER_H__

#include "ShootObject.h"
#include "AutoAimingObject.h"
#include "Mobs.h"


#define LINK_SNIPER					_T("Resource//Mobs//Sniper//")
#define SNIPER_SHOOT_DISTANCE (double)(RESOLUTION_WIDTH)
#define SNIPER_MAX_BULLET_IN_A_ROW	3
#define SNIPER_AIMING_MEASURE	22.5

enum SniperShootSubStatus
{
	SNIPER_STRAIGHT = 0,
	SNIPER_UP_45,
	SNIPER_DOWN_45,
	NUM_OF_SNIPER_STATUS
};


class Sniper :	virtual public ShootObject,
				virtual public Mobs,
				virtual public AutoAimingObject
{
public:
	Sniper(D3DXVECTOR3 position, Object* target);
	~Sniper();

	virtual void setBasicProperties();

	virtual void update();

	virtual void draw();

	virtual int computeShootAngle();

	virtual D3DXVECTOR3 computeShootPoint();

	virtual unsigned int getScore();

	virtual void activateUpdate();

	virtual void waitUpdate();

	virtual void deadUpdate();

private:
	vector<Bullet*> _shootedBullets;

private:
	virtual void deadEffect();

	virtual bool  affect(Object* obj, DWORD key);

	virtual bool isTerminated();

};

#include "Character.h"

#endif // Sniper_h__
