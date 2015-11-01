#include "EffectManager.h"


EffectManager* EffectManager::_instance = nullptr;


EffectManager::EffectManager()
{
}


EffectManager::~EffectManager()
{
}


void EffectManager::add(Effect* newEffect)
{
	getInstance()._livingEffects.push_back(newEffect);
}


void EffectManager::draw()
{
	vector<Effect*>* effects = &getInstance()._livingEffects;

	FOR(effects->size())
	{
		if (effects->at(i)->draw())
		{
			SAFE_RELEASE(effects->at(i));
			effects->erase(effects->begin() + i);
			i--;
		}
	}
}


EffectManager& EffectManager::getInstance()
{
	if (_instance == nullptr)
		_instance = new EffectManager();

	return *_instance;
}


double EffectManager::getExplodeTime()
{
	return EFFECT_EXPLODE_FRAME_TIME;
}




void EffectManager::addExplode		(D3DXVECTOR3 position, DWORD delayTime /*= 0*/)
{
	Effect* newEffect = new Effect(EFFECT_EXPLODE_LINK.c_str(), EFFECT_EXPLODE_COLUMNS, EFFECT_EXPLODE_ROWS, position, delayTime);
	newEffect->setFrameTime(EFFECT_EXPLODE_FRAME_TIME);
	EffectManager::add(newEffect);
}
void EffectManager::addBulletExplode(D3DXVECTOR3 position, DWORD delayTime /*= 0*/)
{
	Effect* newEffect = new Effect(EFFECT_EXPLODE_BULLET_LINK.c_str(), EFFECT_EXPLODE_BULLET_COLUMNS, EFFECT_EXPLODE_BULLET_ROWS, position, delayTime);
	newEffect->setFrameTime(EFFECT_EXPLODE_BULLET_FRAME_TIME);
	EffectManager::add(newEffect);
}
void EffectManager::addSmallExplode	(D3DXVECTOR3 position, DWORD delayTime /*= 0*/)
{
	Effect* newEffect = new Effect(EFFECT_EXPLODE_SMALL_LINK.c_str(), EFFECT_EXPLODE_SMALL_COLUMNS, EFFECT_EXPLODE_SMALL_ROWS, position, delayTime);
	newEffect->setFrameTime(EFFECT_EXPLODE_SMALL_FRAME_TIME);
	EffectManager::add(newEffect);
}
void EffectManager::addWaterFlow	(D3DXVECTOR3 position, DWORD delayTime /*= 0*/)
{
	Effect* newEffect = new Effect(EFFECT_WATER_FLOW_LINK.c_str(), EFFECT_WATER_FLOW_COLUMNS, EFFECT_WATER_FLOW_ROWS, position, delayTime);
	newEffect->setFrameTime(EFFECT_WATER_FLOW_FRAME_TIME);
	EffectManager::add(newEffect);
}