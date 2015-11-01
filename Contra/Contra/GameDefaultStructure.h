#ifndef __GAME_DEFAULT_STRUCTURE_H__
#define __GAME_DEFAULT_STRUCTURE_H__

enum BulletType {
	BULLET_TYPE_ANY = -1,
	L = 0,
	F,
	M,
	S,
	BASIC,
	MOBS,
	RED_BOOM,
	RED_SPREAD_BOOM,
	FIRE_CIRCLE,
	ENEMY_MACHINE,
	NUM_OF_BULLET_TYPE
};

#define RELOAD_TIME	DWORD reload[] = {60, 100, 100, 160, 160, 320, 0, 0, 6000, 0}

enum BulletRS {
	LAZER = 0,
	FIRE,
	MACHINE,
	AMMO,
	ENERGY_FIRE,
	NUM_OF_BULLET_RESOURCE
};

struct Range
{
	int min;
	int max;
};
#endif