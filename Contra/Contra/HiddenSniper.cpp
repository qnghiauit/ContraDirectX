#include "HiddenSniper.h"


HiddenSniper::HiddenSniper(D3DXVECTOR3 position, Object* target, Size visiblePercent, int numOfShootDirection)
{
	setBasicProperties();
	_position = position;
	_visiblePercent = visiblePercent;
	_numOfShootDirection = numOfShootDirection;

	*_objectDefRect = Default::createRectFromBottomCenter(position, HIDDEN_SNIPER_BOUND_SIZE);
	setAimingProperties(target, HIDDEN_SNIPER_DIRECTION_MEASURE(_numOfShootDirection), Default::computeCenter(*_objectDefRect));

	_currentBullet = BulletType::MOBS;
	Object::loadObjectResource(HiddenSniperStatus::NUM_OF_HIDDEN_SNIPER_STATUS, LINK_HIDDEN_SNIPER);
	((Sprite*)_sprites[HiddenSniperStatus::HIDDEN_SNIPER_HIDE])->setFrameTime(DEFAULT_FRAME_TIME * 2);
}


HiddenSniper::~HiddenSniper()
{
}

void HiddenSniper::setBasicProperties()
{
	Mobs::setBasicProperties();
	AutoAimingObject::setBasicProperties();
	ShootObject::setBasicProperties();

	_status = HiddenSniperStatus::HIDDEN_SNIPER_HIDE;
}

void HiddenSniper::update()
{
	Mobs::update();
}

void HiddenSniper::draw()
{
	if (isDead())
	{
		return;
	}

	int sx = GET_SIGN((int)(_target->getPosition().x - getPosition().x));
	sx != 0 ? _scale.x = (float)sx : 0;

	if (_mobState == MOBS_STATE_WAIT)
		if (_shootedBullet == nullptr)
			_sprites[_status]->render(0, _position, ANGLE_NO, _scale, TRANSLATE_NO, DrawCenter::BOTTOMLEFT);
		else
		{
			_sprites[_status]->render(Timer::getRoundTime(), _position,
				ANGLE_NO, _scale, TRANSLATE_NO, DrawCenter::BOTTOMLEFT,
				COLOR_NO, -1, 0);

			if (((Sprite*)_sprites[_status])->isComplete())
				_shootedBullet = nullptr;
		}
	else if (_mobState == MOBS_STATE_ACTIVATE)
	{
		if (_status == HiddenSniperStatus::HIDDEN_SNIPER_HIDE)
		{
			if (_shootedBullet == nullptr)
			{
				_sprites[_status]->render(Timer::getRoundTime(), _position, ANGLE_NO,
					_scale, TRANSLATE_NO, DrawCenter::BOTTOMLEFT);

				if (((Sprite*)_sprites[_status])->isComplete())
					_status = (int)((_currentAimingAngle % 180 == 0 ? 0 : (1 + _currentAimingAngle / 180)));
			}
		}
		else
		{
			_sprites[_status]->render(
				Timer::getRoundTime(), _position, _visiblePercent.width, _visiblePercent.height, false, _scale, DrawCenter::BOTTOMLEFT);
		}
	}
}

unsigned int HiddenSniper::getScore()
{
	return SCORE_SNIPER;
}

void HiddenSniper::waitUpdate()
{
	if (Default::computeDistance(_target->getPosition(), _position) <= HIDDEN_SNIPER_SHOOT_DISTANCE)
		if (checkCounter(HIDDEN_SNIPER_RELOAD_TIME))
			activate();
}

void HiddenSniper::activateUpdate()
{
	ShootObject::update();
	checkIfShooted();

	if (_status != HIDDEN_SNIPER_HIDE)
	{
		if (_shootedBullet == nullptr)
		{
			_shootedBullet = shoot();
		}
		else if (checkCounter(HIDDEN_SNIPER_SHOOT_TIME))
		{
			_status = HiddenSniperStatus::HIDDEN_SNIPER_HIDE;
			suspend();
		}
	}
}

void HiddenSniper::deadUpdate()
{
}

void HiddenSniper::deadEffect()
{
	D3DXVECTOR3 effectPos(_position);
	effectPos.y += (float)(_sprites[_status]->getFrameSize().height / 2.0f);

	EffectManager::addSmallExplode(effectPos);
}

bool HiddenSniper::checkCounter(DWORD countTime)
{
	static DWORD counter = 0;

	counter += Timer::getRoundTime();

	if (counter >= countTime)
	{
		counter = 0;
		return true;
	}
	return false;
}

int HiddenSniper::computeShootAngle()
{
	return (aim());
}

D3DXVECTOR3 HiddenSniper::computeShootPoint()
{
	D3DXVECTOR3 result = _position;

	result.x += _scale.x * _sprites[_status]->getFrameSize().width / 2;
	result.x += (1 - _visiblePercent.width) * _sprites[_status]->getFrameSize().width;
	result.y -= (1 - _visiblePercent.height) * _sprites[_status]->getFrameSize().height;

	switch (_status)
	{
	case HIDDEN_SNIPER_STRAIGHT:
		result.y += HS_HEIGHT_FOOT_TO_STRAIGHT_GUN;
		break;

	case HIDDEN_SNIPER_UP45:
		result.y += _sprites[_status]->getFrameSize().height;
		break;

	case HIDDEN_SNIPER_DOWN45:
		result.y += HS_HEIGHT_FOOT_TO_DOWN45_GUN;
		break;

	default:
		break;
	}

	return result;
}


bool HiddenSniper::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

bool HiddenSniper::isTerminated()
{
	return Mobs::isTerminated();
}

void HiddenSniper::updateAsBossSubpart()
{
	update();
}
