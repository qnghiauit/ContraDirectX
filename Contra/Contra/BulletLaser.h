#ifndef __BULLET_LASER__
#define __BULLET_LASER__

#define GENERATE_TIME	30

#include "Bullet.h"

#define BULLET_L_MOVE_SPEED		300
#define BULLET_L_LATE_START		(-12)
#define BULLET_L_ACTIVATE_TIME	DWORD activateTime[] = {0, 0, 15, 30}
#define BULLET_L_SOUND			_T("Resource//SoundTrack//L.wav")

class BulletLaser : public Bullet
{
public:
	void			shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX);

	virtual void shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX);

	void			update();
	D3DXVECTOR3		generateBullet();
	BulletType		getType();

	virtual void draw();

private:
	BulletLaser();
	~BulletLaser();

	void	removeBulletLaserInManager();
	void	singleBulletFire(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX);
	bool	checkIfActivated();

	friend BulletManager;

private:
	bool _isGenerated;
	float _movedDistance;
	static int _laserCounter;
};
#endif