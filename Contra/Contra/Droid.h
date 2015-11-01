#ifndef Droid_h__
#define Droid_h__

#include "Boss.h"
#include "MiniDroid.h"
#include "ExplodeFire.h"

#define DROID_BASE_RESOURCE_LINK	(tstring(_T("Resource//Bosses//PhantomDroid//DroidBase")) + EXTENSION_PICTURE)
#define DROID_BASE_RESOURCE_ROWS	1
#define DROID_BASE_RESOURCE_COLUMNS	1
#define DROID_FIRE_RESOURCE_LINK	(tstring(_T("Resource//Bosses//PhantomDroid//DroidFire")) + EXTENSION_PICTURE)
#define DROID_FIRE_RESOURCE_ROWS	1
#define DROID_FIRE_RESOURCE_COLUMNS	2
#define DROID_FIRE_TRANSLATE		D3DXVECTOR2(0.0F, -15.0F)
#define DROID_DOOR_RESOURCE_LINK	(tstring(_T("Resource//Bosses//PhantomDroid//DroidDoor")) + EXTENSION_PICTURE)
#define DROID_DOOR_RESOURCE_ROWS	4
#define DROID_DOOR_RESOURCE_COLUMNS	1
#define DROID_DOOR_TRANSLATE		D3DXVECTOR2(0.0F, -37.0F)
#define DROID_STATIC_BASE			(tstring(_T("Resource//Bosses//PhantomDroid//StaticBase")) + EXTENSION_PICTURE)
#define DROID_LIVING_BASE			(tstring(_T("Resource//Bosses//PhantomDroid//LivingBase")) + EXTENSION_PICTURE)
#define DROID_BOUND_SIZE	Size(60.0F, 64.0F)
#define DROID_BASE_WIDTH	104
#define DROID_PREPARING_TIME	2000
#define DROID_FADE_TIME			1000
#define DROID_START_POS_FIX		D3DXVECTOR3(-114.0F, 114.0F, 0.0F)
#define DROID_GENERATE_TIME		(MINI_DROID_WAIT_TIME * 7)
#define DROID_GENERATE_DELAY	(MINI_DROID_WAIT_TIME)
#define DROID_MINI_GENERATE_POS_FIX(side)	(D3DXVECTOR3(side == Side::SIDE_LEFT ? -22.0F : 22.0F, 45.0F, 0.0F))
#define DROID_FIRE_GENERATE_POS_FIX	(D3DXVECTOR3(0.0F, 45.0F, 0.0F))
#define DROID_LIFE_POINTS		300

enum DroidState
{
	DROID_APPEAR = 0,
	DROID_DISSAPEAR,
	DROID_OPENING,
	DROID_GENERATING,
	DROID_CLOSING,
	DROID_PREPARING
};

class Droid :
	public Boss
{
public:
	Droid(D3DXVECTOR3 pos);
	virtual ~Droid();

	virtual void draw();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual void deadEffect();

	virtual void setBasicProperties();

	virtual bool affect(Object* obj, DWORD key);

	virtual void update();

private:
	vector<Mobs*> generatedObjects_;
	DWORD timer_;
	Texture* staticBase_;
	Texture* livingBase_;
	Sprite* fire_;
	Sprite* door_;
	D3DXVECTOR3 basePos_;
	RECT randomRegion_;
	DWORD fadeColor_;
	DroidState state_;
	DWORD generateTimer_;
	Side generateSide_;
	int fadeTimer_;
};

#endif // Droid_h__
