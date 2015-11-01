#ifndef __BULLET_MACHINE__
#define __BULLET_MACHINE__

#include "Bullet.h"

#define BULLET_M_SOUND			_T("Resource//SoundTrack//M.wav")

class BulletMachine : public Bullet
{
public:
	void			shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX);

	void			update();
	D3DXVECTOR3		generateBullet();
	BulletType		getType();

private:
	virtual void shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX);

	BulletMachine();
	BulletMachine(int ally);
	~BulletMachine();
	friend BulletManager;
};
#endif