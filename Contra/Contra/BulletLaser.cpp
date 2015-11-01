#include "BulletLaser.h"
#include "BulletManager.h"

int BulletLaser::_laserCounter = 0;

BulletLaser::BulletLaser()
{
	Bullet::setBasicProperties();
	_bulletRS = BulletRS::LAZER;
	_damage = BULLET_DAMAGE_L;
}


BulletLaser::~BulletLaser()
{
}


BulletType BulletLaser::getType()
{
	return BulletType::L;
}


void BulletLaser::shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	_laserCounter = 0;

	setProperties(angle, ally, scaleX);

	FreeSound::play(BULLET_L_SOUND);

	(_manager->getPool(getType())[_manager->getCounter(getType())])->shoot(
		D3DXVECTOR3(BULLET_L_MOVE_SPEED * Default::cos(DEGREE_TO_RADIAN(angle)),
					BULLET_L_MOVE_SPEED * Default::sin(DEGREE_TO_RADIAN(angle)),
					0.0F), 
					startPoint, 
					ally, 
					scaleX);
}


void BulletLaser::shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	this->setVelo(velocity);
	startPoint.x += BULLET_L_LATE_START * Default::cos(DEGREE_TO_RADIAN(_shootAngle));
	startPoint.y += BULLET_L_LATE_START * Default::sin(DEGREE_TO_RADIAN(_shootAngle));

	_position = startPoint;
	_isGenerated = false;
	_movedDistance = 0.0F;

	generateBullet();

	managerProcessing();
}



void BulletLaser::update()
{
	MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, BULLET_SIZE_L, UPDATE_XY);

	_movedDistance += TRUE_SPEED(BULLET_L_MOVE_SPEED, Timer::getRoundTime());

	if (_movedDistance >= abs(BULLET_L_LATE_START) && !_isGenerated)
	{
		_isGenerated = true;
		if (_manager->getCounter(getType()) >= 0 && _laserCounter < (BULLET_MAX_L - 1))
		{
			_laserCounter++;
			D3DXVECTOR3 startPoint = getPosition();

			if (!isTerminated())
				startPoint.x += BULLET_L_LATE_START * Default::cos(DEGREE_TO_RADIAN(_shootAngle));
			startPoint.y += BULLET_L_LATE_START * Default::sin(DEGREE_TO_RADIAN(_shootAngle));

			Bullet* shootedOne = _manager->getPool(getType())[_manager->getCounter(getType())];
			shootedOne->setProperties(_shootAngle, _ally, _scaleX);
			shootedOne->shoot(*getVelo(), startPoint, _ally, _scaleX);
		}
		else
			_laserCounter = 0;
	}
}



D3DXVECTOR3 BulletLaser::generateBullet()
{
	return Object::_position;
}

void BulletLaser::draw()
{
	if (_rendered)
		(*_sprites)->render(Timer::getRoundTime(), _position,
		(float)_shootAngle + (_shootAngle % 90 != 0 ?
		ANGLE_COUNTER_CLOCKWISE(90) + 2 * (ANGLE_COUNTER_CLOCKWISE(45) - _shootAngle % 90) :
		0),
		SCALE_XY(_scaleX, 1.0F),
		TRANSLATE_NO, DrawCenter::CENTER, COLOR_NO, _bulletRS, _bulletRS);

#if GAMEMODE == GAMEMODE_FLASH_BULLET
	_rendered = !_rendered;
#endif
}