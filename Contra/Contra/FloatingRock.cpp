#include "FloatingRock.h"


FloatingRock::FloatingRock(D3DXVECTOR3 position, D3DXVECTOR3 velocity, RECT moveRange)
{
	setBasicProperties();
	_position = position;
	setVelo(velocity);
	_moveRange = moveRange;
	Object::loadObjectResource(FLOATING_ROCK_RESOURCE_LINK.c_str(), FLOATING_ROCK_RESOURCE_ROWS, FLOATING_ROCK_RESOURCE_COLUMNS, RESOURCE_MS);
	*_objectDefRect = Default::createRectFromCenter(_position, FLOATING_ROCK_BOUND_SIZE);
}


FloatingRock::~FloatingRock()
{
}

void FloatingRock::update()
{
	MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, FLOATING_ROCK_BOUND_SIZE, UPDATE_X, false);

	if (_objectDefRect->left < _moveRange.left ||
		_objectDefRect->right > _moveRange.right)
	{
		setVelo(D3DXVECTOR3(-getVelo()->x, getVelo()->y, getVelo()->z));
	}
}

void FloatingRock::draw()
{
	(*_sprites)->render(Timer::getRoundTime(), _position, ANGLE_NO, _scale, TRANSLATE_NO, DrawCenter::CENTER);
}

void FloatingRock::setBasicProperties()
{
	MoveObject::setBasicProperties();
	Tile::setBasicProperties();
	gravity_ = gravityNO;
}

bool FloatingRock::affect(Object* obj, DWORD key)
{
	affect(obj, key, TileType::TOP_ONLY);
	return false;
}

bool FloatingRock::affect(Object* obj, DWORD key, TileType collisLevel)
{
	if (Tile::affect(obj, key, collisLevel))
		obj->fixPosition(TRUE_SPEED(*getVelo(), Timer::getRoundTime()));
	return false;
}

bool FloatingRock::isTerminated()
{
	return MoveObject::isTerminated();
}

D3DXVECTOR3* FloatingRock::getVelo()
{
	return MoveObject::getVelo();
}
