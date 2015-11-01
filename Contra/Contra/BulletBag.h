#ifndef __BULLET_BAG_H__
#define __BULLET_BAG_H__

#include "DestroyableObject.h"
#include "BulletFalcon.h"

#define BULLET_BAG_SIZE	Size(32.0, 32.0)
#define BULLET_BAG_LINK	(tstring(_T("Resource//Miscellaneous//BulletBag")) + EXTENSION_PICTURE)
#define BLENDED_COLOR_LIST	DWORD color[] = { 0xFFFF8C7C, 0xFFE44434, 0xFFB81C0C, 0xFFE44434 }
#define NUM_OF_BLENDED_COLORS	4
#define BULLET_BAG_RESOURCE_ROWS	3
#define BULLET_BAG_RESOURCE_COLUMNS	2
#define BULLET_BAG_SWITCH_COLOR_TIME	DEFAULT_FRAME_TIME

#define BULLET_BAG_CLOSE_TIME	3000
#define BULLET_BAG_OPENING_TIME	BULLET_BAG_SWITCH_COLOR_TIME
#define BULLET_BAG_OPEN_TIME	2000

class Map;

enum BulletBagStatus
{
	BULLET_BAG_CLOSE = 0,
	BULLET_BAG_OPENING,
	BULLET_BAG_OPEN,
	NUM_OF_BULLET_BAG_STATUS
};

class BulletBag :
	public DestroyableObject
{
public:
	BulletBag();
	BulletBag(D3DXVECTOR3 position, BulletFalconType containFalconType);
	virtual ~BulletBag();

	virtual void update();

	virtual void setBasicProperties();

	virtual void draw();

	virtual void deadEffect();

	virtual bool  affect(Object* obj, DWORD key);

			bool checkIfTimeCounted(DWORD time);

			virtual bool isTerminated();

protected:
	BulletFalconType _containFalconType;

private:
	int _bagColor;
	DWORD _lightTime;
	bool _isOpening;
	DWORD _counter;
	bool _isTerminated;
};

#endif // BulletBag_h__
