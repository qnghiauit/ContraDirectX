#include "StandCanon.h"

DWORD StandCanon::_lightColor[] = { 0xFFFF8C7C, 0xFFE44434, 0xFFB81C0C, 0xFFE44434 };

int StandCanon::_currentColor = 0;


StandCanon::StandCanon(D3DXVECTOR3 position, Object* target)
{
	setBasicProperties();
	this->_position = position;

	setAimingProperties(target, STAND_CANON_DIRECTION_MEASURE, position);

	Object::loadObjectResource(STAND_CANON_RESOURCE_LINK.c_str(), StandCanonStatus::NUM_OF_SC_STATUS, STAND_CANON_NUM_OF_MAP_COLOR, RESOURCE_MT);

	*this->_objectDefRect = Default::createRectFromBottomCenter(position, (*_sprites)->getFrameSize());

}


StandCanon::~StandCanon()
{
}


unsigned int StandCanon::getScore()
{
	return SCORE_STAND_CANON;
}


void StandCanon::draw()
{
	if (_mobState != MOBS_STATE_WAIT){
		if (_status < APPEAR_COMPLETLY) {
			((MergedTexture*)(*_sprites))->draw((_shootDirection)* 2 + 0, _position, 1.0f, 0.3f * _status, false, DrawCenter::BOTTOMCENTER);
			((MergedTexture*)(*_sprites))->draw((_shootDirection)* 2 + 1, _position, 1.0f, 0.3f * _status, false, DrawCenter::BOTTOMCENTER, SCALE_NO, TRANSLATE_NO, ANGLE_NO, _lightColor[_currentColor]);
		}
		else{
			((MergedTexture*)(*_sprites))->draw((_shootDirection)* 2 + 0, _position, DrawCenter::BOTTOMCENTER);
			((MergedTexture*)(*_sprites))->draw((_shootDirection)* 2 + 1, _position, DrawCenter::BOTTOMCENTER, SCALE_NO, TRANSLATE_NO, ANGLE_NO, _lightColor[_currentColor]);
		}
	}

	this->_lightTime += Timer::getRoundTime();

	if (_lightTime >= STAND_CANON_LIGHT_TIME)
	{
		_currentColor = (_currentColor + 1) % STAND_CANON_NUM_OF_LIGHT_COLOR;
		_lightTime = 0;
	}
}

void StandCanon::waitUpdate()
{
	if (Default::computeDistance(_target->getPosition(), _position) <= STAND_CANON_SHOOT_DISTANCE)
		activate();
}

void StandCanon::activateUpdate()
{
	ShootObject::update();
	checkIfShooted();

	if (_status < StandCanonStatus::APPEAR_COMPLETLY)
	{
		if (_timeCounter < STAND_CANON_ACTIVATE_TIME)
			_timeCounter += Timer::getRoundTime();
		else
		{
			_timeCounter = 0;
			_status++;
		}
	}
	else
	{
		rollUpdate();
		shootUpdate();
	}
}


void StandCanon::deadUpdate()
{
}

void StandCanon::setBasicProperties()
{
	ShootObject::setBasicProperties();
	Mobs::setBasicProperties();
	AutoAimingObject::setBasicProperties();

	Object::deactivate(PROPERTIES_VISIBLE);
	_heathPoints = STAND_CANON_HEALTH_POINTS;
	this->_status = StandCanonStatus::APPEAR_30_PERCENT;
	_timeCounter = 0;
	_shootDirection = 2;
}

void StandCanon::deadEffect()
{
	EffectManager::addExplode(_position);
}

void StandCanon::update()
{
	Mobs::update();
}

int StandCanon::computeShootAngle()
{
	return (_shootDirection + 4) * STAND_CANON_DIRECTION_MEASURE;
}

D3DXVECTOR3 StandCanon::computeShootPoint()
{
	return Default::computeCenter(*Object::_objectDefRect);
}


void StandCanon::rollUpdate()
{
	_rotateDelay += Timer::getRoundTime();

	if (_rotateDelay >= TIME_ROTATE_DELAY) {
		int requiredRollStepToAim = (int)(aim() / STAND_CANON_DIRECTION_MEASURE + 0.5) - _shootDirection - 4;

		if (STAND_CANON_CHECK_IF_SHOOT_ANGLE(Default::round(_currentAimingAngle, STAND_CANON_DIRECTION_MEASURE)))
		{
			_shootDirection += GET_SIGN(requiredRollStepToAim);
			_rotateDelay = 0;
		}
	}
}


void StandCanon::shootUpdate()
{
	FOR(_shootedBullets.size())
		if (_shootedBullets.at(i) == nullptr) {
		_shootedBullets.erase(_shootedBullets.begin() + i);
		i--;
		}

	if (_shootedBullets.size() < STAND_CANON_MAX_BULLET_IN_A_ROW && _timeCounter <= STAND_CANON_SHOOT_TIME)
		_shootedBullets.push_back(shoot());

	if (_timeCounter > Timer::getRoundTime()) _timeCounter -= Timer::getRoundTime();

	if (_timeCounter <= Timer::getRoundTime())
	{
		_timeCounter = STAND_CANON_RELOAD_TIME;
		_shootedBullets.clear();
	}
}

bool StandCanon::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

bool StandCanon::isTerminated()
{
	return Mobs::isTerminated();
}
