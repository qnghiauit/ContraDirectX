#include "Tank.h"
#include "Map.h"

Tank::Tank(D3DXVECTOR3 pos, Object* target)
{
	setBasicProperties();
	_position = pos;
	*_objectDefRect = Default::createRectFromBottomCenter(pos, TANK_BOUND_SIZE);
	Object::loadObjectResource(TANK_RESOURCE_LINK.c_str(), TANK_RESOURCE_ROWS, TANK_RESOURCE_COLUMNS, RESOURCE_MT);
	_gun = new TankGun(pos + TANK_GUN_POS_FIX(_scale.x), this, target);
}


Tank::~Tank()
{
	SAFE_RELEASE(_gun);
}

unsigned int Tank::getScore()
{
	return SCORE_TANK;
}

void Tank::draw()
{
	static TANK_FADE_COLORS;
	int fadeColorIndex = (int)((_heathPoints - 1) / (TANK_LIFE_POINTS / TANK_NUM_OF_COLORS));
	((MergedTexture*)*_sprites)->draw(_drawIndex * 2 + 0, getPosition(), DrawCenter::BOTTOMCENTER, _scale);
	((MergedTexture*)*_sprites)->draw(_drawIndex * 2 + 1, getPosition(), DrawCenter::BOTTOMCENTER, _scale,
		TRANSLATE_NO, ANGLE_NO, colors[fadeColorIndex]);

	_gun->setColor(colors[fadeColorIndex]);
	_gun->draw();
}

void Tank::waitUpdate()
{
	if (Default::checkIfBounded(this->_objectDefRect, &Camera::getViewPort()))
		activate();
}

void Tank::activateUpdate()
{
	BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE);
	switch (_status)
	{
	case TANK_MOVE:
	{
		static int frameTime = 0;
		frameTime += Timer::getRoundTime();
		if (frameTime >= DEFAULT_FRAME_TIME)
		{
			_drawIndex = (_drawIndex == 0 ? 1 : 0);
		}

		if (_timeCounter > Timer::getRoundTime())
		{
			MoveObject::updateMoveableWithCollision(TANK_BOUND_SIZE);
		}
		else
		{
			_timeCounter = TANK_STAND_TIME;
			_status = TANK_STAND;
			setVelo(SPEED_NO);
			_gun->activate();
		}
		break;
	}

	case TANK_STAND:
		if (_timeCounter < Timer::getRoundTime())
		{
			_status = TANK_MOVE;
			_timeCounter = -1; // move for infinity
			setVelo(TANK_MOVE_SPEED(_scale.x));
		}
		break;

	default:
		break;
	}

	_gun->update();
	_timeCounter -= Timer::getRoundTime();
}

void Tank::deadUpdate()
{
}

bool Tank::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

void Tank::setBasicProperties()
{
	Mobs::setBasicProperties();
	MoveObject::setBasicProperties();

	_scale = SCALE_FLIP_X;
	_drawIndex = 0;
	_status = TANK_MOVE;
	setVelo(TANK_MOVE_SPEED(_scale.x));
	_timeCounter = TANK_MOVE_TIME;
	_heathPoints = TANK_LIFE_POINTS;
}

void Tank::deadEffect()
{
	srand((unsigned int)time(nullptr));

	FOR(10)
	{
		D3DXVECTOR3 explopos = getPosition();
		explopos.y += (rand() % (int)TANK_BOUND_SIZE.height);
		explopos.x += (rand() % (int)TANK_BOUND_SIZE.width) - ((int)TANK_BOUND_SIZE.width / 2);

		EffectManager::addExplode(explopos, i * DEFAULT_FRAME_TIME);
	}
}

void Tank::update()
{
	Mobs::update();
}

bool Tank::isTerminated()
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
