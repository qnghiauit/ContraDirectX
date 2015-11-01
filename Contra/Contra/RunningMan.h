#ifndef __RUNNING_MAN_H__
#define __RUNNING_MAN_H__

#pragma warning(disable : 4250)

#include "MoveObject.h"
#include "Mobs.h"
#include "ShootObject.h"
#include "DX9Framework\DefaultMethods.h"

#define LINK_RUNNING_MAN		_T("Resource//Mobs//RunningMan//")
#define	RUNNING_MAN_MOVE_SPEED_ABSOLUTE_VALUE	MOVE_SPEED_RIGHT
#define RUNNING_MAN_RESPAWN_TIME	3000
#define RUNNING_MAN_JUMP_SPEED		(JUMP_SPEED / 2.0)
#define RUNNING_MAN_BOUND_SIZE		Size(17.0, 30.0)
#define RUNNING_MAN_RELOAD_TIME		3000
#define RUNNING_MAN_SHOOT_ANGLE_LEFT	180
#define RUNNING_MAN_SHOOT_ANGLE_RIGHT	0
#define RUNNING_MAN_SHOOT_POINT_DISTANCE_FROM_POSITION(scaleX)	D3DXVECTOR3(scaleX * 16.0F, 26.0F, 0.0f)
#define RUNNING_MAN_SHOOT_DELAY		500
#define RUNNING_MAN_SHOOT_TIME(nBullet)		(1000 + nBullet * RUNNING_MAN_SHOOT_DELAY)

enum RunningManStatus {
	RUNNING_MAN_RUN		= 0,
	RUNNING_MAN_JUMP	= 1,
	RUNNING_MAN_DEAD	= 1,
	RUNNING_MAN_SHOOT,
	NUM_OF_RM_STATUS
};

class RunningMan : public virtual MoveObject, public virtual Mobs, public virtual ShootObject
{
public:
	RunningMan(D3DXVECTOR3 position, Velocity velocity, int numOfBullet);
	~RunningMan();

	void activateUpdate();

	virtual void waitUpdate();

	virtual void deadUpdate();

	void draw();
	unsigned int getScore();

	virtual bool  affect(Object* obj, DWORD key);

	virtual void update();

	virtual bool isTerminated();

protected:
	virtual void setBasicProperties();

private:
	bool nextStepIsNotLand();

	virtual void deadEffect();

	virtual void commandJump();

	virtual vector<CollisEvent*> updateMoveableWithCollision();

	virtual void activate();

	virtual void suspend();

	virtual int computeShootAngle();

	virtual D3DXVECTOR3 computeShootPoint();

private:
	RunningManStatus _status;
	D3DXVECTOR3 _respawPosition;
	DWORD _respawnTime;
	int _bulletPool;
	int _shootedBullet;
	DWORD _shootTime;

};

#endif