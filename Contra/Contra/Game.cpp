#include "Game.h"

Game::Game(HINSTANCE hInstance)
{
	_displayWindow = new Windows(hInstance, WINDOWS_NAME, true, SCREEN_WIDTH, SCREEN_HEIGHT, Game::WndProc);
	_displayWindow->StartingDevice();

	_keyboard = new Keyboard(hInstance, _displayWindow->getWindowHandler());

	Texture::initializeHandler(_displayWindow->getSpriteHandler());
	MergedTexture::initialize(Timer::getRoundCode, Timer::getRoundTime);
	Sprite::initialize(Timer::getRoundCode, Timer::getRoundTime);
	GameState::initialize(_keyboard);

	Game::initialize();
}



void Game::initialize()
{
	FreeSound::initialize(_displayWindow->getWindowHandler());
	FreeText::initialize(_displayWindow->getD3DDevice(), _displayWindow->getSpriteHandler());
}


Game::~Game(void)
{
	SAFE_RELEASE(_displayWindow);
	SAFE_RELEASE(_keyboard);
}



LRESULT CALLBACK Game::WndProc(HWND WindowsHandle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(WindowsHandle, Message, wParam, lParam);
}



void Game::update()
{
#if DEBUG == DEBUG_UPDATE_TIME
	clock_t time = clock();
#endif
	_keyboard->update();
	GameState::stateUpdate();
	Texture::setPositioncConvertMatrix(Camera::getTransform());
	Text::setPositioncConvertMatrix(Camera::getTransform());
	Sprite::synchronousProcessToNextFrame(Timer::getRoundTime());

#if DEBUG == DEBUG_UPDATE_TIME
	time = clock() - time;
	OutputDebugString(Default::tostr(time).c_str());
	OutputDebugString(L"\n");
#endif
}



void Game::draw()
{
#if DEBUG == DEBUG_DRAW_TIME
	clock_t time = clock();
#endif
	if (_displayWindow->startDraw()) {
		GameState::stateDraw();
		_displayWindow->stopDraw();
	}
#if DEBUG == DEBUG_DRAW_TIME
	time = clock() - time;
	OutputDebugString(Default::tostr(time).c_str());
	OutputDebugString(L"\n");
#endif
	}



void Game::runGame()
{
	MSG Message;
	ZeroMemory(&Message, sizeof(Message));

	while (Message.message != WM_QUIT) {
		update();
		draw();

		if (PeekMessage(&Message, 0, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		// get current round time for sleep
		Timer::updateRoundTime();

		if (Timer::getRoundTime() < FRAME_TIME)
			SleepEx(FRAME_TIME - Timer::getRoundTime(), false);

		Timer::updateTimeSign();

#if DEBUG >= DEBUG_FPS
		static double lastSecFPS = 0.0;
		static DWORD fcounter = 0;

		if (fcounter >= 1000)
		{
			_displayWindow->setWindowTitle((
				tstring(WINDOWS_NAME) + tstring(L" FPS: ") +
				Default::tostr((int)(lastSecFPS / FPS + 2.5))
				).c_str());

			lastSecFPS = 0.0;
			fcounter = 0;
		}
		else
		{
			lastSecFPS += 1000.0 / Timer::getRoundTime();
			fcounter += Timer::getRoundTime();
		}
		
#endif
	}

	release();
}


void Game::release()
{
	Texture::releaseHandler();
	FreeSound::release();
	GameState::release();
}
