#include "ShootObject.h"

Bullet* ShootObject::shoot(D3DXVECTOR3 velocity)
{
	static RELOAD_TIME;
	if (_lastShootTime >= reload[_currentBullet]) {

		_lastShootTime = Timer::getRoundTime();

		if (velocity == SPEED_NO)
		{
			return BulletManager::getInstance().shoot(_currentBullet, ALLY_MOBS, computeShootPoint(), computeShootAngle(), _scale.x);
		}
		else
		{
			return BulletManager::getInstance().shoot(_currentBullet, ALLY_MOBS, computeShootPoint(), velocity, _scale.x);
		}
	}

	return nullptr;
}


void ShootObject::changeBullet(BulletType newBullet)
{
	_currentBullet = newBullet;
}



void ShootObject::update()
{
	static RELOAD_TIME;
	if (_lastShootTime < reload[_currentBullet]) {
		_lastShootTime += Timer::getRoundTime();
	}
}



void ShootObject::setBasicProperties()
{
	Object::setBasicProperties();
	_currentBullet = BulletType::BASIC;
	_lastShootTime = 0;
}


int ShootObject::getShootDirection(D3DXVECTOR3* target)
{
	return (int)RADIAN_TO_DEGREE(Math::caculateLineAngle(_position, *target));
}

bool ShootObject::affect(Object* obj, DWORD key)
{
	return false;
}

Bullet* ShootObject::instanceShoot(D3DXVECTOR3 velocity /*= SPEED_NO*/)
{
	static RELOAD_TIME;
	_lastShootTime = reload[(_currentBullet)];
	return shoot(velocity);
}
