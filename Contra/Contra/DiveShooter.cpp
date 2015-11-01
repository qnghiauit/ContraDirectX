#include "DiveShooter.h"


DiveShooter::DiveShooter(D3DXVECTOR3 position, Object* target)
{
	setBasicProperties();
	_position = position;
	_target = target;
	Object::loadObjectResource(
		DIVE_SHOOTER_RESOURCE_LINK.c_str(), 
		DIVE_SHOOTER_RESOURCE_ROWS,
		DIVE_SHOOTER_RESOURCE_COLUMNS,
		RESOURCE_MT);
	*_objectDefRect = Default::createRectFromBottomCenter(_position, DIVE_SHOOTER_BOUND_SIZE);
}


DiveShooter::~DiveShooter()
{
}

unsigned int DiveShooter::getScore()
{
	return SCORE_DIVE_SHOOTER;
}

void DiveShooter::draw()
{
	if (!isDead())
	{
		((MergedTexture*)(*_sprites))->draw(_status, _position, DrawCenter::BOTTOMCENTER, SCALE_NO, _translate);
	}
}

void DiveShooter::waitUpdate()
{
	if (Default::checkIfPointInRect(getPosition(), Camera::getViewPort()) && 
		_target->getPosition().y > (getPosition().y + DIVE_SHOOTER_GOOD_DISTANCE_TO_TARGET_Y))
	{
		activate();
	}
}

void DiveShooter::activateUpdate()
{
	_timer += Timer::getRoundTime();

	switch (_status)
	{
	case DiveShooterState::DIVE_SHOOTER_DIVING:
		if (_timer >= DIVE_SHOOTER_RELOAD_TIME)
		{
			_timer = 0;
			_status = DiveShooterState::DIVE_SHOOTER_SHOOT;
		}
		break;

	case DiveShooterState::DIVE_SHOOTER_SHOOT:

		BulletManager::updateIfObjectIsShooted(this, KEY_NO);

		if (_timer >= DEFAULT_FRAME_TIME && _timer - Timer::getRoundTime() < DEFAULT_FRAME_TIME)
		{
			RedSpreadBoom* bullet = reinterpret_cast<RedSpreadBoom*>(shoot());
			bullet->assignTarget(_target);
		}

		if (_timer >= DIVE_SHOOTER_SHOOT_TIME)
		{
			_timer = 0;
			_status = DiveShooterState::DIVE_SHOOTER_DIVING;
		}
		break;

	default:
		break;
	}
}

void DiveShooter::deadUpdate()
{
}

bool DiveShooter::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

void DiveShooter::setBasicProperties()
{
	Mobs::setBasicProperties();
	ShootObject::setBasicProperties();
	_translate = D3DXVECTOR2(0.0F, 1.0F);
	_currentBullet = BulletType::RED_SPREAD_BOOM;
}

void DiveShooter::deadEffect()
{
	EffectManager::addSmallExplode(_position);
}

bool DiveShooter::isDead()
{
	return Mobs::isDead();
}

void DiveShooter::update()
{
	Mobs::update();
	ShootObject::update();
	computeTranslate();
}

bool DiveShooter::isTerminated()
{
	return (isDead() || 
			(!Default::checkIfBounded(_objectDefRect, &Camera::getViewPort()) &&
			(_position.x < Camera::getViewPort().left ||
			_position.y < Camera::getViewPort().bottom)));
}

int DiveShooter::computeShootAngle()
{
	return ANGLE_COUNTER_CLOCKWISE(90);
}

D3DXVECTOR3 DiveShooter::computeShootPoint()
{
	return (_position + DIVE_SHOOTER_SHOOTPOINT_DISTANCE_FROM_POSITION);
}


void DiveShooter::computeTranslate()
{
	_frameTime += Timer::getRoundTime();

	if (_frameTime >= DIVE_SHOOTER_FRAME_TIME)
	{
		_translate.y = -_translate.y;
		_frameTime = 0;
	}
}