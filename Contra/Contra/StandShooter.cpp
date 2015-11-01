#include "StandShooter.h"
#include "Map.h"


StandShooter::StandShooter(D3DXVECTOR3 pos)
{
	setBasicProperties();
	_position = pos;
	*_objectDefRect = Default::createRectFromBottomCenter(getPosition(), STAND_SHOOTER_BOUND_SIZE);
	Object::loadObjectResource(STAND_SHOOTER_RESOURCE_LINK.c_str(),
		STAND_SHOOTER_RESOURCE_ROWS, STAND_SHOOTER_RESOURCE_COLUMNS, RESOURCE_MT);
}


StandShooter::~StandShooter()
{
}

unsigned int StandShooter::getScore()
{
	return SCORE_STAND_SHOOTER;
}

void StandShooter::draw()
{
	static STAND_SHOOTER_CANON_TRANSLATES;
	static int timeCounter = 0;

	if (_currentCanonTranslate > 0)
	{
		timeCounter += Timer::getRoundTime();
		if (timeCounter >= DEFAULT_FRAME_TIME)
		{
			_currentCanonTranslate = 0;
			timeCounter = 0;
		}
	}

	((MergedTexture*)*_sprites)->draw(0, getPosition(), DrawCenter::BOTTOMCENTER, _scale);
	((MergedTexture*)*_sprites)->draw(1, getPosition(), DrawCenter::BOTTOMCENTER, _scale, translate[_currentCanonTranslate]);
}

void StandShooter::waitUpdate()
{
	if (Default::checkIfBounded(_objectDefRect, &Camera::getViewPort()))
		activate();
}

void StandShooter::activateUpdate()
{
	static int timer = 0;
	timer += Timer::getRoundTime();

	BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE);

	if (timer >= STAND_SHOOTER_SHOOT_DELAY)
	{
		timer = 0;
		instanceShoot();
		_currentCanonTranslate = 1;
	}
}

void StandShooter::deadUpdate()
{
}

bool StandShooter::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

void StandShooter::setBasicProperties()
{
	Object::setBasicProperties();
	ShootObject::setBasicProperties();

	_currentBullet = BulletType::ENEMY_MACHINE;
	_heathPoints = STAND_SHOOTER_LIFE_POINTS;
	_scale = SCALE_FLIP_X;
}

void StandShooter::deadEffect()
{
	EffectManager::addExplode(Default::computeCenter(*_objectDefRect));
}

int StandShooter::computeShootAngle()
{
	// the shooter only shoot 1 direction
	return 180;
}

D3DXVECTOR3 StandShooter::computeShootPoint()
{
	D3DXVECTOR3 result = getPosition();
	result.x += STAND_SHOOTER_SHOOTPOINT_FIX.x * _scale.x;
	result.y += STAND_SHOOTER_SHOOTPOINT_FIX.y * _scale.y;

	return result;
}

void StandShooter::update()
{
	Mobs::update();
	ShootObject::update();
}

bool StandShooter::isTerminated()
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