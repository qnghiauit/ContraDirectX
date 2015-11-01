#ifndef __EFFECT_MANAGER_H__
#define __EFFECT_MANAGER_H__

#include "Effect.h"

#pragma region Normal Explode
#define	EFFECT_EXPLODE_LINK		(tstring(_T("Resource//Miscellaneous//Effects//Explode")) + EXTENSION_PICTURE)
#define EFFECT_EXPLODE_ROWS		1
#define	EFFECT_EXPLODE_COLUMNS	5
#define EFFECT_EXPLODE_FRAME_TIME (DEFAULT_FRAME_TIME)
#pragma endregion

#pragma region Small Explode
#define	EFFECT_EXPLODE_SMALL_LINK		(tstring(_T("Resource//Miscellaneous//Effects//Explode_Small")) + EXTENSION_PICTURE)
#define EFFECT_EXPLODE_SMALL_ROWS		1
#define	EFFECT_EXPLODE_SMALL_COLUMNS	3
#define EFFECT_EXPLODE_SMALL_FRAME_TIME (DEFAULT_FRAME_TIME)
#pragma endregion

#pragma region Bullet Explode
#define	EFFECT_EXPLODE_BULLET_LINK		(tstring(_T("Resource//Miscellaneous//Effects//Explode_Bullet")) + EXTENSION_PICTURE)
#define EFFECT_EXPLODE_BULLET_ROWS		1
#define	EFFECT_EXPLODE_BULLET_COLUMNS	3
#define EFFECT_EXPLODE_BULLET_FRAME_TIME (DEFAULT_FRAME_TIME / 5.0)
#pragma endregion

#pragma region Bullet Explode
#define	EFFECT_WATER_FLOW_LINK			(tstring(_T("Resource//Miscellaneous//Effects//WaterFlow")) + EXTENSION_PICTURE)
#define EFFECT_WATER_FLOW_ROWS			1
#define	EFFECT_WATER_FLOW_COLUMNS		2
#define EFFECT_WATER_FLOW_FRAME_TIME	(DEFAULT_FRAME_TIME * 2.0)
#pragma endregion

class EffectManager
{
public:
	static	void	add(Effect* newEffect);
	static	void	addExplode(D3DXVECTOR3 position, DWORD delayTime = 0);
	static	void	addBulletExplode(D3DXVECTOR3 position, DWORD delayTime = 0);
	static	void	addSmallExplode(D3DXVECTOR3 position, DWORD delayTime = 0);
	static	void	addWaterFlow(D3DXVECTOR3 position, DWORD delayTime = 0);

	static	void	draw();
	static	double	getExplodeTime();

private:
			EffectManager();
			~EffectManager();
	static	EffectManager& getInstance();

private:
	static	EffectManager*	_instance;
			vector<Effect*> _livingEffects;
};

#endif // EffectManager_h__
