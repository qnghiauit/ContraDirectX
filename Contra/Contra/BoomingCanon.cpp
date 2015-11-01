#include "BoomingCanon.h"


BoomingCanon::BoomingCanon(D3DXVECTOR3 position, bool drawGuards, DWORD countedTime)
{
	setBasicProperties();
	_position = position;
	*_objectDefRect = Default::createRectFromDrawCenter(_position, BOOMING_CANON_BOUND_SIZE, DrawCenter::TOPRIGHT);
	Object::loadObjectResource(BOOMING_CANON_RESOURCE_LINK.c_str(),
		BOOMING_CANON_RESOURCE_ROWS,
		BOOMING_CANON_RESOURCE_COLUMNS,
		RESOURCE_MT);

	_isDrawGuards = drawGuards;
	_timeCounter = countedTime;
}


BoomingCanon::~BoomingCanon()
{
}

unsigned int BoomingCanon::getScore()
{
	return SCORE_BOOMING_CANON;
}

void BoomingCanon::draw()
{
	if (!isDead())
	{
		((MergedTexture*)*_sprites)->draw(0, _position, _status == BCS_READY_TO_SHOOT ? 0.8F : 1.0F, 1.0F, true, DrawCenter::TOPRIGHT);
		if (_isDrawGuards)
			((MergedTexture*)*_sprites)->draw(1, _position, DrawCenter::TOPRIGHT, SCALE_NO, BOOMING_CANON_GUARD_TRANSLATE);
	}
}

void BoomingCanon::waitUpdate()
{
	if (Default::checkIfBounded(_objectDefRect, &Camera::getViewPort()))
	{
		activate();
	}
}

void BoomingCanon::activateUpdate()
{
	BulletManager::updateIfObjectIsShooted(this, KEY_DESTROYABLE);
	ShootObject::update();

	if (stateProcess())
		shoot(computeBulletLauchVelo());
}

void BoomingCanon::deadUpdate()
{
}

void BoomingCanon::deadEffect()
{
	EffectManager::addExplode(Default::computeCenter(*_objectDefRect));
}

void BoomingCanon::setBasicProperties()
{
	ShootObject::setBasicProperties();
	Mobs::setBasicProperties();
	_shootedBullet = nullptr;
	_isDrawGuards = false;
	_status = BoomingCanonState::BCS_RELOAD;
	_currentBullet = BulletType::RED_BOOM;
	_heathPoints = BOOMING_CANON_HEALTH_POINTS;
}

int BoomingCanon::computeShootAngle()
{
	return 0;
}

D3DXVECTOR3 BoomingCanon::computeShootPoint()
{
	D3DXVECTOR3 result;
	result.x = _position.x - (*_sprites)->getFrameSize().width;
	result.y = _position.y - (*_sprites)->getFrameSize().height / 2.0F;
	result.z = DEFAULT_DEPTH;

	return result;
}

void BoomingCanon::update()
{
	Mobs::update();
}

bool BoomingCanon::affect(Object* obj, DWORD key)
{
	Mobs::affect(obj, key);
	return false;
}

bool BoomingCanon::isTerminated()
{
	return isDead();
}


bool BoomingCanon::stateProcess()
{
	_timeCounter += Timer::getRoundTime();

	DWORD stateTime;

	switch (_status)
	{
	case BCS_RELOAD:
		stateTime = BOOMING_CANON_SHOOT_STATE_TIME * 2;
		break;

	case BCS_SHOOT:
	case BCS_READY_TO_SHOOT:
		stateTime = BOOMING_CANON_SHOOT_STATE_TIME;
		break;

	default:
		break;
	}

	if (_timeCounter >= stateTime)
	{
		_timeCounter = 0;
		_status = (_status + 1) % BoomingCanonState::NUM_OF_BCS_STATE;

		if (_status == BoomingCanonState::BCS_SHOOT)
			return true;
	}

	return false;
}


D3DXVECTOR3 BoomingCanon::computeBulletLauchVelo()
{
	// L = v * sqrt(2 * h / g)
	// L: how far the bullet can reach
	// v: the very start velocity
	// h: height
	// g: gravity

	static int fireForces[] = { 32, 96, 160, 224 };

	srand((unsigned int)time(nullptr));

	return SPEED_X(-fireForces[rand() % 4]);
}