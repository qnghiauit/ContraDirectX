#ifndef __CANON_H__
#define __CANON_H__

#include "Mobs.h"
#include "ShootObject.h"
#include "AutoAimingObject.h"

#include "GraphicAlgorimths2D\MathDefaultConstant.h"
#include "GraphicAlgorimths2D\Math.h"

#define CANON_NUM_OF_DIRECTION	12
#define CANON_DIRECTION_MEASURE	(360.0 / CANON_NUM_OF_DIRECTION)
#define CANON_SHOOT_DISTANCE	(RESOLUTION_WIDTH / 3.0)
#define CANON_NUM_OF_MAP_COLOR	2
#define CANON_NUM_OF_LIGHT_COLOR	3
#define CANON_LIGHT_TIME		(DEFAULT_FRAME_TIME * 2)

#define LINK_CANON				_T("Resource//Mobs//Canon//Canon")
#define CANON_RESOURCE_LINK		(tstring(LINK_CANON) + EXTENSION_PICTURE)

#define CANON_BOUND_SIZE		Size(20.0, 20.0)
#define CANON_MAX_BULLET_IN_A_ROW	3
#define CANON_HEALTH_POINTS		10
#define CANON_RELOAD_TIME		5000
#define CANON_SHOOT_TIME		4000
#define CANON_ACTIVATE_TIME		(DEFAULT_FRAME_TIME * 2)

enum CanonStatus {
	CLOSE = 12,
	OPENING,
	NUM_OF_C_STATUS
};

class Canon : public virtual Mobs, public virtual ShootObject, public virtual AutoAimingObject
{
public:
	Canon(D3DXVECTOR3 position, Object* target);
	virtual ~Canon();

	virtual void update();
	virtual void draw();
	unsigned int getScore();
	int getShootDirection();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

protected:
	virtual void setBasicProperties();

private:
	void activate();

	virtual void deadEffect();

	virtual int computeShootAngle();

	virtual D3DXVECTOR3 computeShootPoint();

	bool rollUpdate();
	void shootUpdate();

	virtual bool  affect(Object* obj, DWORD key);

	virtual bool isTerminated();

private:
	int _shootDirection;
	int _rotateDelay;
	int _currentColor;
	int _lightTime;
	DWORD _timeCounter;
	vector<Bullet*> _shootedBullets;
	static DWORD _lightColor[];
};

#endif