#ifndef __FIRE_H__
#define __FIRE_H__

#include "Mobs.h"
#include "MoveObject.h"

#define FIRE_RESOURCE_LINK	(tstring(_T("Resource//Mobs//Fire")) + EXTENSION_PICTURE)
#define FIRE_RESOURCE_ROWS	1
#define FIRE_RESOURCE_COLUMNS	1
#define FIRE_FRAME_TIME		DEFAULT_FRAME_TIME
#define FIRE_BOUND_SIZE		Size(16.0, 16.0)

class Fire :
	public Mobs,
	public MoveObject
{
public:
	Fire(D3DXVECTOR3 position, D3DXVECTOR3 velocity, RECT moveRange);
	virtual ~Fire();

	virtual void draw();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual bool affect(Object* obj, DWORD key);

	virtual void setBasicProperties();

	virtual void update();

	virtual bool isTerminated();

	virtual unsigned int getScore();

	virtual void deadEffect();

private:
	RECT _moveRange;
	DWORD _frameTime;

};

#endif // Fire_h__
