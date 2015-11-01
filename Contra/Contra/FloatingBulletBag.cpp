#include "FloatingBulletBag.h"


FloatingBulletBag::FloatingBulletBag(D3DXVECTOR3 position, BulletFalconType containFalconType, D3DXVECTOR3 velocity)
{
	setBasicProperties();
	_position = position;
	setVelo(velocity);
	if (getVelo()->x == 0)
		_flyStraightAxis = AXIS_Y;
	else
		_flyStraightAxis = AXIS_X;
	_containFalconType = containFalconType;
	*_objectDefRect = Default::createRectFromCenter(position, FLOATING_BULLET_BAG_SIZE);
	Object::loadObjectResource(
				FLOATING_BULLET_BAG_LINK.c_str(),
				FLOATING_BULLET_BAG_RESOURCE_ROWS,
				FLOATING_BULLET_BAG_RESOURCE_COLUMNS,
				RESOURCE_MS);
}


FloatingBulletBag::~FloatingBulletBag()
{
}

void FloatingBulletBag::update()
{
	switch (_bagState)
	{
	case FBS_WAIT:
		if (!Default::checkIfBounded(_objectDefRect, &Camera::getViewPort()))
		{
			if (_flyStraightAxis == AXIS_X)
			{
				if (_objectDefRect->right < Camera::getViewPort().left)
					_bagState = FloatingBulletBagStatus::FBS_FLY;
			}
			else
				if (_objectDefRect->top < Camera::getViewPort().bottom)
					_bagState = FloatingBulletBagStatus::FBS_FLY;
		}
		break;
	case FBS_FLY:
		MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, FLOATING_BULLET_BAG_SIZE, UPDATE_XY);

		_currentDirectAngle += (int)TRUE_SPEED(FLOATING_BULLET_REDIRECTING_SPEED, Timer::getRoundTime()) % ANGLE_DEGREE_LIM;

		setVelo(computeVelocity());

		BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE | KEY_MOVEABLE);
		if (isDead())
		{
			Map::generateFalcon(getPosition(), _containFalconType);
			_isTerminated = true;
		}
		break;
	default:
		break;
	}
}

void FloatingBulletBag::setBasicProperties()
{
	DestroyableObject::setBasicProperties();
	MoveObject::setBasicProperties();
	gravity_ = &gravityNO;
	_containFalconType = BulletFalconType::FALCON_S;
	_isTerminated = false;
	_bagState = FloatingBulletBagStatus::FBS_WAIT;
	_currentDirectAngle = 0;
}

void FloatingBulletBag::draw()
{
	if (_bagState == FloatingBulletBagStatus::FBS_FLY)
	(*_sprites)->drawImage(_position);
}

void FloatingBulletBag::deadEffect()
{
	EffectManager::addExplode(_position);
}

bool FloatingBulletBag::affect(Object* obj, DWORD key)
{
	return false;
}

bool FloatingBulletBag::isTerminated()
{
	return _isTerminated;
}


D3DXVECTOR3 FloatingBulletBag::computeVelocity()
{
	D3DXVECTOR3 result;

	if (_flyStraightAxis == AXIS_X)
	{
		result.x = getVelo()->x;
		result.y = FLOATING_BULLET_REDIRECTING_CAP * Default::sin(DEGREE_TO_RADIAN(_currentDirectAngle));
	}
	else
	{
		result.x = FLOATING_BULLET_REDIRECTING_CAP * Default::cos(DEGREE_TO_RADIAN(_currentDirectAngle));
		result.y = getVelo()->y;
	}
		result.z = 0.0F;

	return result;
}
