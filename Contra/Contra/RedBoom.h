#ifndef __RED_BOOM_H__
#define __RED_BOOM_H__

#include "Bullet.h"
#include "EffectManager.h"
#include "Tile.h"

#define RED_BOOM_DEFAULT_MOVE_SPEED	300.0F

class RedBoom :
	public Bullet
{
public:
	RedBoom();
	virtual ~RedBoom();

	virtual void shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX);

	virtual void shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX);

	virtual D3DXVECTOR3 generateBullet();

	virtual void update();

	virtual BulletType getType();
			
			void assignTarget(Object* target);

	virtual bool isTerminated();

	virtual void bulletEffect();

	virtual void bulletEffect(D3DXVECTOR3 effectPos);

private:
	Object* _target;
	bool _isTerminated;
};

#endif // RedBoom_h__
