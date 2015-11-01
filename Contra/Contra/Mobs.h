#ifndef __MOBS_H__
#define __MOBS_H__

#include "DestroyableObject.h"
#include "BulletManager.h"

enum MobTypes
{
	RUNNING_MAN = 0,
	STAND_CANON,
	CANON,
	SNIPER,
	HIDDEN_SNIPER,
	DIVE_SHOOTER,
	STAND_SHOOTER,
	TANK,
	NUM_OF_MOB_TYPES
};

enum MobState
{
	MOBS_STATE_WAIT = 0,
	MOBS_STATE_ACTIVATE,
	NUM_OF_MOB_STATES
};

class Character;

class Mobs : public virtual DestroyableObject
{
public:
	virtual unsigned int getScore() = 0;

	virtual		 ~Mobs(){};
			void update();
	virtual void draw() = 0;
	virtual void waitUpdate() = 0;
	virtual void activateUpdate() = 0;
	virtual void deadUpdate() = 0;

	virtual void suspend();
	virtual void activate();

			void compulsoryDead();

	virtual bool  affect(Object* obj, DWORD key);
			bool isActivated();

	virtual void updateAsBossSubpart();

protected:
	virtual void setBasicProperties();

protected:
	MobState	_mobState;

private:
	virtual void deadEffect() = 0;


};

#endif