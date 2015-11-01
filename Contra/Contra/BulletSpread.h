#ifndef __BULLET_SPREAD_H__
#define __BULLET_SPREAD_H__

#include "Bullet.h"

#define BULLET_S_MOVE_SPEED		200
#define BULLET_S_SPREAD_LEVEL	5
#define BULLET_S_SPREAD_ANGLE	int spreadAngle[] = {0, -11, 9, -21, 19}
#define BULLET_S_SOUND			_T("Resource//SoundTrack//S.wav")

class BulletSpread : public Bullet
{
public:
	void			shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX);


	void			update();
	D3DXVECTOR3		generateBullet();
	BulletType		getType();

private:
	virtual void shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX);

	BulletSpread();
	~BulletSpread();

	virtual void draw();

	virtual void increaseVelocity(int bonusSpeed);

	friend BulletManager;

private:
	int _flydistance;
	vector<Bullet*> _instances;
};

#endif