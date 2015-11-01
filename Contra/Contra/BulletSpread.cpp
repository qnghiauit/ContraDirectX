#include "BulletSpread.h"
#include "BulletManager.h"

BulletSpread::BulletSpread()
{
	Bullet::setBasicProperties();
	_bulletRS = BulletRS::MACHINE;
	_damage = BULLET_DAMAGE_M;
}


BulletSpread::~BulletSpread()
{
}


BulletType BulletSpread::getType()
{
	return BulletType::S;
}


void BulletSpread::shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	static BULLET_S_SPREAD_ANGLE;
	int* counter = &_manager->getCounter(getType());
	_instances.clear();

	FOR(BULLET_S_SPREAD_LEVEL)
		if (*counter >= 0)
		{
			FreeSound::play(BULLET_S_SOUND);
			D3DXVECTOR3 velo = D3DXVECTOR3(BULLET_S_MOVE_SPEED * Default::cos(DEGREE_TO_RADIAN(angle + spreadAngle[i])),
							   			   BULLET_S_MOVE_SPEED * Default::sin(DEGREE_TO_RADIAN(angle + spreadAngle[i])),
										   0.0F);

			Bullet* shootedOne = _manager->getPool(getType())[*counter];

			shootedOne->setProperties(angle + spreadAngle[i], ally, scaleX);
			shootedOne->shoot(velo, startPoint, ally, scaleX);
			_instances.push_back(shootedOne);
		}
	else break;
}


void BulletSpread::shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	_flydistance = 0;

	_position = startPoint;

	this->setVelo(velocity);

	*_objectDefRect = Default::createRectFromCenter(_position, BULLET_SIZE_S);

	managerProcessing();
}



void BulletSpread::update()
{
	MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, BULLET_SIZE_S, UPDATE_XY);
	_flydistance += TRUE_SPEED(BULLET_S_MOVE_SPEED, Timer::getRoundTime());
	generateBullet();
}



D3DXVECTOR3 BulletSpread::generateBullet()
{
	return Object::_position;
}

void BulletSpread::draw()
{
	if (_rendered)
		(*_sprites)->render(Timer::getRoundTime(), _position, ANGLE_NO, SCALE_A(((float)_flydistance / RESOLUTION_WIDTH) / 3.0F + 0.75F),
		TRANSLATE_NO, DrawCenter::CENTER, COLOR_NO, _bulletRS, _bulletRS);
}

void BulletSpread::increaseVelocity(int bonusSpeed)
{
	FOR(_instances.size())
		_instances.at(i)->setVelo(*_instances.at(i)->getVelo() + 
		D3DXVECTOR3(bonusSpeed * Default::cos(DEGREE_TO_RADIAN(_instances.at(i)->getShootAngle())),
					bonusSpeed * Default::sin(DEGREE_TO_RADIAN(_instances.at(i)->getShootAngle())),
		0.0f));
}
