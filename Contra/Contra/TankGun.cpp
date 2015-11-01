#include "TankGun.h"


TankGun::TankGun(D3DXVECTOR3 pos, Object* theHost, Object* target)
{
	_position = pos;
	_theHost = theHost;
	_distance = _position - theHost->getPosition();
	_target = target;
	setBasicProperties();
	Object::loadObjectResource(GUN_RESOURCE_LINK.c_str(), GUN_RESOURCE_ROWS, GUN_RESOURCE_COLUMNS, RESOURCE_MT);
}


TankGun::~TankGun()
{
}

unsigned int TankGun::getScore()
{
	return 0;
}

void TankGun::draw()
{
	((MergedTexture*)*_sprites)->draw(_status * 2 + 0, getPosition(), DrawCenter::TOPLEFT, _scale);
	((MergedTexture*)*_sprites)->draw(_status * 2 + 1, getPosition(), DrawCenter::TOPLEFT, _scale, TRANSLATE_NO, ANGLE_NO, _fadeColor);
}

void TankGun::waitUpdate()
{
}

void TankGun::activateUpdate()
{
	_rotateTimeCounter += Timer::getRoundTime();
	_shootDelay += Timer::getRoundTime();

	int status = GUN_STATUS_BY_AIM_ANGLE_AND_SCALEX(AutoAimingObject::aim(), _scale.x);

	if (_rotateTimeCounter >= GUN_ROTATE_DELAY)
	{
		if (status > _status && _status < (GUN_RESOURCE_ROWS - 1))
		{
			_status++;
			_rotateTimeCounter = 0;
		}
		else if (status < _status && _status > 0)
		{
			_status--;
			_rotateTimeCounter = 0;
		}
	}

	if (_shootDelay >= GUN_SHOOT_DELAY)
	{
		if (shoot() != nullptr)
		{
			_bulletCounter++;
			if (_bulletCounter >= GUN_BULLET_ROW)
			{
				_bulletCounter = 0;
				_shootDelay = 0;
			}
		}
	}
}

void TankGun::deadUpdate()
{
	// Do nothing
}

bool TankGun::affect(Object* obj, DWORD key)
{
	return false;
}

void TankGun::setBasicProperties()
{
	Mobs::setBasicProperties();
	ShootObject::setBasicProperties();
	AutoAimingObject::setBasicProperties();
	MoveObject::setBasicProperties();
	AutoAimingObject::setAimingProperties(_target, GUN_AIMING_MEASURE, getPosition());

	_scale = _theHost->getScale();
	_currentBullet = BulletType::MOBS;
	_rotateTimeCounter = 0;
	_shootDelay = GUN_SHOOT_DELAY;
	Object::activate(PROPERTIES_EVATION);
	_fadeColor = 0xFFFF0000;
}

void TankGun::deadEffect()
{
	// Do nothing
}

void TankGun::update()
{
	Mobs::update();
	ShootObject::update();
	_position = _theHost->getPosition() + _distance;
}

bool TankGun::isTerminated()
{
	return false;
}

int TankGun::computeShootAngle()
{
	return ((_status * GUN_AIMING_MEASURE) * (_scale.x > 0 ? -1 : 1) + (90.0F - GET_SIGN(_scale.x) * 90.0F));
}

D3DXVECTOR3 TankGun::computeShootPoint()
{
	static GUN_SHOOTPOINTS_FIX;

	D3DXVECTOR3 result = getPosition();
	result.x += shootPointFixes[_status].x * _scale.x;
	result.y += shootPointFixes[_status].y * _scale.y;

	return result;
}

void TankGun::setColor(DWORD color)
{
	_fadeColor = color;
}
