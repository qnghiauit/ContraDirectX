#include "BulletFalcon.h"
#include "Character.h"

BulletFalcon::BulletFalcon(D3DXVECTOR3 position, BulletFalconType falconType)
{
	setBasicProperties();
	_position = position;
	_falconType = falconType;
	Object::loadObjectResource(BulletFalconType::NUM_OF_FALCON_TYPE, FALCON_LINK);
	*_objectDefRect = Default::createRectFromBottomCenter(position, FALCON_SIZE);
	commandJump();
}


BulletFalcon::~BulletFalcon()
{
}

void BulletFalcon::update()
{
	if (updateMoveableWithCollision(FALCON_SIZE).size() > 0)
		setVelo(SPEED_NO);
}

void BulletFalcon::draw()
{
	_sprites[_falconType]->render(Timer::getRoundTime(), _position, ANGLE_NO, SCALE_NO, TRANSLATE_NO, DrawCenter::BOTTOMCENTER);
}

bool BulletFalcon::affect(Object* obj, DWORD key)
{
	if (CHECK_KEY(key, KEY_PLAYER))
		if (Collision::checkCollision(this, obj, nullptr) >= 0)
		{
		Character *player = dynamic_cast<Character *>(obj);
		if (NULL != player)
		{
			if (_falconType == BulletFalconType::FALCON_R)
			{
				player->activateRapidShoot();
			}
			else if (_falconType == BulletFalconType::FALCON_B)
			{
				player->activateImmortal();
			}
			else if (_falconType == BulletFalconType::FALCON_BOOM)
			{
				player->activateBoom();
			}
			else
				player->changeBullet((BulletType)_falconType);

			return true;
		}
		}

	return false;
}

void BulletFalcon::setBasicProperties()
{
	MoveObject::setBasicProperties();
	_falconType = BulletFalconType::FALCON_S;
}

bool BulletFalcon::isTerminated()
{
	return MoveObject::isTerminated();
}


void BulletFalcon::commandJump()
{
	MoveObject::commandJump();
	_velocity.x = MOVE_SPEED_RIGHT;
}
