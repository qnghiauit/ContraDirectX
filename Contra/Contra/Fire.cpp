#include "Fire.h"


Fire::Fire(D3DXVECTOR3 position, D3DXVECTOR3 velocity, RECT moveRange)
{
	setBasicProperties();
	_position = position;
	setVelo(velocity);
	_moveRange = moveRange;
	Object::loadObjectResource(FIRE_RESOURCE_LINK.c_str(), FIRE_RESOURCE_ROWS, FIRE_RESOURCE_COLUMNS, RESOURCE_MS);
	*_objectDefRect = Default::createRectFromBottomCenter(_position, FIRE_BOUND_SIZE);
}


Fire::~Fire()
{
}

void Fire::draw()
{
	(*_sprites)->render(Timer::getRoundTime(), _position, ANGLE_NO, _scale, TRANSLATE_NO, DrawCenter::BOTTOMCENTER);
}

void Fire::waitUpdate()
{
	if (Default::checkIfBounded(_objectDefRect, &Camera::getViewPort()))
	{
		activate();
	}
}

void Fire::activateUpdate()
{
	_frameTime += Timer::getRoundTime();

	if (_frameTime >= FIRE_FRAME_TIME)
	{
		_frameTime = 0;
		_scale.x = -_scale.x;
	}

	MoveObject::update(Timer::getRoundTime(), Default::createRectFromBottomCenter, FIRE_BOUND_SIZE, UPDATE_X, false);

	if (_objectDefRect->left < _moveRange.left)
		setVelo(D3DXVECTOR3(fabs(getVelo()->x), getVelo()->y, getVelo()->z));
	else if (_objectDefRect->right > _moveRange.right)
		setVelo(D3DXVECTOR3(-fabs(getVelo()->x), getVelo()->y, getVelo()->z));
}

void Fire::deadUpdate()
{
}

bool Fire::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

void Fire::setBasicProperties()
{
	MoveObject::setBasicProperties();
	Mobs::setBasicProperties();
}

void Fire::update()
{
	Mobs::update();
}

bool Fire::isTerminated()
{
	return (_objectDefRect->top < Camera::getViewPort().bottom);
}

unsigned int Fire::getScore()
{
	return 0;
}

void Fire::deadEffect()
{
}