#ifndef __BULLET_BASIC_H__
#define __BULLET_BASIC_H__

#include "Bullet.h"

class BulletBasic : public Bullet
{	
public:
	void			shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX);


	void			update();
	D3DXVECTOR3		generateBullet();
	BulletType		getType();

private:
	virtual void shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX);

	BulletBasic(int ally);
	~BulletBasic();
	friend BulletManager;
	BulletType _type;
};
#endif