#include "Canon.h"
#include "Map.h"

DWORD Canon::_lightColor[] = { 0xFFFF8C7C, 0xFFE44434, 0xFFB81C0C };


Canon::Canon(D3DXVECTOR3 position, Object* target)
{
	setBasicProperties();
	setAimingProperties(target, CANON_DIRECTION_MEASURE, position);
	Object::deactivate(PROPERTIES_VISIBLE);
	_target = target;
	this->_position = position;
	_heathPoints = CANON_HEALTH_POINTS;

	Object::loadObjectResource(CANON_RESOURCE_LINK.c_str(), CanonStatus::NUM_OF_C_STATUS, CANON_NUM_OF_MAP_COLOR, RESOURCE_MT);

	*this->_objectDefRect = Default::createRectFromCenter(position, CANON_BOUND_SIZE);
}


Canon::~Canon()
{
}



void Canon::activate()
{
	if (_status < CanonStatus::OPENING) {
		_status = CanonStatus::OPENING;
		_timeCounter = (DWORD)CANON_ACTIVATE_TIME;
	}
	else if (_timeCounter <= Timer::getRoundTime())
	{
		Mobs::activate();
		_timeCounter = CANON_RELOAD_TIME;
	}
	else _timeCounter -= Timer::getRoundTime();
}


unsigned int Canon::getScore()
{
	return SCORE_CANON;
}


int Canon::getShootDirection()
{
	return aim();
}


void Canon::update()
{
	Mobs::update();
}


void Canon::draw()
{
	if (_status >= CanonStatus::CLOSE && _mobState == MOBS_STATE_WAIT)
	{
		((MergedTexture*)(*_sprites))->draw(_status * 2 + 0, _position, DrawCenter::CENTER);
		((MergedTexture*)(*_sprites))->draw(_status * 2 + 1, _position, DrawCenter::CENTER, SCALE_NO, TRANSLATE_NO, ANGLE_NO, _lightColor[_currentColor]);
	}
	else
	{
		((MergedTexture*)(*_sprites))->draw(_shootDirection * 2 + 0, _position, DrawCenter::CENTER);
		((MergedTexture*)(*_sprites))->draw(_shootDirection * 2 + 1, _position, DrawCenter::CENTER, SCALE_NO, TRANSLATE_NO, ANGLE_NO, _lightColor[_currentColor]);
	}
	this->_lightTime += Timer::getRoundTime();
	if (_lightTime >= CANON_LIGHT_TIME)
	{
		_currentColor = (_currentColor + 1) % CANON_NUM_OF_LIGHT_COLOR;
		_lightTime = 0;
	}
}


void Canon::setBasicProperties()
{
	ShootObject::setBasicProperties();
	AutoAimingObject::setBasicProperties();
	Mobs::setBasicProperties();

	deactivate(PROPERTIES_VISIBLE);
	this->_currentBullet = MOBS;
	this->_status = CanonStatus::CLOSE;
	this->_rotateDelay = TIME_ROTATE_DELAY;
	this->_shootDirection = 0;
}

void Canon::waitUpdate()
{
	if (Default::computeDistance(_target->getPosition(), _position) <= CANON_SHOOT_DISTANCE)
		activate();
}

void Canon::activateUpdate()
{
	ShootObject::update();
	checkIfShooted();

	if (!rollUpdate())
		shootUpdate();
}

void Canon::deadUpdate()
{
}

void Canon::deadEffect()
{
	EffectManager::addExplode(_position);
}

int Canon::computeShootAngle()
{
	return _currentAimingAngle;
}

D3DXVECTOR3 Canon::computeShootPoint()
{
	return _position;
}


bool Canon::rollUpdate()
{
	_rotateDelay += Timer::getRoundTime();
	int rollStep = (int)(aim() / CANON_DIRECTION_MEASURE + 0.5) - _shootDirection;

	if (_rotateDelay >= TIME_ROTATE_DELAY) {

		if (CANON_NUM_OF_DIRECTION - abs(rollStep) < abs(rollStep))
		{
			_shootDirection -= GET_SIGN(rollStep);
			if (_shootDirection < 0)
				_shootDirection += CANON_NUM_OF_DIRECTION;
		}
		else
		{
			_shootDirection += GET_SIGN(rollStep);
		}

		_shootDirection = _shootDirection % CANON_NUM_OF_DIRECTION;

		_rotateDelay = 0;
	}

	if (rollStep != 0)
		return true;
	return false;
}


void Canon::shootUpdate()
{
	FOR(_shootedBullets.size())
		if (_shootedBullets.at(i) == nullptr) {
		_shootedBullets.erase(_shootedBullets.begin() + i);
		i--;
		}

	if (_shootedBullets.empty() && _timeCounter <= CANON_SHOOT_TIME)
		_shootedBullets.push_back(shoot());

	if (_timeCounter > Timer::getRoundTime()) _timeCounter -= Timer::getRoundTime();

	if (_timeCounter <= Timer::getRoundTime())
	{
		_timeCounter = CANON_RELOAD_TIME;
		_shootedBullets.clear();
	}
}

bool Canon::affect(Object* obj, DWORD key)
{
	return false;
}

bool Canon::isTerminated()
{
	switch (Map::getCameraType())
	{
	case CAMERA_X:
		return (isDead() | (_objectDefRect->right < Camera::getViewPort().left));
		break;
	case CAMERA_Y:
		return (isDead() | (_objectDefRect->top < Camera::getViewPort().bottom));
		break;
	default:
		break;
	}

	return false;
}
