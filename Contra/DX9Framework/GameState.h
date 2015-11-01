#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <d3d9.h>
#include "d3dx9.h"
#include "GameStateManager.h"
#include "T6_Keyboard.h"
#include "T6_Sprite.h"

enum GameStateId
{
	GAME_START,GAME_MENU,GAME_PLAY,GAME_END,GAME_OVER
};
class CGameState
{
protected :
	GameStateId m_id;
	CGameStateManager* m_game;
	T6_Sprite* m_background;
	float m_time;
	bool m_isQuit;
public:
	CGameState();
	~CGameState();
	CGameState(CGameStateManager*);
	virtual void Init();
	virtual void CleanUp() = 0;
	virtual void Pause() = 0;
	virtual void Resum() = 0;
	virtual void Update(T6_Keyboard* , float) = 0;
	virtual void Draw(LPD3DXSPRITE) =0;

	GameStateId GetGameStateId();
};
#endif

