#ifndef __CROSSHAIR_H__
#define __CROSSHAIR_H__

#include "Boss.h"
#include "BoomingCanon.h"
#include "HiddenSniper.h"

#define CROSSHAIR_LINK						_T("Resource//Bosses//TheWall//")
#define CROSSHAIR_RESOURCE_LINK				(tstring(CROSSHAIR_LINK) + _T("CrossHair") + EXTENSION_PICTURE)
#define CROSSHAIR_STATIC_BASE_LINK			(tstring(CROSSHAIR_LINK) + _T("StaticBase") + EXTENSION_PICTURE)
#define CROSSHAIR_LIVING_BASE_LINK			(tstring(CROSSHAIR_LINK) + _T("LivingBase") + EXTENSION_PICTURE)
#define CROSSHAIR_NUM_OF_COLOR_MAPS			2
#define CROSSHAIR_NUM_OF_RESOURCE_ROW		1
#define CROSSHAIR_NUM_OF_RESOURCE_COLUMNS	2
#define CROSSHAIR_HEALTH_POINTS				100
#define CROSSHAIR_BOUND_SIZE				Size(24.0, 32.0)
#define CROSSHAIR_STATIC_BASE_TRANSLATE		D3DXVECTOR2(-20.0F, 62.0F)
#define CROSSHAIR_LIVING_BASE_TRANSLATE		D3DXVECTOR2(-18.0F, 22.0F)
#define LIGHT_TIME							DEFAULT_FRAME_TIME
#define LIGHT_COLOR							DWORD lightColor[] = { 0xFFFF8C7C, 0xFFE44434, 0xFFB81C0C, 0xFFE44434 }
#define NUM_OF_LIGHT_COLOR					4
#define CANON_1_TRANSLATE					D3DXVECTOR3(18.0F, 48.0F, 0.0F)
#define CANON_2_TRANSLATE					D3DXVECTOR3(-5.0F, 48.0F, 0.0F)
#define SNIPER_TRANSLATE					D3DXVECTOR3( 5.0f, 90.0F, 0.0F)


class CrossHair :
	public Boss
{
public:
	CrossHair(D3DXVECTOR3 position, Object* target);
	virtual ~CrossHair();

	virtual void draw();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual void deadEffect();

	virtual void setBasicProperties();

	virtual bool affect(Object* obj, DWORD key);

			bool lightTimeCounter(DWORD timeCap);

private:
	Texture* _staticBase;
	Texture* _livingBase;
};

#endif // CrossHair_h__