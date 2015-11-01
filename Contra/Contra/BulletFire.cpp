#include "BulletFire.h"


BulletFire::BulletFire()
{
	Bullet::setBasicProperties();
	_bulletRS = BulletRS::FIRE;
	_damage = BULLET_DAMAGE_F;
}


BulletFire::~BulletFire()
{
}


BulletType BulletFire::getType()
{
	return BulletType::F;
}


void BulletFire::shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	setProperties(angle, ally, scaleX);

	this->_selfRotateDegree = angle + ANGLE_COUNTER_CLOCKWISE(180);

	shoot(D3DXVECTOR3(BULLET_F_MOVE_SPEED * Default::cos(DEGREE_TO_RADIAN(angle)),
						BULLET_F_MOVE_SPEED * Default::sin(DEGREE_TO_RADIAN(angle)),
						0.0F),
			startPoint,
			ally,
			scaleX);
}


void BulletFire::shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	startPoint = CircleEquation(startPoint.x, startPoint.y, BULLET_F_ROUND_RANGE).getCoordinatesFromDegree(_shootAngle);
	setVelo(velocity);

	_centerPos = startPoint;
	_position = POSITION_ZERO;

	generateBullet();
	managerProcessing();
}



void BulletFire::update()
{
	Default::swap(_centerPos, _position);
	MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, BULLET_SIZE_F, UPDATE_XY);
	Default::swap(_centerPos, _position);

	generateBullet();
}



D3DXVECTOR3 BulletFire::generateBullet()
{
	_position = CircleEquation(_centerPos.x, _centerPos.y, BULLET_F_ROUND_RANGE).getCoordinatesFromDegree(_selfRotateDegree);

	_selfRotateDegree += (int)(_scaleX * TRUE_SPEED(BULLET_F_ROUND_SPEED, (float)Timer::getRoundTime()));
	_selfRotateDegree = _selfRotateDegree % ANGLE_DEGREE_LIM;

	*Object::_objectDefRect = Default::createRectFromCenter(_position, BULLET_SIZE_F);

	return _position;
}

void BulletFire::draw()
{
	if (_rendered)
		(*_sprites)->render(Timer::getRoundTime(), _position, ANGLE_NO, SCALE_NO,
		TRANSLATE_NO, DrawCenter::CENTER, COLOR_NO, _bulletRS, _bulletRS);

#if GAMEMODE == GAMEMODE_FLASH_BULLET
	_rendered = !_rendered;
#endif
}
