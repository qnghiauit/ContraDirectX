#include "BulletBag.h"
#include "Map.h"


BulletBag::BulletBag()
{
}


BulletBag::BulletBag(D3DXVECTOR3 position, BulletFalconType containFalconType)
{
	setBasicProperties();
	_position = position;
	_containFalconType = containFalconType;
	*_objectDefRect = Default::createRectFromCenter(position, BULLET_BAG_SIZE);
	Object::loadObjectResource(BULLET_BAG_LINK.c_str(), BULLET_BAG_RESOURCE_ROWS, BULLET_BAG_RESOURCE_COLUMNS, RESOURCE_MT);
}


BulletBag::~BulletBag()
{
}

void BulletBag::update()
{
	DestroyableObject::update();
	checkIfShooted();

	if (isDead())
	{
		Map::generateFalcon(getPosition(), _containFalconType);
		_isTerminated = true;
	}

	switch (_status)
	{
	case BulletBagStatus::BULLET_BAG_CLOSE:
		if (checkIfTimeCounted(BULLET_BAG_CLOSE_TIME))
		{
			_status = BulletBagStatus::BULLET_BAG_OPENING;
			_isOpening = true;
		}
		break;

	case BulletBagStatus::BULLET_BAG_OPENING:
		if (checkIfTimeCounted(BULLET_BAG_OPENING_TIME))
		{
			if (_isOpening)
			{
				_status = BulletBagStatus::BULLET_BAG_OPEN;
				deactivate(PROPERTIES_IMMORTAL);
			}
			else
			{
				_status = BulletBagStatus::BULLET_BAG_CLOSE;
				activate(PROPERTIES_IMMORTAL);
			}
		}
		break;

	case BulletBagStatus::BULLET_BAG_OPEN:
		if (checkIfTimeCounted(BULLET_BAG_OPEN_TIME))
		{
			_status = BulletBagStatus::BULLET_BAG_OPENING;
			_isOpening = false;
		}
		break;

	default:
		break;
	}
}

void BulletBag::setBasicProperties()
{
	DestroyableObject::setBasicProperties();
	deactivate(PROPERTIES_VISIBLE);
	activate(PROPERTIES_IMMORTAL);
	_status = BulletBagStatus::BULLET_BAG_CLOSE;
	_containFalconType = BulletFalconType::FALCON_S;
	_bagColor = 0;
	_lightTime = 0;
	_isOpening = true;
	_isTerminated = false;
	_counter = 0;
}

void BulletBag::draw()
{
	static BLENDED_COLOR_LIST;
	_lightTime += Timer::getRoundTime();
	if (_lightTime >= BULLET_BAG_SWITCH_COLOR_TIME)
	{
		_lightTime = 0;
		_bagColor = (_bagColor + 1) % NUM_OF_BLENDED_COLORS;
	}

	((MergedTexture*)*_sprites)->draw(_status * 2 + 0, _position, DrawCenter::CENTER);
	((MergedTexture*)*_sprites)->draw(_status * 2 + 1, _position, DrawCenter::CENTER, SCALE_NO, TRANSLATE_NO, ANGLE_NO, color[_bagColor]);
}

void BulletBag::deadEffect()
{
	EffectManager::addExplode(_position);
}

bool BulletBag::affect(Object* obj, DWORD key)
{
	return false;
}


bool BulletBag::checkIfTimeCounted(DWORD time)
{
	_counter += Timer::getRoundTime();
	if (_counter >= time)
	{
		_counter = 0;
		return true;
	}

	return false;
}


bool BulletBag::isTerminated()
{
	if (!_isTerminated)
	{
		return DestroyableObject::isTerminated();
	}

	return true;
}
