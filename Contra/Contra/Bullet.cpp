#include "Bullet.h"
#include "Mobs.h"
#include "Character.h"

void Bullet::setBasicProperties()
{
	MoveObject::setBasicProperties();
	activate(PROPERTIES_EVATION);
	activate(PROPERTIES_VISIBLE);
	gravity_ = gravityNO;
	_ally = ALLY_PLAYER;
	_sprites = new Texture*[BulletRS::NUM_OF_BULLET_RESOURCE];
	*_sprites = new Sprite(tstring(tstring(LINK_BULLET_RS) + EXTENSION_PICTURE).c_str(), BulletRS::NUM_OF_BULLET_RESOURCE, 1);
	_bulletRS = BulletRS::MACHINE;
	_rendered = true;
	_isLiving = false;
}


bool Bullet::isTerminated()
{
	if (!Default::checkIfPointInRect(_position, Camera::getViewPort()))
		return true;
	return false;
}


void Bullet::draw()
{
	if (_rendered)
		(*_sprites)->render(Timer::getRoundTime(), _position, ANGLE_NO, SCALE_NO,
		TRANSLATE_NO, DrawCenter::CENTER, COLOR_NO, _bulletRS, _bulletRS);

#if GAMEMODE == GAMEMODE_FLASH_BULLET
	_rendered = !_rendered;
#endif
}


void Bullet::setAlly(int newAlly)
{
	_ally = newAlly;
}


bool Bullet::isLiving()
{
	return _isLiving;
}


void Bullet::setLivingStatus(bool newLivingStatus)
{
	_isLiving = newLivingStatus;
}


void Bullet::setProperties(int angle, int ally, float scaleX)
{
	this->_shootAngle = angle;
	this->_ally = ally;
	this->_scaleX = scaleX;
}


int Bullet::getDamage()
{
	return _damage;
}


bool Bullet::affect(Object* obj, DWORD key)
{
	if (obj->isShootable())
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
				bulletEffect();
				return true;
				}
		}
		else
			if (_ally == ALLY_PLAYER)
				if (Collision::checkCollision(this, obj, nullptr) >= 0)
				{
			DestroyableObject *affectedObject = dynamic_cast<DestroyableObject *>(obj);
			if (NULL != affectedObject)
			{
				affectedObject->damaged(_damage);

				Mobs* mobs = dynamic_cast<Mobs*>(obj);
				if (mobs != nullptr)
					if (mobs->isDead())
					{
					ScoreManager::increase(mobs->getScore());
					}
			}
			bulletEffect();
			return true;
				}
	}
	return false;
}


void Bullet::increaseVelocity(int bonusSpeed)
{
	setVelo(*getVelo() + D3DXVECTOR3(bonusSpeed * Default::cos(DEGREE_TO_RADIAN(_shootAngle)),
		bonusSpeed * Default::sin(DEGREE_TO_RADIAN(_shootAngle)),
		0.0f));
}


void Bullet::bulletEffect()
{
	EffectManager::addBulletExplode(_position);
}


void Bullet::assignManager(BulletManager& manager)
{
	_manager = &manager;
}


void Bullet::managerProcessing()
{
	_manager->getLivingBulletList().push_back(this);

	Bullet** pool = _manager->getPool(getType());
	int* counter = &_manager->getCounter(getType());

	if (pool != nullptr)
	{
		pool[*counter] = 0;
		(*counter)--;
	}
}

int Bullet::getShootAngle()
{
	return _shootAngle;
}
