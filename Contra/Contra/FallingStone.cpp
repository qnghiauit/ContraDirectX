#include "FallingStone.h"

D3DXVECTOR2 FallingStone::_translate = ROLLING_TRANSLATE;

FallingStone::FallingStone(D3DXVECTOR3 position)
{
	setBasicProperties();
	_respawPos = _position = position;
	*_objectDefRect = Default::createRectFromCenter(_position, FALLING_STONE_BOUND_SIZE);
	Object::loadObjectResource(FALLING_STONE_RESOURCE_LINK.c_str(), 
								FALLING_STONE_RESOURCE_ROWS, 
								FALLING_STONE_RESOURCE_COLUMNS, 
								RESOURCE_MS);
}


FallingStone::~FallingStone()
{
}

unsigned int FallingStone::getScore()
{
	return SCORE_FALLING_STONE;
}

void FallingStone::draw()
{
	if (!isDead())
		if (_status == FALLING_STONE_ROOL)
			(*_sprites)->render(Timer::getRoundTime(), _position, ANGLE_NO, SCALE_NO, _translate, DrawCenter::CENTER);
		else
			(*_sprites)->render(Timer::getRoundTime(), _position, ANGLE_NO, _scale, TRANSLATE_NO, DrawCenter::CENTER);
}

void FallingStone::waitUpdate()
{
	if (Default::checkIfBounded(_objectDefRect, &Camera::getViewPort()))
		activate();
}

void FallingStone::activateUpdate()
{
	static int currentFlip = 0;
	static int currentTimeSign = 0;
	static DWORD frameTime_;
	static SCALE_CASE;

	_timeCounter += Timer::getRoundTime();

	_scale = scale[currentFlip];

	if (currentTimeSign != Timer::getRoundCode())
	{
		currentTimeSign = Timer::getRoundCode();
		frameTime_ += Timer::getRoundTime();
		if (frameTime_ >= FALLING_STONE_FRAME_TIME)
		{
			frameTime_ = 0;
			currentFlip = (currentFlip + 1) % NUM_OF_SCALE_CASE;
			_translate = -_translate;
		}
	}

	switch (_status)
	{
	case FALLING_STONE_ROOL:
		if (_timeCounter >= FALLING_STONE_ROLL_TIME)
		{
			_timeCounter = 0;
			_status = FALLING_STONE_FALL;
			Object::activate(PROPERTIES_VISIBLE);
		}
		break;

	case FALLING_STONE_FALL:

		if (!_currentCollisEvents.empty())
		{
			activateFallThrough();
			commandJump();
		}

		updateMoveableWithCollision(FALLING_STONE_BOUND_SIZE, false, TileType::TOP_ONLY, &Default::createRectFromCenter);
		BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE);

		if (_position.y < Camera::getViewPort().bottom)
		{
			dead(0);
			deactivate(PROPERTIES_VISIBLE);
		}
		break;

	default:
		break;
	}
}

void FallingStone::deadUpdate()
{
	_timeCounter += Timer::getRoundTime();
	if (_timeCounter >= FALLING_STONE_RELOAD_TIME)
	{
		activate();
		_timeCounter = 0;
		reset();
	}
}

void FallingStone::setBasicProperties()
{
	MoveObject::setBasicProperties();
	Mobs::setBasicProperties();
	setVelo(SPEED_NO);
	deactivate(PROPERTIES_VISIBLE);
	_status = FALLING_STONE_ROOL;
	_heathPoints = FALLING_STONE_HEALTH_POINTS;
}

void FallingStone::deadEffect()
{
	EffectManager::addExplode(_position);
}

bool FallingStone::isDead()
{
	return (_position.y < Camera::getViewPort().bottom && !Default::checkIfBounded(_objectDefRect, &Camera::getViewPort()) ||
		Mobs::isDead());
}

void FallingStone::update()
{
	Mobs::update();
}

bool FallingStone::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

bool FallingStone::isTerminated()
{
	return (_respawPos.y < Camera::getViewPort().bottom);
}

void FallingStone::commandJump()
{
	setVelo(FALLING_STONE_JUMP_VELO);
}


void FallingStone::reset()
{
	_status = FALLING_STONE_ROOL;
	_position = _respawPos;
	_timeCounter = 0;
	setVelo(SPEED_NO);
	_heathPoints = FALLING_STONE_HEALTH_POINTS;
	_ignoredCollisObject.clear();
}
