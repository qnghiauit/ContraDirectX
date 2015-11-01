#include "RedBoom.h"

RedBoom::RedBoom()
{
	Bullet::setBasicProperties();
	_bulletRS = BulletRS::MACHINE;
	_damage = BULLET_DAMAGE_MASSIVE;
	_ally = ALLY_MOBS;
	activate(PROPERTIES_VISIBLE);
	gravity_ = &gravityYES;
}


RedBoom::~RedBoom()
{
}

void RedBoom::shoot(int angle, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	setProperties(angle, ally, scaleX);

	D3DXVECTOR3 velo = D3DXVECTOR3(
		RED_BOOM_DEFAULT_MOVE_SPEED * Default::cos(DEGREE_TO_RADIAN(angle)),
		RED_BOOM_DEFAULT_MOVE_SPEED * Default::sin(DEGREE_TO_RADIAN(angle)),
		0.0F);

	shoot(velo, startPoint, ally, scaleX);
}


void RedBoom::shoot(D3DXVECTOR3 velocity, D3DXVECTOR3 startPoint, int ally, float scaleX)
{
	setVelo(velocity);
	_position = startPoint;
	*_objectDefRect = Default::createRectFromCenter(_position, BULLET_SIZE_RED_BOOM);
	_isTerminated = false;
	_target = nullptr;

	managerProcessing();
}


D3DXVECTOR3 RedBoom::generateBullet()
{
	return _position;
}

void RedBoom::update()
{
	updateMoveableWithCollision(BULLET_SIZE_RED_BOOM, false, _target == nullptr ? TOP_UNDROP : TOP_ONLY,
		Default::createRectFromCenter, UPDATE_XY);

	CollisEvent* event = CollisEvent::getFirstHappenedEvent(_currentCollisEvents);

	if (event != nullptr)
	{
		Tile *touchedTile = dynamic_cast<Tile *>(event->getTouchedObject());

		if (touchedTile->getType() >= TileType::TOP_UNDROP && !touchedTile->isWater())
		{
			_isTerminated = true;
			bulletEffect(D3DXVECTOR3(getPosition().x, touchedTile->getRect().top, getPosition().z));
		}

		if (_target != nullptr)
			if (getPosition().y <= _target->getPosition().y && !touchedTile->isWater())
			{
				_isTerminated = true;
				bulletEffect(D3DXVECTOR3(getPosition().x, touchedTile->getRect().top, getPosition().z));
			}
	}
}

BulletType RedBoom::getType()
{
	return BulletType::RED_BOOM;
}

bool RedBoom::isTerminated()
{
	return (Bullet::isTerminated() || _isTerminated);
}


void RedBoom::bulletEffect()
{
	bulletEffect(_position);
}


void RedBoom::bulletEffect(D3DXVECTOR3 effectPos)
{
	if (_target == nullptr)
		EffectManager::addExplode(effectPos);
	else
		EffectManager::addSmallExplode(effectPos);
}


void RedBoom::assignTarget(Object* target)
{
	_target = target;
}
