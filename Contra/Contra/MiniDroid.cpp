#include "MiniDroid.h"


MiniDroid::MiniDroid(D3DXVECTOR3 pos, D3DXVECTOR2 scale)
{
	setBasicProperties();
	_position = pos;
	_scale = scale;
	*_objectDefRect = Default::createRectFromCenter(getPosition(), MINI_DROID_BOUND_SIZE);
	Object::loadObjectResource(MINI_DROID_RESOURCE_LINK.c_str(), MINI_DROID_RESOURCE_ROWS,
								MINI_DROID_RESOURCE_COLUMNS, RESOURCE_MS);
}


MiniDroid::~MiniDroid()
{
}

unsigned int MiniDroid::getScore()
{
	return SCORE_MINI_DROID;
}

void MiniDroid::draw()
{
	(*_sprites)->render(Timer::getRoundTime(), getPosition(), ANGLE_NO, _scale);
}

void MiniDroid::waitUpdate()
{
	timeCounter_ += Timer::getRoundTime();

	if (timeCounter_ >= MINI_DROID_WAIT_TIME)
	{
		activate();
		timeCounter_ = 0;
	}
}

void MiniDroid::activateUpdate()
{
	MoveObject::updateMoveableWithCollision(MINI_DROID_BOUND_SIZE, true, 0, Default::createRectFromCenter);
	BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE);

	switch (_status)
	{
	case MINI_DROID_MOVE_X:
		if (!Default::checkIfPointInRect(getPosition() + 
			D3DXVECTOR3((*_sprites)->getFrameSize().width * _scale.x, 0.0F, 0.0F), Camera::getViewPort()))
		{
			setVelo(MINI_DROID_MOVE_SPEED_Y);
			_status = MINI_DROID_MOVE_Y;
		}
		break;

	case MINI_DROID_MOVE_Y:
		if (this->_currentCollisEvents.size() > 0)
		{
			setVelo(MINI_DROID_MOVE_SPEED_X(-_scale.x));
			_status = MINI_DROID_GO_OUT;
		}

	case MINI_DROID_GO_OUT:
		break;

	default:
		break;
	}
}

void MiniDroid::deadUpdate()
{
	// Do nothing
}

void MiniDroid::activate()
{
	Mobs::activate();
	setVelo(MINI_DROID_MOVE_SPEED_X(_scale.x));
	_status = MINI_DROID_MOVE_X;
}

bool MiniDroid::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

void MiniDroid::setBasicProperties()
{
	Mobs::setBasicProperties();
	MoveObject::setBasicProperties();

	gravity_ = &gravityNO;
	timeCounter_ = 0;
}

void MiniDroid::deadEffect()
{
	EffectManager::addExplode(Default::computeCenter(getRect()));
}

void MiniDroid::update()
{
	Mobs::update();
}

bool MiniDroid::isTerminated()
{
	return Mobs::isTerminated();
}