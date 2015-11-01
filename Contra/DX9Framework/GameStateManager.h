#ifndef GAMESTATEMANAGER_H_
#define GAMESTATEMANAGER_H_

#include "GameState.h"
#include "T6_Texture.h"
//#include "Camera.h"
#include "vector"
#include <d3d9.h>
#include <d3dx9.h>
#include "T6_Windows.h"

using namespace std;
class CGameStateManager
{
private:
	
	HINSTANCE m_hInstance;
	HWND m_wndHandle;
	LPD3DXSPRITE m_spriteHandle;
	vector<CGameState*> m_gameState;
public:

	T6_Keyboard* m_input;
	T6_Texture* m_texture;
	CGameState* m_nextState;
	T6_Windows* m_windows;

	int GetScreenWidth();
	int GetScreenHight();

	CGameStateManager();
	~CGameStateManager();

	void Init(HINSTANCE, HWND, T6_Windows*, T6_Keyboard*);
	void CleanUp() {};
	void ChangeState(CGameState*);
	void PushState(CGameState*);
	void PopState();
	void Update(float);
	void Draw(LPD3DXSPRITE);
	//void Quit();
	void InitFirstState(CGameState*);
	

};
#endif;
