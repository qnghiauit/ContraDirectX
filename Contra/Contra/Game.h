#ifndef __GAME_H__
#define __GAME_H__

#include <time.h>

#include "DX9Framework\Windows.h"
#include "DX9Framework\Sprite.h"
#include "DX9Framework\Keyboard.h"
#include "DX9Framework\Text.h"
#include "DX9Framework\Sound.h"
#include "DX9Framework\Camera.h"

#include "GameState.h"

class Game
{
public:
	Game(HINSTANCE hInstance);
	~Game(void);
	void runGame();
	
private:
	static LRESULT CALLBACK WndProc(HWND WindowsHandle, UINT Message, WPARAM wParam, LPARAM lParam);

	void update();
	void draw();
	void initialize();
	void release();

private:
	Windows*		_displayWindow;
	Keyboard*	_keyboard;
};

#endif