#include "DestroyableObject.h"
#include "BulletManager.h"
#include "Map.h"

DestroyableObject::~DestroyableObject()
{
}



bool DestroyableObject::isDead()
{
	return (_heathPoints == 0);
}


void DestroyableObject::damaged(unsigned int damage, DWORD deadCounter)
{
	if (!isImmortal())
		if (_heathPoints <= damage)
		{
			_heathPoints = 0;
			dead(deadCounter);
			deadEffect();
		}
		else
		{
			_heathPoints -= damage;
			FreeSound::play(DAMAGE_SOUND, false, true);
		}
	else
		FreeSound::play(DAMAGE_SOUND, false, true);
}


void DestroyableObject::dead(DWORD deadCounter)
{
	_deadCounter = deadCounter;
}

void DestroyableObject::update()
{
	if (_heathPoints == 0)
		if (_deadCounter <= Timer::getRoundTime())
			_deadCounter = 0;
		else
			_deadCounter -= Timer::getRoundTime();
}

void DestroyableObject::setBasicProperties()
{
	Object::setBasicProperties();
	_heathPoints = 1;
	_deadCounter = 0;
}



void DestroyableObject::checkIfShooted()
{
	BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE | KEY_NO);
}


bool DestroyableObject::isTerminated()
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
