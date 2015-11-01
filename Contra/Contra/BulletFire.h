#ifndef __BULLET_FIRE_H__
#define __BULLET_FIRE_H__

#include "Bullet.h"

class BulletFire : public Bullet
{
public:
	void			shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX);

	void			update();
	D3DXVECTOR3		generateBullet();
	BulletType		getType();

	virtual void draw();


private:
	virtual void shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX);

	BulletFire();
	~BulletFire();


	friend BulletManager;

private:
	int				_selfRotateDegree;
	D3DXVECTOR3		_centerPos;
};

#endif