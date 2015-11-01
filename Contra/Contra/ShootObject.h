#ifndef __SHOOT_OBJECT_H__
#define __SHOOT_OBJECT_H__

#include "Object.h"
#include "Bullet.h"
#include "BulletManager.h"
#include "GameDefaultConstant.h"
#include "GameDefaultStructure.h"

#include "GraphicAlgorimths2D\Math.h"
#include "GraphicAlgorimths2D\MathDefaultConstant.h"

class Bullet;
class BulletManager;

class ShootObject : public virtual Object
{
public:
			void		draw()				= 0;
	virtual int			computeShootAngle() = 0;
	virtual D3DXVECTOR3 computeShootPoint() = 0;
	virtual bool		isTerminated()		= 0;
	virtual				~ShootObject(){};

	virtual void	update();
	virtual Bullet*	shoot(D3DXVECTOR3 velocity = SPEED_NO);
	Bullet* instanceShoot(D3DXVECTOR3 velocity = SPEED_NO);
	virtual void	setBasicProperties();
			void	changeBullet(BulletType newBullet);
			int		getShootDirection(D3DXVECTOR3* target);

			virtual bool  affect(Object* obj, DWORD key);

protected:
	BulletType		_currentBullet;
	double			_lastShootTime;
};

#endif