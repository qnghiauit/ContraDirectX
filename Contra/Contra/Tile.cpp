#include "Tile.h"
#include "Character.h"

Tile::Tile(long left, long right, long top, long bottom, int characterTouch, TileType type)
{
	Object::setBasicProperties();
	_currentTouchableStatus = Object::propertiesYes;
	_currentShootableStatus = Object::propertiesNo;

	_type = type;
	_characterTouch = characterTouch;

	*_objectDefRect = Default::createRECT(left, right, top, bottom);
	this->_position = Default::computeCenter(*_objectDefRect);
}

Tile::Tile()
{

}


Tile::~Tile()
{
	FOR(_linkedPretties.size())
		SAFE_RELEASE(_linkedPretties.at(i));
	_linkedPretties.clear();
}


void Tile::draw()
{
	FOR(_linkedPretties.size())
		_linkedPretties.at(i)->draw();
}


void Tile::update()
{
}


TileType Tile::getType()
{
	return _type;
}


int Tile::getTouchedStatus()
{
	return _characterTouch;
}


RECT Tile::getRect()
{
	return *_objectDefRect;
}


void Tile::pushAPretties(Pretties* newPretties)
{
	_linkedPretties.push_back(newPretties);
}


void Tile::activate()
{

}


bool Tile::isWater()
{
	return (_characterTouch == CharacterStatus::SWIM);
}

void Tile::setBasicProperties()
{
	Object::setBasicProperties();
	_characterTouch = 0;
}

bool Tile::affect(Object* obj, DWORD key, TileType collisLevel)
{
	if ((this->isWater() && !CHECK_KEY(key, KEY_WATER_COLLIS)) || _type < collisLevel)
		return false;

	if (CHECK_KEY(key, KEY_MOVEABLE))
	{
		double time = 0;
		Direction direct = Direction::NONE;

		if (isTouchable())
			time = Collision::checkCollision(obj, this, &direct);

		if (direct != Direction::NONE) { // If the object touched this tile.
			if (getType() >= TouchLevel::FOUR_FACE || direct == Direction::UP) // If tile is touchable at any direction or the touch side is top of tile
				if (time <= Timer::getRoundTime() && time >= 0) // If this collision happened sooner than current result, replace it, then.
				{
				if (time == 0)
				{
					switch (direct)
					{
					case UP:	if (obj->getRect().bottom < this->getRect().top)	return false; break;
					case DOWN:	if (obj->getRect().top > this->getRect().bottom)	return false; break;
					case LEFT:	if (obj->getRect().right > this->getRect().left)	return false; break;
					case RIGHT:	if (obj->getRect().left < this->getRect().right)	return false; break;
					default: break;
					}
				}

				MoveObject *moveObject = dynamic_cast<MoveObject *>(obj);
				moveObject->pushCollisEvent(new CollisEvent(this, direct, time));
				return true;
				}
		}
	}

	return false;
}


bool Tile::affect(Object* obj, DWORD key)
{
	return affect(obj, key, TileType::TOP_ONLY);
}


bool Tile::isTerminated()
{
	return false;
}
