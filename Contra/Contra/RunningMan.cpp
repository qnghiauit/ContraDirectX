#include "RunningMan.h"
#include "BulletManager.h"
#include "Map.h"

RunningMan::RunningMan(D3DXVECTOR3 position, Velocity velocity, int numOfBullet)
{
	setBasicProperties();
	this->_position = position;
	this->_respawPosition = position;
	Object::loadObjectResource(RunningManStatus::NUM_OF_RM_STATUS, LINK_RUNNING_MAN);

	*this->_objectDefRect = Default::createRectFromBottomCenter(
		_position,
		_sprites[RunningManStatus::RUNNING_MAN_RUN]->getFrameSize());

	this->_mobState = MobState::MOBS_STATE_WAIT;
	this->_velocity = velocity;
	this->_respawnTime = RUNNING_MAN_RESPAWN_TIME;
	this->_bulletPool = numOfBullet;
}


RunningMan::~RunningMan()
{
}


void RunningMan::activateUpdate()
{
	switch (_status)
	{
	case RUNNING_MAN_RUN:
	{
		updateMoveableWithCollision();
		checkIfShooted();

		if (nextStepIsNotLand() && _status == RUNNING_MAN_RUN) {
			srand((unsigned int)time(NULL));
			if (rand() % 2 == 0) {
				_velocity.x = -_velocity.x;
			}
			else {
				commandJump();
			}
		}

		if (_shootedBullet < _bulletPool)
		{
			_shootTime += Timer::getRoundTime();

			if (_shootTime >= RUNNING_MAN_RELOAD_TIME)
			{
				_status = RunningManStatus::RUNNING_MAN_SHOOT;
				_shootTime = 0;
			}
		}
		break;
	}

	case RUNNING_MAN_JUMP:
		checkIfShooted();

		if (updateMoveableWithCollision().size() > 0)
			_status = RunningManStatus::RUNNING_MAN_RUN;

		break;

	case RUNNING_MAN_SHOOT:
		checkIfShooted();

		if ((_shootTime % RUNNING_MAN_SHOOT_DELAY) <= Timer::getRoundTime() && _shootTime < _bulletPool)
		{
			static RELOAD_TIME;
			_lastShootTime = reload[(_currentBullet)];
			shoot();
			_shootedBullet++;
		}

		if (_shootTime > RUNNING_MAN_SHOOT_TIME(_bulletPool))
		{
			_shootedBullet = 0;
			_status = RUNNING_MAN_RUN;
			_shootTime = 0;
		}

		_shootTime += Timer::getRoundTime();

		break;
	default:
		break;
	}

	if (!Default::checkIfPointInRect(_position, Default::multipleRectWithDouble(Camera::getViewPort(), 1.2)))
		suspend();
}


unsigned int RunningMan::getScore()
{
	return SCORE_RUNNING_MAN;
}


void RunningMan::draw()
{
	if (_status >= RunningManStatus::RUNNING_MAN_RUN && isActivated())
		_sprites[_status]->render(Timer::getRoundTime(), _position, 0, getScale(), TRANSLATE_NO, DrawCenter::BOTTOMCENTER);
}


void RunningMan::deadEffect()
{
	D3DXVECTOR3 effectPos(_position);
	effectPos.y += (float)(_sprites[_status]->getFrameSize().height / 2.0f);

	EffectManager::addSmallExplode(effectPos);

	suspend();
}


bool RunningMan::nextStepIsNotLand()
{
	FOR(_currentCollisEvents.size())
	{
		if (_currentCollisEvents.at(i)->getDirection() == Direction::UP)
			if ((_currentCollisEvents.at(i)->getTouchedObject()->getRect().left > _position.x && getVelo()->x < 0) ||
				(_currentCollisEvents.at(i)->getTouchedObject()->getRect().right < _position.x && getVelo()->x > 0))
			{
			if (!dynamic_cast<Tile*>(_currentCollisEvents.at(i)->getTouchedObject())->isWater())
				FORJ(_currentCollisEvents.size())
			{
				if (_currentCollisEvents.at(j)->getDirection() == Direction::UP)
					if (_currentCollisEvents.at(i)->getTouchedObject()->getRect().left ==
						_currentCollisEvents.at(j)->getTouchedObject()->getRect().right ||
						_currentCollisEvents.at(i)->getTouchedObject()->getRect().right ==
						_currentCollisEvents.at(j)->getTouchedObject()->getRect().left)
						goto CONTINUE;
			}
			return true;
			}
	CONTINUE:;
	}
	return false;
}


void RunningMan::commandJump()
{
	_sprites[_status]->reset();
	_status = RunningManStatus::RUNNING_MAN_JUMP;
	_velocity.y += RUNNING_MAN_JUMP_SPEED;
}

vector<CollisEvent*> RunningMan::updateMoveableWithCollision()
{
	MoveObject::updateMoveableWithCollision(RUNNING_MAN_BOUND_SIZE);

	if (_currentCollisEvents.size() > 0)
	{
		FOR(_currentCollisEvents.size())
			if (!dynamic_cast<Tile*>(_currentCollisEvents.at(i)->getTouchedObject())->isWater())
				return _currentCollisEvents;

		EffectManager::addWaterFlow(_position);
		suspend();
	}

	return _currentCollisEvents;
}


void RunningMan::setBasicProperties()
{
	MoveObject::setBasicProperties();
	Mobs::setBasicProperties();
	ShootObject::setBasicProperties();
	_currentBullet = BulletType::MOBS;
}

void RunningMan::waitUpdate()
{
	if (_respawnTime >= RUNNING_MAN_RESPAWN_TIME)
	{
		if (!Default::checkIfPointInRect(_respawPosition, Camera::getViewPort()) &&
			Default::checkIfPointInRect(_respawPosition, Default::multipleRectWithDouble(Camera::getViewPort(), 1.2)))
			if (_respawPosition.x > Camera::getViewPort().left - 50)
			{
			activate();
			_respawnTime = 0;
			_shootTime = 0;
			}
			else
				Mobs::compulsoryDead();
	}
	else
	{
		_respawnTime += Timer::getRoundTime();
	}
}

void RunningMan::deadUpdate()
{
	suspend();
}


void RunningMan::activate()
{
	Mobs::activate();
	Object::deactivate(PROPERTIES_EVATION);

	_status = RunningManStatus::RUNNING_MAN_RUN;
	_position = _respawPosition;
	*_objectDefRect = Default::createRectFromBottomCenter(_position, RUNNING_MAN_BOUND_SIZE);
	_velocity.x = fabs(_velocity.x) * GET_SIGN(Camera::getPosition().x - _respawPosition.x);
}

bool RunningMan::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}


void RunningMan::update()
{
	Mobs::update();
}

bool RunningMan::isTerminated()
{
	return (Mobs::isTerminated() && MoveObject::isTerminated());
}


void RunningMan::suspend()
{
	Mobs::suspend();

	Object::activate(PROPERTIES_EVATION);
	_heathPoints = 1;
	_position = _respawPosition;
}


int RunningMan::computeShootAngle()
{
	return (_scale.x > 0 ? RUNNING_MAN_SHOOT_ANGLE_RIGHT : RUNNING_MAN_SHOOT_ANGLE_LEFT);
}


D3DXVECTOR3 RunningMan::computeShootPoint()
{
	return (_position + RUNNING_MAN_SHOOT_POINT_DISTANCE_FROM_POSITION(_scale.x));
}