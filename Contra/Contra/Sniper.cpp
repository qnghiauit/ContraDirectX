#include "Sniper.h"



Sniper::Sniper(D3DXVECTOR3 position, Object* target)
{
	setBasicProperties();
	AutoAimingObject::setAimingProperties(target, SNIPER_AIMING_MEASURE, position);

	_position = position;
	_currentBullet = BulletType::MOBS;
	_status = SniperShootSubStatus::SNIPER_STRAIGHT;

	Object::loadObjectResource(SniperShootSubStatus::NUM_OF_SNIPER_STATUS, LINK_SNIPER);

	*this->_objectDefRect = Default::createRectFromCenter(
		_position,
		_sprites[_status]->getFrameSize());

}


Sniper::~Sniper()
{
}

void Sniper::draw()
{
	int sx = GET_SIGN((int)(_target->getPosition().x - getPosition().x));
	sx != 0 ? _scale.x = (float)sx : 0;

	_sprites[_status]->render(
		Timer::getRoundTime(), _position, ANGLE_NO, _scale, TRANSLATE_NO, DrawCenter::BOTTOMCENTER);
}

int Sniper::computeShootAngle()
{
	return aim();
}

D3DXVECTOR3 Sniper::computeShootPoint()
{
	D3DXVECTOR3 result = _position;

	result.x += _scale.x * _sprites[_status]->getFrameSize().width / 2;

	switch (_status)
	{
	case HIDDEN_SNIPER_STRAIGHT:
		result.y += HS_HEIGHT_FOOT_TO_STRAIGHT_GUN;
		break;

	case HIDDEN_SNIPER_UP45:
		result.y += _sprites[_status]->getFrameSize().height;
		break;

	case HIDDEN_SNIPER_DOWN45:
		result.y += HS_HEIGHT_FOOT_TO_DOWN45_GUN;
		break;

	default:
		break;
	}

	return result;
}

unsigned int Sniper::getScore()
{
	return SCORE_SNIPER;
}

void Sniper::deadEffect()
{
	D3DXVECTOR3 effectPos(_position);
	effectPos.y += (float)(_sprites[_status]->getFrameSize().height / 2.0f);

	EffectManager::addSmallExplode(effectPos);
}

void Sniper::setBasicProperties()
{
	ShootObject::setBasicProperties();
	Mobs::setBasicProperties();
	AutoAimingObject::setBasicProperties();
}


void Sniper::activateUpdate()
{
	ShootObject::update();
	checkIfShooted();

	FOR(_shootedBullets.size())
		if (_shootedBullets.at(i) == nullptr || _shootedBullets.at(i)->getPosition() == POSITION_ZERO) {
		_shootedBullets.erase(_shootedBullets.begin() + i);
		i--;
		}

	if (_shootedBullets.size() < SNIPER_MAX_BULLET_IN_A_ROW)
	{
		Bullet* shootedOne = shoot();
		if (shootedOne != nullptr)
			_shootedBullets.push_back(shootedOne);
	}

	_status = (_currentAimingAngle % 180 == 0 ? 0 : (1 + _currentAimingAngle / 180));
}

void Sniper::waitUpdate()
{
	if (Default::computeDistance(_target->getPosition(), _position) <= SNIPER_SHOOT_DISTANCE)
		activate();
}

void Sniper::deadUpdate()
{
}


void Sniper::update()
{
	Mobs::update();
}


bool Sniper::affect(Object* obj, DWORD key)
{
	return Mobs::affect(obj, key);
}

bool Sniper::isTerminated()
{
	return Mobs::isTerminated();
}
