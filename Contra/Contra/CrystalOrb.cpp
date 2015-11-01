#include "CrystalOrb.h"


CrystalOrb::CrystalOrb(D3DXVECTOR3 position, Object* target)
{
	setBasicProperties();

	setPos(position);

	AutoAimingObject::setAimingProperties(target, CRYSTAL_AIMING_MEASURE);

	Object::loadObjectResource(CRYSTAL_RESOURCE_LINK.c_str(), CRYSTAL_RESOURCE_ROWS, CRYSTAL_RESOURCE_COLUMNS, RESOURCE_MS);
}


CrystalOrb::~CrystalOrb()
{
}

void CrystalOrb::setBasicProperties()
{
	MetalOrb::setBasicProperties();
	ShootObject::setBasicProperties();
	AutoAimingObject::setBasicProperties();
	_currentBullet = BulletType::FIRE_CIRCLE;
	_timeCounter = 0;
	_heathPoints = CRYSTAL_LIFE_POINTS;
}

int CrystalOrb::computeShootAngle()
{
	return aim();
}

D3DXVECTOR3 CrystalOrb::computeShootPoint()
{
	return _position;
}

void CrystalOrb::update()
{
	Mobs::update();
}

void CrystalOrb::draw()
{
	MetalOrb::draw();
}

bool CrystalOrb::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

bool CrystalOrb::isTerminated()
{
	return isDead();
}

void CrystalOrb::activateUpdate()
{
	ShootObject::update();
	BulletManager::updateIfObjectIsShooted(this, KEY_NO);

	_timeCounter += Timer::getRoundTime();
	if (_timeCounter >= CRYSTAL_SHOOT_DELAY)
	{
		instanceShoot();
		_timeCounter = 0;
	}
}