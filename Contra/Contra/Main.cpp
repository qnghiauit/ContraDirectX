#include <Windows.h>
#include "Game.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmpLine, int nCmpShow)
{
	Game* contraGame = new Game(hInstance);
	contraGame->runGame();
	SAFE_RELEASE(contraGame);
	return 0;
}