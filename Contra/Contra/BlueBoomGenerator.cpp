#include "BlueBoomGenerator.h"


BlueBoomGenerator::BlueBoomGenerator(D3DXVECTOR3 pos)
{
	_position = pos;
}


BlueBoomGenerator::~BlueBoomGenerator()
{
}

void BlueBoomGenerator::setBasicProperties()
{
	Object::setBasicProperties();
}

void BlueBoomGenerator::update()
{
	if (_timeCounter < BB_GENERATOR_DELAY)
	{
		_timeCounter += Timer::getRoundTime();
	}

	if (Default::checkIfPointInRect(getPosition(), Camera::getViewPort()))
	{
		if (_timeCounter >= BB_GENERATOR_DELAY)
		{
			Map::addGameObject(new BlueBoom(getPosition()));
			_timeCounter = 0;
		}
	}
}

void BlueBoomGenerator::draw()
{
	// Do nothing
}

bool BlueBoomGenerator::affect(Object* obj, DWORD key)
{
	// Do nothing
	return false;
}

bool BlueBoomGenerator::isTerminated()
{
	return (getPosition().x < Camera::getViewPort().left);
}
