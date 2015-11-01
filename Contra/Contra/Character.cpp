#include "Character.h"

Character::Character(D3DXVECTOR3 position)
{
	setBasicProperties();
	this->_position = position;

	Object::loadObjectResource(CharacterStatus_Leg::NUM_OF_C_LEG_STATUS, LINK_CHARACTER_LEG);

	_spriteCharLeg = _sprites;
	_sprites = 0;

	Object::loadObjectResource(CharacterStatus_Head::NUM_OF_C_HEAD_STATUS, LINK_CHARACTER_HEAD);


	*this->_objectDefRect = Default::createRectFromBottomCenter(_position, _sprites[_status]->getFrameSize());
	ScoreManager::initialize(this);
}


Character::~Character()
{
	State::releaseCharacterState();

	SAFE_RELEASE(_spriteCharLeg);
}


void Character::update()
{
	_currentState->controling();
	_currentState->update();

	if (isImmortal())
	{
		_immortalTime += Timer::getRoundTime();
		if ((_immortalTime >= CHARACTER_IMMORTAL_TIME && !_immortalBulletActivate) ||
			(_immortalTime >= CHARACTER_BULLET_IMMORTAL_TIME && _immortalBulletActivate))
		{
			deactivate(PROPERTIES_IMMORTAL);
		}
	}
}


void Character::draw()
{
	static DWORD immortalColors[] = { 0xFFFF0000, 0xFF0000FF };
	static DWORD respawColors[] = { COLOR_NO, 0x00000000 };
	static int colorIndex = 0;
	static DWORD timer = 0;
	static Texture lifeIcon = Texture(CHARACTER_EXTRA_LIFE_RESOURCE_LINK.c_str());

	DWORD color = COLOR_NO;

	if (isImmortal())
	{
		timer += Timer::getRoundTime();
		if (timer >= DEFAULT_FRAME_TIME)
		{
			// 2 is the number of faded colors which is declared above
			colorIndex = (colorIndex + 1) % 2;
			timer = 0;
		}

		if (_immortalBulletActivate)
		{
			color = immortalColors[colorIndex];\
		}
		else
		{
			color = respawColors[colorIndex];
		}
	}

	_currentState->draw(_sprites[_status], _spriteCharLeg[_status_leg], color);

	FOR(getLifeRemaining())
	{
		if (i >= CHARACTER_NUM_OF_MAX_LIFE_ICON)
		{
			break;
		}

		// The objects which is not converted as a thing in game world must be compute the draw position
		// by the machine coordinates
		D3DXVECTOR3 lifePos = WDPOS_TOP_LEFT;
		lifePos.y += CHARACTER_LIFE_ICON_Y_FIX;
		lifePos.x += i * (lifeIcon.getFrameSize().width + CHARACTER_LIFE_ICON_X_FIX) + CHARACTER_LIFE_ICON_DISTANCE_TO_LEFT;
		lifeIcon.drawImage(lifePos, nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::TOPLEFT, ANGLE_NO, COLOR_NO, false);
	}
}


void Character::setBasicProperties()
{
	static RELOAD_TIME;
	ShootObject::setBasicProperties();
	MoveObject::setBasicProperties();
	_currentBullet = BulletType::BASIC;
	this->_lastShootTime = reload[(_currentBullet)];
	this->_status = CharacterStatus_Head::RUN_HEAD;
	this->_status_leg = CharacterStatus_Leg::RUN_LEG;
	this->_velocity = SPEED_NO;

	_currentTouchableStatus = &Object::propertiesNo;
	this->_life = DEFAULT_LIFE;
}


void Character::setStatus(State* newState)
{
	if (_currentState != newState) {
		_currentState = newState;
		_currentState->enter();
	}
}


void Character::switchLegStatusTo(unsigned int newStatus)
{
	if (_status_leg != newStatus) {
		_spriteCharLeg[_status_leg]->reset();
		_status_leg = newStatus;
	}
}


int Character::reset()
{
	_position = Map::computeRespawPos();
	_velocity = SPEED_NO;
	_immortalBulletActivate = false;
	_immortalTime = 0;
	Object::activate(PROPERTIES_IMMORTAL);
	changeBullet(BulletType::BASIC);
	State::reset();
	_life--;
	deactivateFallThrough();

	return _life;
}


Bullet* Character::shoot()
{
	if (_status == CharacterStatus_Head::RUN_HEAD)
		switchStatusTo(CharacterStatus_Head::STRAIGHT);

	static RELOAD_TIME;

	if (_lastShootTime >= reload[(_currentBullet)]) {
		_lastShootTime = Timer::getRoundTime();

		Bullet* shootedOne = BulletManager::getInstance().shoot(_currentBullet, ALLY_PLAYER, computeShootPoint(), computeShootAngle(), _scale.x);
		if (_bonusBulletSpeed != 0 && shootedOne != nullptr)
		{
			shootedOne->increaseVelocity(_bonusBulletSpeed);
		}

		return shootedOne;
	}

	return 0;
}


int	Character::computeShootAngle()
{
	int angle = (int)ANGLE_BASIC_SX(_scale.x);
	return _currentState->computeShootAngle(angle);
}


D3DXVECTOR3 Character::computeShootPoint()
{
	D3DXVECTOR3 startpoint(_position.x,
		_position.y,
		DEFAULT_DEPTH);

	if (_currentState->getIndex() == CharacterStatus::ON_GROUND ||
		_currentState->getIndex() == CharacterStatus::FALL_DOWN)
		startpoint.y += (float)_spriteCharLeg[_status_leg]->getFrameSize().height;

	switch (this->_status)
	{
	case CharacterStatus_Head::UP90:
		startpoint.x += (float)(_scale.x * 4);
		startpoint.y += (float)(_sprites[_status]->getFrameSize().height);
		break;

	case CharacterStatus_Head::UP45:
		startpoint.x += (float)(_scale.x * (_sprites[_status]->getFrameSize().width / 2));
		startpoint.y += (float)(_sprites[_status]->getFrameSize().height);
		break;

	case CharacterStatus_Head::STRAIGHT:
		startpoint.x += (float)(_scale.x * (_sprites[_status]->getFrameSize().width / 2));
		startpoint.y += (float)(DISTANCE_FROM_HIP_TO_MUZZLE_STRAIGHT_Y);
		break;

	case CharacterStatus_Head::DOWN45:
		startpoint.x += (float)(_scale.x * (_sprites[_status]->getFrameSize().width / 2));
		startpoint.y += (float)(DISTANCE_FROM_HIP_TO_MUZZLE_DOWN45_Y);
		break;

	case CharacterStatus_Head::FALL:
		startpoint.x += (float)(_scale.x * (_sprites[_status]->getFrameSize().width / 2));
		startpoint.y += (float)(DISTANCE_FROM_HIP_TO_MUZZLE_FALL_Y);
		break;

	case CharacterStatus_Head::JUMP:
		startpoint.y += (float)(_spriteCharLeg[_status_leg]->getFrameSize().height / 2);
		break;

	case CharacterStatus_Head::LIE:
		startpoint.x += (float)(_scale.x * (_sprites[_status]->getFrameSize().width / 2));
		startpoint.y += (float)(DISTANCE_FROM_HIP_TO_MUZZLE_LIE_Y);
		break;

	default:
		break;
	}

	return startpoint;
}


int Character::getCurrentLegSpriteFrame()
{
	return _spriteCharLeg[_status_leg]->getCurrentFrame();
}


void Character::setLegFrame(int frame)
{
	((Sprite*)_spriteCharLeg[_status_leg])->setFrame(frame);
}


State* Character::getCurrentStatus()
{
	return _currentState;
}


vector<CollisEvent*> Character::updateMoveableWithCollision()
{
	MoveObject::updateMoveableWithCollision(_currentState->getBoundSize(), _currentState->isWaterCollis());
	if (_position.y < Camera::getViewPort().bottom)
	{
		if (_currentState->getIndex() != DIE)
			dead();
	}

	CollisEvent* standingSignCollisEvent = CollisEvent::isStandingOnSomething(&_currentCollisEvents, &getRect(),
		_fallThrough ?
		&_ignoredCollisObject : nullptr);

	if (standingSignCollisEvent == nullptr){ // Not standing on anything
		if (!IS_UPDATING_Y(this->getCurrentStatus()->getIndex())) // If current status is not allow to Update Y-axis
			this->setStatus(State::getState(CharacterStatus::FALL_DOWN));
	}
	else // If standing on something
		if (this->getCurrentStatus()->getIndex() != CharacterStatus::DIE){ // If not dying
		this->setStatus(State::getState((dynamic_cast<Tile*>(standingSignCollisEvent->getTouchedObject())->getTouchedStatus())));
		/*this->setVelo(D3DXVECTOR3(this->getVelo()->x, TRUE_SPEED(GRAVITY, Timer::getRoundTime()), this->getVelo()->z));*/
		this->deactivateFallThrough();
		}
		else { // If dying
			this->switchStatusTo(CharacterStatus_Head::DIE_ALREADY);
			this->setVelo(D3DXVECTOR3(0.0F, TRUE_SPEED(GRAVITY, Timer::getRoundTime()), this->getVelo()->z));
		}

		return _currentCollisEvents;
}


bool Character::checkIfReadyToClimbFromSwim()
{
	// BUGGED
	FOR(_currentCollisEvents.size())
		if ((dynamic_cast<Tile*>(_currentCollisEvents[i]->getTouchedObject()))->getType() >= TileType::FOUR_FACE)
			if ((_currentCollisEvents[i]->getDirection() == Direction::RIGHT && this->getVelo()->x > 0) ||
				(_currentCollisEvents[i]->getDirection() == Direction::LEFT && this->getVelo()->x < 0))
				return true;

	return false;
}


void Character::dead()
{
	State::dead();
	_bonusBulletSpeed = 0;
	_immortalTime = RESPAW_IMMORTAL_TIME;
	changeBullet(BulletType::BASIC);
}

bool Character::affect(Object* obj, DWORD key)
{
	return false;
}

void Character::activateRapidShoot()
{
	_bonusBulletSpeed = RAPID_SHOOT_BONUS_SPEED;
}

void Character::activateImmortal()
{
	Object::activate(PROPERTIES_IMMORTAL);
	_immortalBulletActivate = true;
	_immortalTime = 0;
}

void Character::activateBoom()
{

}

bool Character::isTerminated()
{
	return false;
}


void Character::updateDefRect(Size boundRange)
{
	*_objectDefRect = Default::createRectFromBottomCenter(_position, boundRange);
}

int Character::getLifeRemaining()
{
	return _life;
}

void Character::bonusLife()
{
	_life += 1;
	// sound
}
