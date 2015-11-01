
#include "GameState.h"


CGameState::CGameState()
{
}


CGameState::~CGameState()
{
}

CGameState::CGameState(CGameStateManager* _game)
{
	this->m_game = _game;
}

void CGameState::Init()
{
	m_isQuit = false;
	m_time = 0.0f;
	m_background = NULL;

}

GameStateId CGameState::GetGameStateId()
{
	return this->m_id;
}

