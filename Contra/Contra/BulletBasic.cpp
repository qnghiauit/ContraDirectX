#include "BulletBasic.h"

BulletBasic::BulletBasic(int ally)
{
	Bullet::setBasicProperties();
	_bulletRS = BulletRS::AMMO;
	_damage = BULLET_DAMAGE_BASIC;

	if (ally == ALLY_PLAYER)
		_type = BulletType::BASIC;
	else
		_type = BulletType::MOBS;
}


BulletBasic::~BulletBasic()
{
}


BulletType BulletBasic::getType()
{
	return _type;
}


void BulletBasic::shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	setProperties(angle, ally, scaleX);

	D3DXVECTOR3 velo = D3DXVECTOR3(BULLET_BASIC_MOVE_SPEED * Default::cos(DEGREE_TO_RADIAN(angle)),
								  BULLET_BASIC_MOVE_SPEED * Default::sin(DEGREE_TO_RADIAN(angle)),
								  0.0F);

	if (_ally == ALLY_MOBS)
	{
		velo /= 2;
	}

	shoot(velo, startPoint, ally, scaleX);
}


void BulletBasic::shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	_position = startPoint;

	setVelo(velocity);

	*_objectDefRect = Default::createRectFromCenter(_position, BULLET_SIZE_BASIC);
	managerProcessing();
}



void BulletBasic::update()
{
	MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, BULLET_SIZE_BASIC, UPDATE_XY);

	generateBullet();
}



D3DXVECTOR3 BulletBasic::generateBullet()
{
	return Object::_position;
}