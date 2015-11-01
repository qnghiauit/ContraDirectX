#ifndef __MOUTH_H__
#define __MOUTH_H__

#include "Boss.h"
#include "ShootObject.h"
#include "Tentacle.h"

#define MOUTH_RESOURCE_HEAD	(tstring(_T("Resource//Bosses//ShadowEntrance//Head")) + EXTENSION_PICTURE)
#define MOUTH_RESOURCE_MOUTH	(tstring(_T("Resource//Bosses//ShadowEntrance//Mouth")) + EXTENSION_PICTURE)
#define MOUTH_RESOURCE_HEAD_ROWS	1
#define MOUTH_RESOURCE_HEAD_COLUMNS	2
#define MOUTH_RESOURCE_MOUTH_ROWS	1
#define MOUTH_RESOURCE_MOUTH_COLUMNS	3
#define MOUTH_FRAME_NUM				3
#define MOUTH_STATIC_BASE	(tstring(_T("Resource//Bosses//ShadowEntrance//StaticBase")) + EXTENSION_PICTURE)
#define MOUTH_LIVING_BASE	(tstring(_T("Resource//Bosses//ShadowEntrance//LivingBase")) + EXTENSION_PICTURE)
#define MOUTH_MOUTH_TRANSLATE	D3DXVECTOR3(0.0f, 160.0f, 0.0f)
#define MOUTH_STATIC_BASE_TRANSLATE	D3DXVECTOR2(0.0f, 0.0f)
#define MOUTH_LIVING_BASE_TRANSLATE	D3DXVECTOR2(0.0f, -22.0f)
#define MOUTH_HEAD_TRANSLATE	D3DXVECTOR2(0.0f, -30.0f)
#define MOUTH_EYES_COLORS		DWORD eyeColors[] = {0xFFB8005C, 0xFFFF00A0, 0xFFFFADF9, 0xFFFF00A0}
#define MOUTH_EYES_NUM_OF_COLOS	4
#define MOUTH_SHOOT_ANGLES		int shootAngles[] = {270, 255, 285}
#define MOUTH_NUM_OF_SHOOT_ANGLE	3
#define MOUTH_CLOSE_TIME	3000	// 3s
#define MOUTH_OPEN_TIME		5000	// 5s
#define MOUTH_LIFE_POINTS	100
#define MOUTH_BOUND_SIZE	Size(30.0, 49.0)
#define MOUTH_TENTACLE_L_POS_FIX	D3DXVECTOR3(-51.0f, -41.0f, 0.0f)
#define MOUTH_TENTACLE_R_POS_FIX	D3DXVECTOR3(51.0f, -41.0f, 0.0f)
#define MOUTH_TENTACLE_L_ANGLE		ANGLE_COUNTER_CLOCKWISE(135)
#define MOUTH_TENTACLE_R_ANGLE		ANGLE_COUNTER_CLOCKWISE(45)
#define MOUTH_FADE_SPEED			60.0f

enum MouthStatus
{
	MOUTH_CLOSE = 0,
	MOUTH_OPENING,
	MOUTH_CLOSING,
	MOUTH_OPEN
};

class Mouth : 
	public Boss,
	public ShootObject
{
public:
	Mouth(D3DXVECTOR3 pos, Object* player);
	virtual ~Mouth();

	virtual unsigned int getScore();

	virtual void draw();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadEffect();

	virtual void setBasicProperties();

	virtual void update();

	virtual int computeShootAngle();

	virtual D3DXVECTOR3 computeShootPoint();

	virtual bool affect(Object* obj, DWORD key);

	virtual bool isTerminated();

	virtual void activate();

private:
	Texture* _staticBase;
	Texture* _livingBase;
	MergedTexture* _head;
	int _currentEyeColorIndex;
	DWORD _timeCounter;
	D3DXVECTOR3 basePos_;
	Object* _target;
	DWORD _opacity;
};

#endif // Mouth_h__
