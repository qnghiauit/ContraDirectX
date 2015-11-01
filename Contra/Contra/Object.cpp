#include "Object.h"

Object::~Object()
{
	SAFE_RELEASE(_sprites);
	SAFE_RELEASE(_objectDefRect);
}


void Object::setBasicProperties()
{
	_currentImmortalStatus = Object::propertiesNo;
	_currentShootableStatus = Object::propertiesYes;
	_currentTouchableStatus = Object::propertiesYes;
	try
	{
		SAFE_RELEASE(_objectDefRect);
	}
	catch (exception* e){}

	_objectDefRect = new RECT();
	_status = 0;
	_scale = SCALE_NO;
}


bool Object::isImmortal()
{
	return (*_currentImmortalStatus)();
}



bool Object::isTouchable()
{
	return (*_currentTouchableStatus)();
}



bool Object::isShootable()
{
	return (*_currentShootableStatus)();
}



bool Object::propertiesYes()
{
	return true;
}


bool Object::propertiesNo()
{
	return false;
}


void Object::activate(int properties)
{
	switch (properties)
	{
	case PROPERTIES_EVATION:
		_currentShootableStatus = &Object::propertiesNo;
		break;

	case PROPERTIES_IMMORTAL:
		_currentImmortalStatus = &Object::propertiesYes;
		break;

	case PROPERTIES_VISIBLE:
		_currentTouchableStatus = &Object::propertiesYes;
		break;

	default:
		break;
	}
}


void Object::deactivate(int properties)
{
	switch (properties)
	{
	case PROPERTIES_EVATION:
		_currentShootableStatus = &Object::propertiesYes;
		break;

	case PROPERTIES_IMMORTAL:
		_currentImmortalStatus = &Object::propertiesNo;
		break;

	case PROPERTIES_VISIBLE:
		_currentTouchableStatus = &Object::propertiesNo;
		break;

	default:
		break;
	}
}


void Object::switchStatusTo(unsigned int newStatus)
{
	if (_status != newStatus) {
		_sprites[_status]->reset();
		_status = newStatus;
	}
}



void Object::loadObjectResource(int numOfSprite, tstring spriteLink)
{
	_nSprite = numOfSprite;
	fstream file(spriteLink + INFO_FILE);
	if (file.is_open()) {
		_sprites = new Texture*[numOfSprite];
		FOR(numOfSprite)
			_sprites[i] = new Sprite((spriteLink + Default::tostr(i) + EXTENSION_PICTURE).c_str(), file);

		file.close();
	}
	else {
		WARNING_BOX((WARNING_CANNOT_OPEN_FILE + spriteLink).c_str());
	}
}



void Object::loadObjectResource(const tchar* path, int rows, int columns, int prettiesResourceType)
{
	_nSprite = 1;
	_resourceType = prettiesResourceType;

	_sprites = new Texture*();

	switch (prettiesResourceType)
	{
	case RESOURCE_MT:
		*_sprites = new MergedTexture(path, columns, rows);
		break;
	case RESOURCE_MS:
		*_sprites = new Sprite(path, columns, rows);
		break;
	default:
		break;
	}
}



void Object::setScale(D3DXVECTOR2 newScale)
{
	_scale = newScale;
}


D3DXVECTOR3 Object::getPosition()
{
	return this->_position;
}


void Object::fixPosition(D3DXVECTOR3 fixDistance)
{
	_position += fixDistance;
	Default::moveRECT(*_objectDefRect, fixDistance);
}


Texture* Object::getCurrentSprite()
{
	return _sprites[_status];
}


RECT Object::getRect()
{
	return *Object::_objectDefRect;
}


D3DXVECTOR3* Object::getVelo()
{
	return &SPEED_NO;
}


D3DXVECTOR2 Object::getScale()
{
	return _scale;
}


bool Object::checkIfCurrentSpriteIs(int spriteIndex)
{
	return (spriteIndex == _status);
}

void Object::setPosition(D3DXVECTOR3 newPos)
{
	_position = newPos;
}
