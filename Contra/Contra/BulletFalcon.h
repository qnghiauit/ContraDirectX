#ifndef __BULLET_FALCON_H__
#define __BULLET_FALCON_H__

#include "MoveObject.h"
#include "BulletManager.h"

enum BulletFalconType
{
	FALCON_L = BulletType::L,
	FALCON_F = BulletType::F,
	FALCON_M = BulletType::M,
	FALCON_S = BulletType::S,
	FALCON_R,
	FALCON_B,
	FALCON_BOOM,
	NUM_OF_FALCON_TYPE = BulletType::NUM_OF_BULLET_TYPE + 1
};

#define FALCON_LINK	_T("Resource//Miscellaneous//Falcon//")
#define FALCON_SIZE Size(24.0, 15.0)


class BulletFalcon : public MoveObject
{
public:
	BulletFalcon(D3DXVECTOR3 position, BulletFalconType falconType);
	virtual ~BulletFalcon();

	virtual void update();

	virtual void draw();

	virtual bool  affect(Object* obj, DWORD key);

	virtual void setBasicProperties();

	virtual bool isTerminated();

	virtual void commandJump();

private:
	BulletFalconType _falconType;
};

#endif // BulletFalcon_h__
