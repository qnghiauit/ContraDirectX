#include "Boss.h"

Boss::~Boss()
{
	FOR(_subParts.size())
		SAFE_RELEASE(_subParts.at(i));
	_subParts.clear();
}


unsigned int Boss::getScore()
{
	return SCORE_BOSS;
}


void Boss::setBasicProperties()
{
	Mobs::setBasicProperties();
	_timeCounter = 0;
}


void Boss::assignSubPart(Mobs* newPart)
{
	_subParts.push_back(newPart);
}


void Boss::update()
{
	Mobs::update();
}


void Boss::draw()
{
	FOR(_subParts.size())
		_subParts.at(i)->draw();
}


void Boss::activateUpdate()
{

	FOR(_subParts.size())
	{
		_subParts.at(i)->updateAsBossSubpart();
		if (_subParts.at(i)->isDead())
		{
			SAFE_RELEASE(_subParts.at(i))
			_subParts.erase(_subParts.begin() + i);
		}
	}
}


void Boss::deadUpdate()
{
	FOR(_subParts.size())
	{
		_subParts.at(i)->damaged(BULLET_DAMAGE_MASSIVE);
		if (_subParts.at(i)->isDead())
		{
			SAFE_RELEASE(_subParts.at(i))
				_subParts.erase(_subParts.begin() + i);
		}
	}

	_timeCounter += Timer::getRoundTime();

	if (_timeCounter > BOSS_DEAD_TIME && _stageClearPlayed == false)
	{
		FreeSound::play(STAGE_CLEAR_SOUND);
		_stageClearPlayed = true;
	}
}


void Boss::waitUpdate()
{
	if (Default::checkIfBounded(_objectDefRect, &Camera::getViewPort()))
		activate();
}


void Boss::deadEffect()
{
	FreeSound::play(BOSS_DEAD_SOUND);
}


DWORD Boss::getTimeCounter()
{
	return _timeCounter;
}
