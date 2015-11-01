#include "RedSpreadBoom.h"
#include "BulletManager.h"
#include "Character.h"
#include "DestroyableObject.h"

RedSpreadBoom::RedSpreadBoom()
{
	setBasicProperties();
	_bulletRS = BulletRS::MACHINE;
	_damage = BULLET_DAMAGE_MASSIVE;
	_ally = ALLY_MOBS;
}


RedSpreadBoom::~RedSpreadBoom()
{
}

void RedSpreadBoom::shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	setProperties(angle, ally, scaleX);

	D3DXVECTOR3 velo = D3DXVECTOR3(RED_SPREAD_MOVE_SPEED * Default::cos(DEGREE_TO_RADIAN(angle)),
									RED_SPREAD_MOVE_SPEED * Default::sin(DEGREE_TO_RADIAN(angle)),
									0.0F);

	shoot(velo, startPoint, ally, scaleX);
}


void RedSpreadBoom::shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	_target = nullptr;
	setVelo(velocity);

	_position = startPoint;

	*_objectDefRect = Default::createRectFromCenter(_position, RED_SPREAD_BOUND_SIZE);
	_timeCounter = 0;

	managerProcessing();
}


D3DXVECTOR3 RedSpreadBoom::generateBullet()
{
	return _position;
}

void RedSpreadBoom::update()
{
	static RED_SPREAD_BOOM_VELO;
	_timeCounter += Timer::getRoundTime();
	MoveObject::update(Timer::getRoundTime(), Default::createRectFromCenter, RED_SPREAD_BOUND_SIZE, UPDATE_XY);

	if (_timeCounter >= RED_SPREAD_BLOW_TIME)
	{
		bulletEffect();
		FOR(RED_SPREAD_NUM_OF_BOOM_GENERATED)
		{
			RedBoom* bullet = reinterpret_cast<RedBoom*>(_manager->getPool(BulletType::RED_BOOM)[_manager->getCounter(BulletType::RED_BOOM)]);
			bullet->shoot(generatedBoomVelo[i], _position, _ally, _scale.x);
			bullet->assignTarget(_target);
		}
	}
}

BulletType RedSpreadBoom::getType()
{
	return BulletType::RED_SPREAD_BOOM;
}

bool RedSpreadBoom::isTerminated()
{
	return (_timeCounter >= RED_SPREAD_BLOW_TIME);
}

void RedSpreadBoom::bulletEffect()
{
	EffectManager::addExplode(_position);
}

bool RedSpreadBoom::affect(Object* obj, DWORD key)
{
	if (CHECK_KEY(key, KEY_PLAYER))
	{
		if (_ally == ALLY_MOBS)
			if (Collision::checkCollision(this, obj, nullptr) >= 0)
			{
			if (!obj->isImmortal())
			{
				Character *player = dynamic_cast<Character *>(obj);
				if (NULL != player)
					player->dead();
			}
			}
	}
	else
		if (_ally == ALLY_PLAYER)
			if (Collision::checkCollision(this, obj, nullptr) >= 0)
			{
		if (!obj->isImmortal())
		{
			DestroyableObject *affectedObject = dynamic_cast<DestroyableObject *>(obj);
			if (NULL != affectedObject)
			{
				affectedObject->damaged(_damage);
			}
		}
		}

	return false;
}


void RedSpreadBoom::assignTarget(Object* target)
{
	_target = target;
}
