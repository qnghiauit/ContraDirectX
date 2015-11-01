
#include "GameStateManager.h"


CGameStateManager::CGameStateManager()
{
}


CGameStateManager::~CGameStateManager()
{
}

void CGameStateManager::InitFirstState(CGameState* _state)
{
	m_gameState.push_back(_state); // chu y
	m_gameState.back()->Init();
}

void CGameStateManager::Init(HINSTANCE _hInstance, HWND _wndHandle, T6_Windows* _windows, T6_Keyboard* _input)
{
	this->m_hInstance = _hInstance;
	this->m_wndHandle = _wndHandle;
	this->m_input = _input;
	this->m_windows = _windows;
	D3DXCreateSprite(m_windows->getD3DDevice(), &m_spriteHandle);
}

void CGameStateManager::CleanUp()
{

}

void CGameStateManager::ChangeState(CGameState* _nextState)
{
	this->m_nextState = _nextState;
	if (!m_gameState.empty())
	{
		m_gameState.back()->CleanUp();
		m_gameState.pop_back();
	}
	m_gameState.push_back(m_nextState);
	m_gameState.back()->Init();
}

void CGameStateManager::PushState(CGameState* _nextState)
{ 
	this->m_nextState = _nextState;
	if (!m_gameState.empty())
	{
		m_gameState.back()->Pause();
	}
	m_gameState.push_back(m_nextState);
	m_gameState.back()->Init();
}

void CGameStateManager::PopState()
{
	if (!m_gameState.empty())
	{
		m_gameState.back()->CleanUp();
		m_gameState.pop_back();
	}
}

void CGameStateManager::Update(float _time)
{
	m_gameState.back()->Update(m_input, _time);
}

void CGameStateManager::Draw(LPD3DXSPRITE _spriteHandler)
{
	m_gameState.back()->Draw(_spriteHandler);
}

int CGameStateManager::GetScreenHight()
{
	return RESOLUTION_HEIGHT;
}

int CGameStateManager::GetScreenWidth()
{
	return RESOLUTION_WIDTH;
}