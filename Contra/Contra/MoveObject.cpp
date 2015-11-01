#include "MoveObject.h"
#include "Map.h"

void MoveObject::update(double moveTime, RECT (*createRECTFunction)(D3DXVECTOR3, Size), Size boundSize, int updateDirect, bool isUpdateScale)
{
	D3DXVECTOR3 trueSpeed = TRUE_SPEED(_velocity, (float)moveTime);

	if (isUpdateScale)
	{
		if (trueSpeed.x > 0.0f)	setScale(SCALE_NO); else
			if (trueSpeed.x < 0.0f)	setScale(SCALE_FLIP_X);
	}

	if (updateDirect == UPDATE_X || updateDirect == UPDATE_XY)
		_position.x += trueSpeed.x;

	if (updateDirect == UPDATE_X)
		_velocity.y = SPEED_NO.y;

	if (updateDirect == UPDATE_Y || updateDirect == UPDATE_XY)
		_position.y += trueSpeed.y;

	_position.z += trueSpeed.z;

	*_objectDefRect = createRECTFunction(_position, boundSize);

	gravity_(this);
}

void MoveObject::update()
{
}


void MoveObject::gravityYES(MoveObject* object)
{
	object->_velocity.y += TRUE_SPEED(GRAVITY, (float)Timer::getRoundTime());
}


void MoveObject::gravityNO(MoveObject* object)
{
}


void MoveObject::setBasicProperties()
{
	Object::setBasicProperties();
	gravity_ = &gravityYES;
	_velocity = SPEED_NO;
	_fallThrough = false;
}



D3DXVECTOR3* MoveObject::getVelo()
{
	return &_velocity;
}


void MoveObject::setVelo(D3DXVECTOR3 newVelo)
{
	_velocity = newVelo;
}


void MoveObject::commandJump()
{
	_velocity.y += JUMP_SPEED;
}


vector<CollisEvent*> MoveObject::updateMoveableWithCollision(
	Size boundSize, 
	bool isSwimCollis, 
	int collisLevel, 
	RECT(*createRectFunction)(D3DXVECTOR3, Size),
	DWORD absoluteUpdate)
{
	detectCurrentCollisEvent(isSwimCollis, collisLevel);

	if (_currentCollisEvents.size() == 0) { // Doesn't touch to any tile.
		this->MoveObject::update(Timer::getRoundTime(), createRectFunction, boundSize, UPDATE_XY);
	}
	else { // If touch
		// Normally update
		int updateDirect = UPDATE_XY;

		if (absoluteUpdate != UPDATE_XY)
		{
			FOR(_currentCollisEvents.size()){

				Tile *tile = dynamic_cast<Tile *>(_currentCollisEvents.at(i)->getTouchedObject());
				if (NULL != tile)
				{
					if (isSwimCollis || (!isSwimCollis && !tile->isWater()))
					{
						if (_currentCollisEvents.at(i)->isIgnored(&_ignoredCollisObject) && _fallThrough)
						{
							_currentCollisEvents.erase(_currentCollisEvents.begin() + i);
							i--;
							continue;
						}

						if (_currentCollisEvents.at(i)->getTime() == 0)
						{ // Hit from prev update

							if (_currentCollisEvents.at(i)->getDirection() == Direction::DOWN && _velocity.y > 0 ||
								_currentCollisEvents.at(i)->getDirection() == Direction::UP && _velocity.y < 0)
								_velocity.y = 0.0F;

							updateDirect = updateDirect & (~_currentCollisEvents.at(i)->getDirection());
						}
					}
				}
			}
		}

			if (_currentCollisEvents.size() == 0)
				this->MoveObject::update(Timer::getRoundTime(), createRectFunction, boundSize, (updateDirect | absoluteUpdate));
			else
			{
				CollisEvent* firstHappenedEvent = CollisEvent::getFirstHappenedEvent(_currentCollisEvents);
				this->MoveObject::update(firstHappenedEvent->getTime() == 0 ?
					Timer::getRoundTime() : firstHappenedEvent->getTime(),
					createRectFunction,
					boundSize, (updateDirect | absoluteUpdate));
		}
	}
	return _currentCollisEvents;
}


void MoveObject::activateFallThrough()
{
	_fallThrough = true;
	_ignoredCollisObject.clear();

	FOR(_currentCollisEvents.size())
		_ignoredCollisObject.push_back(_currentCollisEvents.at(i)->getTouchedObject());
}


void MoveObject::deactivateFallThrough()
{
	_fallThrough = false;
	_ignoredCollisObject.clear();
}


vector<CollisEvent*> MoveObject::getCollisEvent()
{
	return _currentCollisEvents;
}


bool MoveObject::isTerminated()
{
	if (Default::checkIfBounded(&getRect(), &Camera::getViewPort()))
		if (getRect().right < Camera::getViewPort().left ||
			getRect().top < Camera::getViewPort().bottom)
			return true;
	return false;
}


void MoveObject::pushCollisEvent(CollisEvent* newCollis)
{
	_currentCollisEvents.push_back(newCollis);
}


void MoveObject::detectCurrentCollisEvent(bool isWaterCollis, int collisLevel)
{
	FOR(_currentCollisEvents.size())
		SAFE_RELEASE(_currentCollisEvents[i]);
	_currentCollisEvents.clear();

	Map::affectedByTiles(this, isWaterCollis, collisLevel);
}
