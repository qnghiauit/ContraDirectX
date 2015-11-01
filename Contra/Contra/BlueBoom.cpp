#include "BlueBoom.h"


BlueBoom::BlueBoom(D3DXVECTOR3 pos)
{
	setBasicProperties();
	_position = pos;
	*_objectDefRect = Default::createRectFromCenter(getPosition(), BLUE_BOOM_THROW_SIZE);
	Object::loadObjectResource(BLUE_BOOM_RESOURCE_LINK.c_str(), BLUE_BOOM_RESOURCE_ROWS, BLUE_BOOM_RESOURCE_COLUMNS, RESOURCE_MS);
}


BlueBoom::~BlueBoom()
{
}

void BlueBoom::update()
{
	Mobs::update();
}

void BlueBoom::draw()
{
	if (!isTouchable())
	{
		(*_sprites)->drawImage(getPosition(), nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::CENTER, _currentAngle);
	}
}

bool BlueBoom::isTerminated()
{
	return (_timeCounter >= BLUE_BOOM_EXPLODE_TIME);
}

bool BlueBoom::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

void BlueBoom::setBasicProperties()
{
	MoveObject::setBasicProperties();
	Mobs::setBasicProperties();

	_timeCounter = 0;
	Object::deactivate(PROPERTIES_VISIBLE);
	MoveObject::setVelo(BLUE_BOOM_START_VELO);
}

unsigned int BlueBoom::getScore()
{
	return 0;
}

void BlueBoom::waitUpdate()
{
	MoveObject::updateMoveableWithCollision(BLUE_BOOM_THROW_SIZE, false, 0, Default::createRectFromCenter);
	_currentAngle = Default::fixAngle(_currentAngle + TRUE_SPEED(BLUE_BOOM_ROOL_SPEED, Timer::getRoundTime()));

	if (_currentCollisEvents.size() > 0)
	{
		activate();
	}
}

void BlueBoom::activateUpdate()
{
	_timeCounter += Timer::getRoundTime();
}

void BlueBoom::deadUpdate()
{
	// Do nothing
}

void BlueBoom::activate()
{
	Mobs::activate();

	Object::activate(PROPERTIES_VISIBLE);
	*_objectDefRect = Default::createRectFromCenter(getPosition(), BLUE_BOOM_EXPLODE_RANGE);
	EffectManager::addSmallExplode(getPosition());
}

void BlueBoom::deadEffect()
{
	// Do nothing
}
