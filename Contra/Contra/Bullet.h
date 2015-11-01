#ifndef __BULLET_H__
#define __BULLET_H__

#include "GameDefaultStructure.h"
#include "MoveObject.h"
#include "FreeSound.h"

#include "DX9Framework\MergedTexture.h"

#include "GraphicAlgorimths2D\CircleEquation.h"

class BulletManager;
class Mobs;
class Character;


class Bullet : public MoveObject
{
public:
	virtual void			shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)	= 0;
	virtual void			shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX) = 0;
	virtual D3DXVECTOR3		generateBullet()													= 0;

	virtual	void			update()															= 0;
	virtual void			increaseVelocity(int bonusSpeed);

	virtual BulletType		getType() = 0;

	virtual bool			isTerminated();
	virtual void			bulletEffect();
	bool					isLiving();
	void					setLivingStatus(bool newLivingStatus);
	virtual void			draw();
	void					setAlly(int newAlly);
	void					setProperties(int angle, int ally, float scaleX);
	int						getDamage();

			void			assignManager(BulletManager& manager);
			void			managerProcessing();
			int				getShootAngle();

protected:
	void					setBasicProperties();

	virtual bool			affect(Object* obj, DWORD key);

protected:
	int			_shootAngle;
	int			_damage;
	int			_bulletRS;
	BYTE		_ally;
	float		_scaleX;
	bool		_rendered;
	bool		_isLiving;

	BulletManager*	_manager;

	friend		BulletManager;
};
#endif