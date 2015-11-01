#ifndef __FIRE_CIRCLE_H__
#define __FIRE_CIRCLE_H__

#include "Bullet.h"
class BulletManager;

#define BULLET_FIRE_CIRCLE_MOVE_SPEED	150
#define BULLET_SIZE_FIRE_CIRCLE	Size(16.0F, 16.0F)
#define BULLET_FIRE_CIRCLE_GYRATE_MEASURE	ANGLE_CLOCKWISE(45)

class FireCircle :
	public Bullet
{
public:
	void			shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX);

	void			update();
	D3DXVECTOR3		generateBullet();
	BulletType		getType();

private:
	virtual void shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX);

	FireCircle();
	virtual ~FireCircle();

	virtual void draw();

	friend BulletManager;

	int _seftGyrateAngle;
};

#endif // FireCircle_h__