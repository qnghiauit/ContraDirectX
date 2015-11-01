#include "BulletMachine.h"

BulletMachine::BulletMachine()
{
	Bullet::setBasicProperties();
	_bulletRS = BulletRS::MACHINE;
	_damage = BULLET_DAMAGE_M;
}

BulletMachine::BulletMachine(int ally)
{
	Bullet::setBasicProperties();
	_bulletRS = BulletRS::MACHINE;
	_damage = BULLET_DAMAGE_M;
	this->_ally = ally;
}


BulletMachine::~BulletMachine()
{
}


BulletType BulletMachine::getType()
{
	return BulletType::M;
}


void BulletMachine::shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	setProperties(angle, ally, scaleX);

	shoot(D3DXVECTOR3(BULLET_M_MOVE_SPEED * Default::cos(DEGREE_TO_RADIAN(angle)),
					  BULLET_M_MOVE_SPEED * Default::sin(DEGREE_TO_RADIAN(angle)),
					  0.0F),
					  startPoint,
					  ally,
					  scaleX);
}


void BulletMachine::shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	setVelo(velocity);

	_position = startPoint;

	*_objectDefRect = Default::createRectFromCenter(_position, BULLET_SIZE_M);

	FreeSound::play(BULLET_M_SOUND);

	managerProcessing();
}



void BulletMachine::update()
{
	MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, BULLET_SIZE_M, UPDATE_XY);

	generateBullet();
}



D3DXVECTOR3 BulletMachine::generateBullet()
{
	return Object::_position;
}