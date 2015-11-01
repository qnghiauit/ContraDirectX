#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <d3d9.h>
#include <d3dx9.h>
#include "DefaultConstant.h"
#include "GraphicAlgorimths2D/MathDefaultConstant.h"

#define CAMERA_X (1)
#define CAMERA_Y (-1)
#define CAMERA_MOVE_SPEED	50

enum CameraState
{
	CAMERA_NORMAL = 0,
	CAMERA_AUTO,
	CAMERA_STOP,
	NUM_OF_CAMERA_STATE
};

class Camera
{
public:
	static void update(D3DXVECTOR3 pos, DWORD deltaTime, int cameraAxis = CAMERA_X);
	static D3DXMATRIX getTransform();
	static D3DXVECTOR3 getPosition();

	static RECT getViewPort();

	static Camera* getInstance();

	static void activateMove(D3DXVECTOR3 distance, CameraState endState = CameraState::CAMERA_STOP);

	static void setPosition(D3DXVECTOR3 pos);

	static CameraState getState();

private:
	Camera();
	~Camera();

	static void normalUpdate(D3DXVECTOR3 pos, int cameraAxis);
	static void autoUpdate(DWORD deltaTime);
	
	static void	computeViewPort();

private:
	D3DXMATRIX _transform;
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _moveDistance;
	CameraState _state;
	CameraState _nextState;
	RECT _view;
	static Camera* _instance;
};

#endif