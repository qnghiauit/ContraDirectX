#include "State.h"

Keyboard* State::_controler = 0;
Character* State::_player = 0;
State* State::_stateList[CharacterStatus::NUM_OF_CHARACTER_STATUS];

#pragma region Pure Virtual Base State

void State::initializeCharacterState(Keyboard* keyboard, Character* player)
{
	State::_controler = keyboard;
	State::_player = player;

	State::_stateList[CharacterStatus::ON_GROUND] = new OnGround();
	State::_stateList[CharacterStatus::LIE_DOWN] = new LieDown();
	State::_stateList[CharacterStatus::JUMP_UP] = new JumpUp();
	State::_stateList[CharacterStatus::FALL_DOWN] = new FallDown();
	State::_stateList[CharacterStatus::DIE] = new Die();
	State::_stateList[CharacterStatus::SWIM] = new Swim();

	reset();
}


void State::releaseCharacterState()
{
	FOR_e(CharacterStatus::NUM_OF_CHARACTER_STATUS) {
		SAFE_RELEASE(State::_stateList[i]);
	}

	_player = 0;
	_controler = 0;
}


Keyboard* State::getControler()
{
	return _controler;
}


void State::collisBullet()
{
	BulletManager::updateIfObjectIsShooted(_player, KEY_PLAYER);
}


void State::reset()
{
	_player->setStatus(_stateList[CharacterStatus::JUMP_UP]);
	_player->setVelo(SPEED_NO);
}


State* State::getState(int index)
{
	return _stateList[index];
}


D3DXVECTOR3 State::getDrawPos(Texture* leg)
{
	D3DXVECTOR3 result(_player->getPosition());
	if (leg != nullptr)
		result.y += (float)leg->getFrameSize().height;
	return result;
}


void State::dead()
{
	_player->setStatus(_stateList[CharacterStatus::DIE]);
}


bool State::isWaterCollis()
{
	return true;
}


#pragma endregion

#pragma region Being on ground

// ------------------------------
// OnGround Class
// ------------------------------
void OnGround::enter()
{
	_player->switchStatusTo(CharacterStatus_Head::STRAIGHT);
	_player->switchLegStatusTo(CharacterStatus_Leg::STAND);
}


void OnGround::update()
{
	collisBullet();
	Map::affectedByGameObjects(_player, KEY_PLAYER | KEY_MOVEABLE);

	_player->updateMoveableWithCollision();

	_player->ShootObject::update();
}


void OnGround::draw(Texture* head, Texture* leg, DWORD fadedColor)
{
	D3DXVECTOR3 drawPos = getDrawPos(leg);
	leg->render(Timer::getRoundTime(), drawPos, 0, _player->getScale(), TRANSLATE_NO, DrawCenter::TOPCENTER, fadedColor);
	if (_player->checkIfCurrentSpriteIs(CharacterStatus_Head::DOWN45))
		drawPos.y += DISTANCE_FROM_HIP_TO_MUZZLE_DOWN45_Y;
	head->render(Timer::getRoundTime(), drawPos, 0, _player->getScale(), TRANSLATE_NO, DrawCenter::BOTTOMCENTER, fadedColor);
}


void OnGround::setMoveSprite()
{
	if (_controler->getKeyStatus(CK_UP) <= KeyStatus::KEY_PRESS)
		_player->switchStatusTo(CharacterStatus_Head::UP45);

	else if (_controler->getKeyStatus(CK_DOWN) <= KeyStatus::KEY_PRESS)
		_player->switchStatusTo(CharacterStatus_Head::DOWN45);

	else
		_player->switchStatusTo(CharacterStatus_Head::RUN_HEAD);

	_player->switchLegStatusTo(CharacterStatus_Leg::RUN_LEG);
}


void OnGround::controling()
{
	// Move left or right or stay if the keys aren't pressed.
	if (_controler->getKeyStatus(CK_RIGHT) <= KeyStatus::KEY_PRESS) {

		_player->getVelo()->x = MOVE_SPEED_RIGHT;

		setMoveSprite();
	}
	else if (_controler->getKeyStatus(CK_LEFT) <= KeyStatus::KEY_PRESS) {

		_player->getVelo()->x = MOVE_SPEED_LEFT;

		setMoveSprite();
	}
	else {
		if (_player->getVelo()->x != 0) {
			_player->getVelo()->x = 0;
			_player->switchLegStatusTo(CharacterStatus_Leg::STAND);
		}

		if (_controler->getKeyStatus(CK_UP) <= KeyStatus::KEY_PRESS)
			_player->switchStatusTo(CharacterStatus_Head::UP90);
		else
			_player->switchStatusTo(CharacterStatus_Head::STRAIGHT);
	}

	// Shoot
	if (_controler->getKeyStatus(CK_FIRE) <= KeyStatus::KEY_PRESS) {
		_player->shoot();
	}

	// Jump
	if (_controler->getKeyStatus(CK_JUMP) == KeyStatus::KEY_PRESS) {
		_player->setStatus(_stateList[CharacterStatus::JUMP_UP]);
	}

	// Lie down
	if (_controler->getKeyStatus(CK_DOWN) <= KeyStatus::KEY_PRESS) {
		if (_player->getVelo()->x == 0)
			_player->setStatus(_stateList[CharacterStatus::LIE_DOWN]);
	}

	// Dead
	if (_controler->getKeyStatus(DIK_U) <= KeyStatus::KEY_PRESS) {
		_player->setStatus(_stateList[CharacterStatus::DIE]);
	}

	// Cheatcode
	if (_controler->getKeyStatus(DIK_1) == KeyStatus::KEY_PRESS) {
		_player->changeBullet(BulletType::BASIC);
	}
	if (_controler->getKeyStatus(DIK_2) == KeyStatus::KEY_PRESS) {
		_player->changeBullet(BulletType::F);
	}
	if (_controler->getKeyStatus(DIK_3) == KeyStatus::KEY_PRESS) {
		_player->changeBullet(BulletType::L);
	}
	if (_controler->getKeyStatus(DIK_4) == KeyStatus::KEY_PRESS) {
		_player->changeBullet(BulletType::M);
	}
	if (_controler->getKeyStatus(DIK_5) == KeyStatus::KEY_PRESS) {
		_player->changeBullet(BulletType::S);
	}
	if (_controler->getKeyStatus(DIK_6) == KeyStatus::KEY_PRESS) {
		switch (Map::getCameraType())
		{
		case CAMERA_X:
			_player->fixPosition(D3DXVECTOR3(Map::getMapSize_() - 300, 100, 0) - _player->getPosition());
			break;

		case CAMERA_Y:
			_player->fixPosition(D3DXVECTOR3(100, Map::getMapSize_() - 300, 0) - _player->getPosition());
			break;

		default:
			break;
		}
	}
}


int OnGround::computeShootAngle(int& baseAngle)
{
	int directionKey = baseAngle > ANGLE_COUNTER_CLOCKWISE(90) ? CK_LEFT : CK_RIGHT;

	if (_controler->getKeyStatus(CK_UP) <= KeyStatus::KEY_PRESS)
		if (_controler->getKeyStatus(directionKey) <= KeyStatus::KEY_PRESS)
			if (directionKey == CK_RIGHT) return SHOOT_ANGLE_UP45;
			else return SHOOT_ANGLE_UP135;
		else return SHOOT_ANGLE_UP90;
	else
		if (_controler->getKeyStatus(CK_DOWN) <= KeyStatus::KEY_PRESS)
			if (_controler->getKeyStatus(directionKey) <= KeyStatus::KEY_PRESS)
				if (directionKey == CK_RIGHT) return SHOOT_ANGLE_DOWN45;
				else return SHOOT_ANGLE_DOWN135;

				return baseAngle;
}


CharacterStatus	OnGround::getIndex()
{
	return CharacterStatus::ON_GROUND;
}

Size OnGround::getBoundSize()
{
	return BOUND_SIZE_ON_GROUND;
}

#pragma endregion

#pragma region Lying down

// ------------------------------
// LieDown Class
// ------------------------------
void LieDown::enter()
{
	_player->switchStatusTo(CharacterStatus_Head::LIE);
}


void LieDown::update()
{
	collisBullet();/*

	Direction collisDirect = collisTile(Default::createRectFromBottomCenter);

	if (collisDirect == Direction::NONE) {
	player_->MoveObject::update();
	}
	else
	if (collisDirect == Direction::UP) {
	player_->setVelo(D3DXVECTOR3(player_->getVelo()->x, 0.0F, player_->getVelo()->z));
	}*/

	Map::affectedByGameObjects(_player, KEY_PLAYER | KEY_MOVEABLE);

	_player->updateDefRect(getBoundSize());

	_player->ShootObject::update();
}


void LieDown::draw(Texture* head, Texture* leg, DWORD fadedColor)
{
	head->render(Timer::getRoundTime(), _player->getPosition(), 0, _player->getScale(), TRANSLATE_NO, DrawCenter::BOTTOMCENTER, fadedColor);
}


void LieDown::controling()
{
	// Stand up
	if (_controler->getKeyStatus(CK_DOWN)	>= KeyStatus::KEY_RELEASE	||
		_controler->getKeyStatus(CK_LEFT)	<= KeyStatus::KEY_PRESS		||
		_controler->getKeyStatus(CK_RIGHT)	<= KeyStatus::KEY_PRESS) {
		_player->setStatus(_stateList[CharacterStatus::ON_GROUND]);
	}

	// Shoot
	if (_controler->getKeyStatus(CK_FIRE) <= KeyStatus::KEY_PRESS) {
		_player->shoot();
	}

	// Fall down
	if (_controler->getKeyStatus(CK_JUMP) <= KeyStatus::KEY_PRESS) {

		CollisEvent* standEvent = CollisEvent::isStandingOnSomething(&_player->getCollisEvent());

		if (standEvent != nullptr)
			if (dynamic_cast<Tile*>(standEvent->getTouchedObject())->getType() < TileType::TOP_UNDROP)
				_player->setStatus(_stateList[CharacterStatus::FALL_DOWN]);
	}

	// Dead
}


int LieDown::computeShootAngle(int& baseAngle)
{
	return baseAngle;
}


CharacterStatus	LieDown::getIndex()
{
	return CharacterStatus::LIE_DOWN;
}

Size LieDown::getBoundSize()
{
	return BOUND_SIZE_LIE;
}

#pragma endregion

#pragma region Jumping

// ------------------------------
// JumpUp Class
// ------------------------------
void JumpUp::enter()
{
	_player->switchStatusTo(CharacterStatus_Head::JUMP);
	_player->commandJump();
}


void JumpUp::update()
{
	collisBullet();
	Map::affectedByGameObjects(_player, KEY_PLAYER | KEY_MOVEABLE);

	_player->updateMoveableWithCollision();

	_player->ShootObject::update();
}


void JumpUp::draw(Texture* head, Texture* leg, DWORD fadedColor)
{
	head->render(Timer::getRoundTime(), _player->getPosition(), 0, _player->getScale(), TRANSLATE_NO, DrawCenter::BOTTOMCENTER, fadedColor);
}


void JumpUp::controling()
{
	// Move left or right or stay if the keys aren't pressed.
	if (_controler->getKeyStatus(CK_RIGHT) <= KeyStatus::KEY_PRESS) {
		_player->getVelo()->x = MOVE_SPEED_RIGHT;
	}
	else if (_controler->getKeyStatus(CK_LEFT) <= KeyStatus::KEY_PRESS) {
		_player->getVelo()->x = MOVE_SPEED_LEFT;
	}

	// Shoot
	if (_controler->getKeyStatus(CK_FIRE) <= KeyStatus::KEY_PRESS) {
		_player->shoot();
	}
}


int JumpUp::computeShootAngle(int& baseAngle)
{
	int directionKey = baseAngle > ANGLE_COUNTER_CLOCKWISE(90) ? CK_LEFT : CK_RIGHT;

	if (_controler->getKeyStatus(CK_UP) <= KeyStatus::KEY_PRESS)
		if (_controler->getKeyStatus(directionKey) <= KeyStatus::KEY_PRESS)
			if (directionKey == CK_RIGHT) return SHOOT_ANGLE_UP45;
			else return SHOOT_ANGLE_UP135;
		else return SHOOT_ANGLE_UP90;
	else
		if (_controler->getKeyStatus(CK_DOWN) <= KeyStatus::KEY_PRESS)
			if (_controler->getKeyStatus(directionKey) <= KeyStatus::KEY_PRESS)
				if (directionKey == CK_RIGHT) return SHOOT_ANGLE_DOWN45;
				else return SHOOT_ANGLE_DOWN135;

				// The difference between this state and OnGround state.
			else return SHOOT_ANGLE_DOWN90;

			return baseAngle;
}


CharacterStatus	JumpUp::getIndex()
{
	return CharacterStatus::JUMP_UP;
}

Size JumpUp::getBoundSize()
{
	return BOUND_SIZE_JUMPING;
}

#pragma endregion

#pragma region Falling from a tile

// ------------------------------
// FallDown Class
// ------------------------------
void FallDown::enter()
{
	_player->switchStatusTo(CharacterStatus_Head::FALL);
	_player->activateFallThrough();
}


void FallDown::update()
{
	collisBullet();
	Map::affectedByGameObjects(_player, KEY_PLAYER | KEY_MOVEABLE);

	_player->updateMoveableWithCollision();

	FallDown::controling();
}


void FallDown::draw(Texture* head, Texture* leg, DWORD fadedColor)
{
	head->render(Timer::getRoundTime(), _player->getPosition(), 0, _player->getScale(), TRANSLATE_NO, DrawCenter::BOTTOMCENTER, fadedColor);
}


void FallDown::controling()
{
	// Move left or right or stay if the keys aren't pressed.
	if (_controler->getKeyStatus(CK_RIGHT) <= KeyStatus::KEY_PRESS) {
		_player->getVelo()->x = MOVE_SPEED_RIGHT;
	}
	else if (_controler->getKeyStatus(CK_LEFT) <= KeyStatus::KEY_PRESS) {
		_player->getVelo()->x = MOVE_SPEED_LEFT;
	}

	// Shoot
	if (_controler->getKeyStatus(CK_FIRE) <= KeyStatus::KEY_PRESS) {
		_player->shoot();
	}

	// Landing

	// Dead
}


int FallDown::computeShootAngle(int& baseAngle)
{
	return baseAngle;
}


CharacterStatus	FallDown::getIndex()
{
	return CharacterStatus::FALL_DOWN;
}

Size FallDown::getBoundSize()
{
	return BOUND_SIZE_FALLING;
}

#pragma endregion

#pragma region Die state

// ------------------------------
// Die Class
// ------------------------------
void Die::enter()
{
	_showTime = TIME_DEATH;
	_player->getVelo()->y = JUMP_SPEED_DEAD;
	_player->getVelo()->x = MOVE_SPEED_LEFT * _player->getScale().x;
	_player->switchStatusTo(CharacterStatus_Head::DEAD);
	FreeSound::play(DIE_SOUND);
}


void Die::update()
{
	_player->ShootObject::update();

	_player->updateMoveableWithCollision();

	_showTime -= Timer::getRoundTime();

	if (_showTime <= 0 || (!Default::checkIfBounded(&_player->getRect(), &Camera::getViewPort()))) {
		_player->setStatus(_stateList[CharacterStatus::JUMP_UP]);
		_player->setVelo(D3DXVECTOR3(0.0F, GRAVITY * 5, _player->getVelo()->z));
		_player->reset();
	}
}


void Die::draw(Texture* head, Texture* leg, DWORD fadedColor)
{
	head->render(Timer::getRoundTime(), _player->getPosition(), 0, _player->getScale(), TRANSLATE_NO, DrawCenter::BOTTOMCENTER, fadedColor);
}


void Die::controling()
{

}


int Die::computeShootAngle(int& baseAngle)
{
	return baseAngle;
}


CharacterStatus	Die::getIndex()
{
	return CharacterStatus::DIE;
}

Size Die::getBoundSize()
{
	return BOUND_SIZE_DIE;
}

bool Die::isWaterCollis()
{
	return false;
}

#pragma endregion

#pragma region Swim state

// ======================================
// SWIM STATE
// ======================================
Sprite* Swim::_waterWave = nullptr;

void Swim::update()
{
	if (_currentSwimState != SwimSubState::DIVE)
	{
		collisBullet();
		Map::affectedByGameObjects(_player, KEY_PLAYER | KEY_MOVEABLE);
		_player->ShootObject::update();
		if (_currentSwimState != SwimSubState::CLIMB_UP)
			_player->updateMoveableWithCollision();
	}

	switch (_currentSwimState)
	{
	case FALL_IN_WATER:
		if (_subStateCompleted)
			substateSwitchTo(SwimSubState::SWIMING);

		_fallInWaterTime += Timer::getRoundTime();
		_subStateCompleted = (_fallInWaterTime >= FALL_IN_WATER_TIME);
		break;

	case SWIMING:
		if (_player->checkIfReadyToClimbFromSwim()) {
			substateSwitchTo(SwimSubState::CLIMB_UP);
			_player->switchStatusTo(CharacterStatus_Head::CLIMB);
		}

		if (_subStateCompleted)
			_floatPixel = ROUNDED_RANGE_INCREASE(_floatPixel, 1, 2);
		break;

	case CLIMB_UP:
		if (_subStateCompleted)
		{
			_player->setStatus(State::_stateList[CharacterStatus::ON_GROUND]);
			_player->fixPosition(CLIMB_DISTANCE);
		}
		break;

	case DIVE:
		break;
	default:
		substateSwitchTo(SwimSubState::FALL_IN_WATER);
		break;
	}
}


void Swim::draw(Texture* head, Texture* leg, DWORD fadedColor)
{
	_waterWave->render(Timer::getRoundTime(), _player->getPosition(), ANGLE_NO, SCALE_NO, TRANSLATE_NO, DrawCenter::CENTER);

	if (_currentSwimState != SwimSubState::DIVE)
		if (_currentSwimState != SwimSubState::FALL_IN_WATER)
			_subStateCompleted = head->render(Timer::getRoundTime(),
			D3DXVECTOR3(_player->getPosition().x, _player->getPosition().y - _floatPixel, DEFAULT_DEPTH),
			0, _player->getScale(), TRANSLATE_NO, DrawCenter::BOTTOMCENTER, fadedColor);
}


void Swim::substateSwitchTo(SwimSubState newState)
{
	_currentSwimState = newState;
	enter();
}


void Swim::enter()
{
	if (this->_waterWave == nullptr)
		_waterWave = new Sprite((tstring(LINK_WATER_WAVE) + EXTENSION_PICTURE).c_str(), WATER_WAVE_COLUMNS, WATER_WAVE_ROWS, WATER_WAVE_TOTAL_FRAMES);

	_subStateCompleted = false;

	switch (_currentSwimState)
	{
	case SwimSubState::FALL_IN_WATER:
		EffectManager::addWaterFlow(_player->getPosition());
		_fallInWaterTime = 0;
	case SwimSubState::CLIMB_UP:
	case SwimSubState::SWIMING:
	{
		_player->switchStatusTo(CharacterStatus_Head::SWIM_STILL);
		_player->deactivate(PROPERTIES_EVATION);
		break;
	}

	case SwimSubState::DIVE:
	{
		_player->activate(PROPERTIES_EVATION);
		break;
	}

	default:
		break;
	}
}


Swim::~Swim()
{
	SAFE_RELEASE(_waterWave);
}


void Swim::setShootSprite()
{
	if (_controler->getKeyStatus(CK_UP) <= KeyStatus::KEY_PRESS){
		if (_controler->getKeyStatus(CK_LEFT) <= KeyStatus::KEY_PRESS ||
			_controler->getKeyStatus(CK_RIGHT) <= KeyStatus::KEY_PRESS)
			_player->switchStatusTo(CharacterStatus_Head::UP45);
		else
			_player->switchStatusTo(CharacterStatus_Head::UP90);
	}
	else
		_player->switchStatusTo(CharacterStatus_Head::STRAIGHT);
}


void Swim::controling()
{
	// Move left or right or stay if the keys aren't pressed.
	if (_controler->getKeyStatus(CK_RIGHT) <= KeyStatus::KEY_PRESS) {

		_player->getVelo()->x = MOVE_SPEED_RIGHT;
	}
	else if (_controler->getKeyStatus(CK_LEFT) <= KeyStatus::KEY_PRESS) {

		_player->getVelo()->x = MOVE_SPEED_LEFT;
	}
	else {
		if (_player->getVelo()->x != 0) {
			_player->getVelo()->x = 0;
		}
	}

	if (_currentSwimState == SwimSubState::DIVE)
		if (_controler->getKeyStatus(CK_DOWN) == KeyStatus::KEY_RELEASE)
			substateSwitchTo(SwimSubState::SWIMING);

	// Shoot
	if (_controler->getKeyStatus(CK_FIRE) <= KeyStatus::KEY_PRESS) {
		setShootSprite();
		_player->shoot();
	}
	else
		_player->switchStatusTo(CharacterStatus_Head::SWIM_STILL);

	// Dive
	if (_controler->getKeyStatus(CK_DOWN) <= KeyStatus::KEY_PRESS)
		substateSwitchTo(SwimSubState::DIVE);
}


CharacterStatus	Swim::getIndex()
{
	return CharacterStatus::SWIM;
}


int Swim::computeShootAngle(int& baseAngle)
{
	int directionKey = baseAngle > ANGLE_COUNTER_CLOCKWISE(90) ? CK_LEFT : CK_RIGHT;

	if (_controler->getKeyStatus(CK_UP) <= KeyStatus::KEY_PRESS)
		if (_controler->getKeyStatus(directionKey) <= KeyStatus::KEY_PRESS)
			if (directionKey == CK_RIGHT) return SHOOT_ANGLE_UP45;
			else return SHOOT_ANGLE_UP135;
		else return SHOOT_ANGLE_UP90;

		return baseAngle;
}

Size Swim::getBoundSize()
{
	return BOUND_SIZE_SWIM;
}

#pragma endregion