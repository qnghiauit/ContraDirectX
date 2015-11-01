#include "FireCircle.h"
#include "BulletManager.h"

void FireCircle::shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	setProperties(angle, ally, scaleX);

	shoot(D3DXVECTOR3(BULLET_FIRE_CIRCLE_MOVE_SPEED * Default::cos(DEGREE_TO_RADIAN(angle)),
		BULLET_FIRE_CIRCLE_MOVE_SPEED * Default::sin(DEGREE_TO_RADIAN(angle)),
		0.0F),
		startPoint,
		ally,
		scaleX);
}

void FireCircle::shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	_seftGyrateAngle = 0;

	setVelo(velocity);

	_position = startPoint;

	*_objectDefRect = Default::createRectFromCenter(_position, BULLET_SIZE_FIRE_CIRCLE);

	managerProcessing();
}

void FireCircle::update()
{
	MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, BULLET_SIZE_FIRE_CIRCLE, UPDATE_XY);
	_seftGyrateAngle += BULLET_FIRE_CIRCLE_GYRATE_MEASURE;
	generateBullet();
}

D3DXVECTOR3 FireCircle::generateBullet()
{
	return Object::_position;
}

BulletType FireCircle::getType()
{
	return BulletType::FIRE_CIRCLE;
}

FireCircle::FireCircle()
{
	Bullet::setBasicProperties();
	_bulletRS = BulletRS::ENERGY_FIRE;
	_damage = BULLET_DAMAGE_MASSIVE;
}

FireCircle::~FireCircle()
{

}

void FireCircle::draw()
{
	if (_rendered)
		(*_sprites)->render(Timer::getRoundTime(), _position, _seftGyrateAngle, SCALE_NO,
		TRANSLATE_NO, DrawCenter::CENTER, COLOR_NO, _bulletRS, _bulletRS);
}
