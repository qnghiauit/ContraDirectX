#include "Camera.h"

Camera* Camera::_instance = 0;


Camera::Camera()
{
	_position = POSITION_ZERO;
	_moveDistance = POSITION_ZERO;
	D3DXMatrixIdentity(&_transform);
	_state = CAMERA_NORMAL;
}


Camera::~Camera()
{
	SAFE_RELEASE(_instance);
}


Camera* Camera::getInstance()
{
	if (_instance == 0)
		_instance = new Camera();
	return _instance;
}


void Camera::update(D3DXVECTOR3 pos, DWORD deltaTime, int cameraAxis)
{
	switch (Camera::getInstance()->_state)
	{
	case CAMERA_NORMAL:
		normalUpdate(pos, cameraAxis);
		break;
	case CAMERA_AUTO:
		autoUpdate(deltaTime);
		break;
	case CAMERA_STOP:
		break;
	default:
		break;
	}
}


D3DXMATRIX Camera::getTransform()
{
	D3DXMATRIX mtScale;
	D3DXMatrixScaling(&mtScale, 1, -1, 0);

	D3DXMATRIX mtTrans;
	D3DXMatrixTranslation(&mtTrans, -(Camera::getInstance()->_position.x), 
		-(Camera::getInstance()->_position.y + RESOLUTION_HEIGHT), 
		Camera::getInstance()->_position.z);

	Camera::getInstance()->_transform = mtTrans * mtScale;
	return Camera::getInstance()->_transform;
}


D3DXVECTOR3 Camera::getPosition()
{
	return Camera::getInstance()->_position;
}


RECT Camera::getViewPort()
{
	return Camera::getInstance()->_view;
}


void Camera::activateMove(D3DXVECTOR3 distance, CameraState endState)
{
	Camera::getInstance()->_moveDistance = distance;
	Camera::getInstance()->_state = CameraState::CAMERA_AUTO;
	Camera::getInstance()->_nextState = endState;
}

void Camera::normalUpdate(D3DXVECTOR3 pos, int cameraAxis)
{
	D3DXVECTOR3* position = &Camera::getInstance()->_position;

	switch (cameraAxis)
	{
	case CAMERA_X:
		if (pos.x > position->x + RESOLUTION_WIDTH / 2.0F)
			position->x = pos.x - RESOLUTION_WIDTH / 2.0F;
		break;
	case CAMERA_Y:
		if (pos.y > position->y + RESOLUTION_HEIGHT / 2.0F)
			position->y = pos.y - RESOLUTION_HEIGHT / 2.0F;
		break;
	default:
		break;
	}

	Camera::getInstance()->computeViewPort();
}

void Camera::autoUpdate(DWORD deltaTime)
{
	D3DXVECTOR3* position = &Camera::getInstance()->_position;
	RECT* view = &Camera::getInstance()->_view;
	D3DXVECTOR3* moveDistance = &Camera::getInstance()->_moveDistance;

	float moveStep = TRUE_SPEED(CAMERA_MOVE_SPEED, deltaTime) * GET_SIGN(moveDistance->x);

	if (fabs(moveDistance->x) < moveStep)
	{
		position->x += moveDistance->x;
		moveDistance->x = 0.0F;
	}
	else
	{
		position->x += moveStep;
		moveDistance->x -= moveStep;
	}

	moveStep = TRUE_SPEED(CAMERA_MOVE_SPEED, deltaTime) * GET_SIGN(moveDistance->y);

	if (fabs(moveDistance->y) < moveStep)
	{
		position->y += moveDistance->y;
		moveDistance->y = 0.0F;
	}
	else
	{
		position->y += moveStep;
		moveDistance->y -= moveStep;
	}

	if (*moveDistance == POSITION_ZERO)
		Camera::getInstance()->_state = Camera::getInstance()->_nextState;

	Camera::getInstance()->computeViewPort();
}


void Camera::computeViewPort()
{
	RECT* view = &Camera::getInstance()->_view;

	view->left = (LONG)Camera::getInstance()->_position.x;
	view->bottom = (LONG)Camera::getInstance()->_position.y;
	view->right = (LONG)view->left + RESOLUTION_WIDTH;
	view->top = (LONG)view->bottom + RESOLUTION_HEIGHT;

	if (view->left < 0) {
		view->right -= view->left;
		view->left = 0;
	}

	if (view->bottom < 0) {
		view->top -= view->bottom;
		view->bottom = 0;
	}
}


void Camera::setPosition(D3DXVECTOR3 pos)
{
	Camera::getInstance()->_position = pos;
	Camera::getInstance()->computeViewPort();
}


CameraState Camera::getState()
{
	return Camera::getInstance()->_state;
}