#include "Tentacle.h"


Tentacle::Tentacle(D3DXVECTOR3 position, int angleInDegree, Object* target)
{
	setBasicProperties();
	FOR(TENTACLE_NUM_OF_METAL_ORB)
	{
		_orbs.push_back(new TOrb(new MetalOrb(position), angleInDegree, i * TENTACLE_ORB_PREPARE_TIME));
	}

	_head = new CrystalOrb(position, target);
	_target = target;
	_orbs.push_back(new TOrb(_head, angleInDegree, TENTACLE_ORB_PREPARE_TIME * _orbs.size()));
	_headDirection = angleInDegree;

	if (Default::fixAngle(angleInDegree) > ANGLE_COUNTER_CLOCKWISE(90) &&
		Default::fixAngle(angleInDegree) < ANGLE_COUNTER_CLOCKWISE(270))
	{
		_waveSpeedProcess = &Tentacle::waveSpeedProcessLeft;
		_routSpeed = TENTACLE_WAVE_SPEED;
	}
	else
	{
		_waveSpeedProcess = &Tentacle::waveSpeedProcessRight;
		_routSpeed = -TENTACLE_WAVE_SPEED;
	}
}


Tentacle::~Tentacle()
{
	FOR(_orbs.size())
		SAFE_RELEASE(_orbs.at(i));
	_orbs.clear();
}

unsigned int Tentacle::getScore()
{
	return SCORE_TENTACLE;
}

void Tentacle::draw()
{
	if (_isActivated)
	{
		FOR(_orbs.size())
			_orbs.at(i)->orb->draw();
	}
	else
	{
		FOR(_orbs.size())
			if (_orbs.at(i)->orb->getPosition() != _orbs.at(0)->orb->getPosition())
				_orbs.at(i)->orb->draw();
	}
}

void Tentacle::activateUpdate()
{
	if (_isActivated)
	{
		preparingTimeProcess();
		static updateFunc updateFunctions[] = { &Tentacle::updateWaveType, &Tentacle::updateRoundType, &Tentacle::updatePointType };

		if ((this->*(updateFunctions[_currentUpdatefunction]))())
		{
			_timerCounter = 0;
			_currentUpdatefunction = (_currentUpdatefunction + 1) % TENTACLE_NUM_OF_UPDATE_TYPE;
		}

		FOR(_orbs.size())
			_orbs.at(i)->orb->update();
	}
	else
	{
		activating();
	}
}

void Tentacle::deadUpdate()
{
	FOR(_orbs.size())
		_orbs.at(i)->orb->damaged(BULLET_DAMAGE_MASSIVE);
}

void Tentacle::setBasicProperties()
{
	Mobs::setBasicProperties();
	deactivate(PROPERTIES_VISIBLE);
	Object::activate(PROPERTIES_EVATION);
	_timerCounter = 0;
	_waveSpeed = TENTACLE_WAVE_SPEED;
	_currentUpdatefunction = 0;
	_heathPoints = TENTACLE_LIFE_POINT;
}

bool Tentacle::affect(Object* obj, DWORD key)
{
	return _head->affect(obj, key);
}

bool Tentacle::isTerminated()
{
	return _head->isTerminated();
}

bool Tentacle::isDead()
{
	return _head->isDead();
}

bool Tentacle::updateWaveType()
{
	(this->*(this->_waveSpeedProcess))();

	_orbs.at(0)->angle += TRUE_SPEED(_waveSpeed, Timer::getRoundTime());

	DFOR(_orbs.size() - 1)
		if (i > 0)
			_orbs.at(i)->angle = _orbs[i - 1]->angle;

	setOrbPosByTheirAngle();
	
	_timerCounter += Timer::getRoundTime();
	return (_timerCounter >= TENTACLE_UPDATE_TURN_TIME);
}

bool Tentacle::updateRoundType()
{
	_orbs.at(0)->angle += TRUE_SPEED(_routSpeed, Timer::getRoundTime());

	DFOR(_orbs.size() - 1)
		if (i > 0)
		_orbs.at(i)->angle = _orbs[i - 1]->angle;

	//headDirection_ += TRUE_SPEED(routSpeed_, Timer::getRoundTime());

	setOrbPosByTheirAngle();

	_timerCounter += Timer::getRoundTime();
	return (_timerCounter >= TENTACLE_UPDATE_TURN_TIME);
}

bool Tentacle::updatePointType()
{
	double angleToTarget = CircleEquation(_orbs.at(0)->orb->getPosition(), _target->getPosition()).
		getDegreeFromCoordinates(_target->getPosition());

	_headDirection = RADIAN_TO_DEGREE(angleToTarget);

	FOR(_orbs.size())
		_orbs.at(i)->angle = angleToTarget;

	setOrbPosByTheirAngle();

	_timerCounter += Timer::getRoundTime();
	return (_timerCounter >= TENTACLE_UPDATE_TURN_TIME);
}

void Tentacle::waveSpeedProcessLeft()
{
	double rootAngle = CircleEquation(_orbs.at(0)->orb->getPosition(),
		_orbs.at(1)->orb->getPosition()).getDegreeFromCoordinates(_orbs.at(1)->orb->getPosition());

	if (Default::fixAngle(rootAngle) >= ANGLE_COUNTER_CLOCKWISE(225))
		_waveSpeed = -TENTACLE_WAVE_SPEED;
	else if (Default::fixAngle(rootAngle) <= ANGLE_COUNTER_CLOCKWISE(135))
		_waveSpeed = TENTACLE_WAVE_SPEED;
}

void Tentacle::waveSpeedProcessRight()
{
	double rootAngle = CircleEquation(_orbs.at(0)->orb->getPosition(),
		_orbs.at(1)->orb->getPosition()).getDegreeFromCoordinates(_orbs.at(1)->orb->getPosition());

	if (rootAngle >= ANGLE_COUNTER_CLOCKWISE(45))
		_waveSpeed = -TENTACLE_WAVE_SPEED;
	else if (rootAngle <= ANGLE_CLOCKWISE(45))
		_waveSpeed = TENTACLE_WAVE_SPEED;
}

void Tentacle::activating()
{
	double moveCap = TENTACLE_ORB_DISTANCE -
		Default::computeDistance(_orbs.at(0)->orb->getPosition(), _orbs.at(1)->orb->getPosition());

	D3DXVECTOR3 moveDistance;
	if (moveCap < TRUE_SPEED(TENTACLE_ACTIVATE_SPEED, Timer::getRoundTime()))
	{
		moveDistance = D3DXVECTOR3(
			Default::cos(DEGREE_TO_RADIAN(_headDirection)) * moveCap,
			Default::sin(DEGREE_TO_RADIAN(_headDirection)) * moveCap,
			0.0F);
		_isActivated = true;
	}
	else
		moveDistance = TRUE_SPEED(D3DXVECTOR3(
		Default::cos(DEGREE_TO_RADIAN(_headDirection)) * TENTACLE_ACTIVATE_SPEED,
		Default::sin(DEGREE_TO_RADIAN(_headDirection)) * TENTACLE_ACTIVATE_SPEED,
		0.0F),
		Timer::getRoundTime());
	
	DFOR(_orbs.size() - 1)
	{
		if (i > 0)
		{
			_orbs.at(i)->orb->setPos(_orbs.at(i)->orb->getPosition() + moveDistance);

			if (Default::computeDistance(_orbs.at(i - 1)->orb->getPosition(), _orbs.at(i)->orb->getPosition()) <
				TENTACLE_ORB_DISTANCE)
				return;
		}
	}
}

void Tentacle::waitUpdate()
{
	activate();
}

void Tentacle::deadEffect()
{
	FOR(_orbs.size())
		EffectManager::addExplode(_orbs.at(i)->orb->getPosition());
}

void Tentacle::setOrbPosByTheirAngle()
{
	FOR(_orbs.size())
	{
		if (i > 0)
		{
			_orbs.at(i)->orb->setPos(D3DXVECTOR3(
				Default::cos(DEGREE_TO_RADIAN(_orbs.at(i)->angle)) * i * TENTACLE_ORB_DISTANCE,
				Default::sin(DEGREE_TO_RADIAN(_orbs.at(i)->angle)) * i * TENTACLE_ORB_DISTANCE,
				0.0F) + _orbs.at(0)->orb->getPosition());
		}
	}
}

void Tentacle::preparingTimeProcess()
{
	FOR(_orbs.size())
		if (_orbs.at(i)->timeCounter < Timer::getRoundTime())
			_orbs.at(i)->timeCounter = 0;
		else
			_orbs.at(i)->timeCounter -= Timer::getRoundTime();
}
