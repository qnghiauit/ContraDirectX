#include "GameState.h"

#pragma region Game State
Keyboard* GameState::_controler = nullptr;
MainMenu* MainMenu::_instance = nullptr;

GameState* GameState::_gameState = nullptr;

Character* GameState::_player = nullptr;


void GameState::initialize(Keyboard* controler)
{
	_controler = controler;

	_player = new Character(DEFAULT_STARTPOINT(0, 0));
	State::initializeCharacterState(_controler, _player);
	Map::assignPlayer(_player);

	switchState(MainMenu::get());
}

void GameState::release()
{
	SAFE_RELEASE(_player);
}


void GameState::stateUpdate()
{
	_gameState->update();
}


void GameState::switchState(GameState* newState)
{
	_gameState = newState;
	_gameState->enter();
}


void GameState::stateDraw()
{
	_gameState->draw();
}

void GameState::update()
{

}

void GameState::draw()
{

}
#pragma endregion

#pragma region Main Menu
MainMenu::MainMenu()
{
	_billAndLance = new Texture(MAIN_MENU_RESOURCE_BILL.c_str());
	_contraLogo = new Texture(MAIN_MENU_RESOURCE_CONTRA_LOGO.c_str());
	_konamiLogo = new Texture(MAIN_MENU_RESOURCE_KONAMI_LOGO.c_str());

	_selector = new Selector(MAIN_MENU_SELECTION_POS);

	_selector->assignSelection(MAIN_MENU_SELECTION_START);
	_selector->assignSelection(MAIN_MENU_SELECTION_CREDIT);

	enter();
}

MainMenu::~MainMenu()
{
	SAFE_RELEASE(_billAndLance);
	SAFE_RELEASE(_contraLogo);
}

void MainMenu::update()
{
	if (_selectedOne != nullptr)
	{
		_timeCounter += Timer::getRoundTime();

		if (_timeCounter >= MAIN_MENU_SELECT_TIME)
		{
			if (_selectedOne == MAIN_MENU_SELECTION_START)
			{
				switchState(StartingState::get());
			}
			else if (_selectedOne == MAIN_MENU_SELECTION_CREDIT)
			{
				// switch to credit state
			}
		}
	}
	else
		if (Camera::getState() == CAMERA_STOP)
		{
		if (!_isPlayedSound)
		{
			FreeSound::play(SOUND_MAINMENU_BACKGOUND, false, false);
			_isPlayedSound = true;
		}

		if (_controler->getKeyStatus(CK_START) == KeyStatus::KEY_PRESS)
		{
			_selectedOne = _selector->select();
		}
		}
		else
		{
			if (_controler->getKeyStatus(CK_START) == KeyStatus::KEY_PRESS)
			{
				Camera::setPosition(POSITION_ZERO);
				Camera::activateMove(POSITION_ZERO, CAMERA_STOP);
			}
		}

	if (_controler->getKeyStatus(CK_SELECT) == KeyStatus::KEY_PRESS)
	{
		_selector->toNextSelection();
	}

	cameraUpdate();
}

void MainMenu::draw()
{
	if (Camera::getState() == CameraState::CAMERA_STOP)
	{
		_billAndLance->drawImage(DRAWPOS_BILL_AND_LANCE, nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::TOPLEFT);
	}

	_contraLogo->drawImage(DRAWPOS_CONTRA_LOGO, nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::TOPLEFT);
	_selector->draw(Camera::getState() == CameraState::CAMERA_STOP);
	_konamiLogo->drawImage(DRAWPOS_KONAMI_LOGO, nullptr, SCALE_NO, TRANSLATE_NO, DrawCenter::TOPLEFT);
	FreeText::get().drawText(MAIN_MENU_LISENCE, MAIN_MENU_LISENCE_POS, DEFAULT_FONT_COLOR, DEFAULT_FONTSIZE, DT_CENTER);
	FreeText::get().drawText(MAIN_MENU_SELECT_ORDER, DRAWPOS_SELECT_ORDER, DEFAULT_FONT_COLOR, DEFAULT_FONTSIZE, DT_LEFT);
}

MainMenu* MainMenu::get()
{
	if (_instance == nullptr)
		_instance = new MainMenu();

	return _instance;
}

void MainMenu::enter()
{
	D3DXVECTOR3 cameraPos = POSITION_ZERO;
	cameraPos.x -= RESOLUTION_WIDTH;

	Camera::setPosition(cameraPos);

	cameraPos.x = -cameraPos.x;
	Camera::activateMove(cameraPos);

	_isPlayedSound = false;
	_selectedOne = nullptr;
	_timeCounter = 0;
	Map::reset();
	_selector->reset();
}

void MainMenu::cameraUpdate()
{
	Camera::update(POSITION_ZERO, Timer::getCameraRoundTime());
}

#pragma endregion

#pragma region Starting

StartingState* StartingState::_instance = nullptr;

StartingState::StartingState()
{
	_isFlashOn = true;
	_timeCounter = 0;
}

StartingState::~StartingState()
{

}

void StartingState::update()
{
	_timeCounter += Timer::getRoundTime();
	if (_timeCounter >= STARTING_FLASH_TIME)
	{
		_timeCounter = 0;
		_isFlashOn = !_isFlashOn;
	}

	_timeCounterToNextState += Timer::getRoundTime();

	if (_timeCounterToNextState >= STARTING_STATE_TIME)
	{
		switchState(PlayingState::get());
	}
}

void StartingState::draw()
{
	FreeText::get().drawText(STARTING_STRING_1P, STARTING_STRING_POS_SCORE);
	FreeText::get().drawText(STARTING_STRING_REST, STARTING_STRING_POS_REST);
	FreeText::get().drawText(STARTING_STRING_HIGH_S, STARTING_STRING_POS_HIGH_S);
	FreeText::get().drawText(tstring(STARTING_STRING_STAGE) + Default::tostr(Map::getStageIndex()), STARTING_POS_STAGE_INDEX);
	FreeText::get().drawText(Map::getStageName(), STARTING_POS_STAGE_NAME);
	FreeText::get().drawText(_player->getLifeRemaining(), STARTING_POS_REST);

	if (_isFlashOn)
	{
		FreeText::get().drawText(ScoreManager::getScore(), STARTING_POS_SCORE);
		FreeText::get().drawText(ScoreManager::getHighScore(), STARTING_POS_HIGH_S);
	}
}

void StartingState::enter()
{
	Map::reload();
	Camera::setPosition(POSITION_ZERO);
	_timeCounter = 0;
	_timeCounterToNextState = 0;
}

StartingState* StartingState::get()
{
	if (_instance == nullptr)
	{
		_instance = new StartingState();
	}

	return _instance;
}

void StartingState::cameraUpdate()
{
}

#pragma endregion

#pragma region Playing

PlayingState* PlayingState::_instance = nullptr;

void PlayingState::update()
{
	cameraUpdate();
	if (Camera::getState() != CAMERA_AUTO)
	{
		_player->update();
		Map::update();
		BulletManager::update();

		if (_player->getLifeRemaining() < 0)
		{
			FreeSound::stop(Map::getSoundtrackLink());
			switchState(DeadScene::get());
		}

		if (Camera::getState() != CAMERA_STOP)
		{
			if (Map::getCameraType() == CAMERA_X)
			{
				if (Camera::getViewPort().right >= (Map::getMapSize_() - RESOLUTION_WIDTH / 2.0))
					Camera::activateMove(SPEED_X(Map::getMapSize_() - Camera::getViewPort().right));
			}
			else
			{
				if (Camera::getViewPort().top >= (Map::getMapSize_() - RESOLUTION_HEIGHT / 2.0))
					Camera::activateMove(SPEED_Y(Map::getMapSize_() - Camera::getViewPort().top));
			}
		}
	}
}

void PlayingState::draw()
{
	Map::getInstance().draw();
	_player->draw();
	BulletManager::getInstance().draw();
	EffectManager::draw();
}

void PlayingState::enter()
{
	_player->setPosition(Map::getPlayerSpawPos());
	Camera::setPosition(POSITION_ZERO);
	Camera::activateMove(POSITION_ZERO, CAMERA_NORMAL);
	FreeSound::play(Map::getSoundtrackLink(), true, true);
}

PlayingState::PlayingState()
{

}

PlayingState::~PlayingState()
{

}

void PlayingState::cameraUpdate()
{
	if (_player->getCurrentStatus()->getIndex() != CharacterStatus::DIE)
		Camera::update(_player->getPosition(), Timer::getCameraRoundTime(), Map::getCameraType());
}

PlayingState* PlayingState::get()
{
	if (_instance == nullptr)
	{
		_instance = new PlayingState();
	}

	return _instance;
}


#pragma endregion

#pragma region Dead State
void DeadScene::update()
{
	_timeCounter += Timer::getRoundTime();
	if (_timeCounter >= STARTING_FLASH_TIME)
	{
		_timeCounter = 0;
		_isFlashOn = !_isFlashOn;
	}

	if (_controler->getKeyStatus(CK_START) <= KeyStatus::KEY_PRESS)
	{
		ScoreManager::reset();
		if (_deadTimesRemaining <= 0)
		{
			_deadTimesRemaining = DEAD_STATE_APPEAR_TIMES;
			switchState(MainMenu::get());
		}

		if (_selector->getSelectionName() == DEAD_STATE_SELECTOR_CONTINUE)
		{
			this->_deadTimesRemaining--;
			switchState(StartingState::get());
			_player->setBasicProperties();
		}
		else
		{
			_deadTimesRemaining = DEAD_STATE_APPEAR_TIMES;
			switchState(MainMenu::get());
		}
	}

	if (_controler->getKeyStatus(CK_SELECT) == KeyStatus::KEY_PRESS)
	{
		_selector->toNextSelection();
	}
}

void DeadScene::draw()
{
	FreeText::get().drawText(DEAD_STATE_STRING_1P, DEAD_STATE_STRING_POS_SCORE);
	FreeText::get().drawText(DEAD_STATE_STRING_HIGH_S, DEAD_STATE_STRING_POS_HIGH_S);
	FreeText::get().drawText(DEAD_STATE_STRING_GAMEOVER, DEAD_STATE_STRING_POS_GAMEOVER);

	if (_isFlashOn)
	{
		FreeText::get().drawText(ScoreManager::getScore(), STARTING_POS_SCORE);
		FreeText::get().drawText(ScoreManager::getHighScore(), STARTING_POS_HIGH_S);
	}

	if (_deadTimesRemaining > 0)
		_selector->draw(true);
}

void DeadScene::enter()
{
	_player->setBasicProperties();
	Camera::setPosition(POSITION_ZERO);
	_timeCounter = 0;
	_selector->reset();
}

void DeadScene::cameraUpdate()
{
}

DeadScene* DeadScene::get()
{
	if (_instance == nullptr)
	{
		_instance = new DeadScene();
	}

	return _instance;
}

DeadScene::DeadScene()
{
	_isFlashOn = true;
	_timeCounter = 0;
	_deadTimesRemaining = DEAD_STATE_APPEAR_TIMES;
	_selector = new Selector(DEAD_STATE_POS_SELECTOR);
	_selector->assignSelection(DEAD_STATE_SELECTOR_CONTINUE);
	_selector->assignSelection(DEAD_STATE_SELECTOR_END);
}

DeadScene::~DeadScene()
{

}

DeadScene* DeadScene::_instance = nullptr;

#pragma endregion
