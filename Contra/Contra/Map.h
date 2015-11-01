#ifndef __MAP_H__
#define __MAP_H__

#include <fstream>
#include <vector>

#include "GameDefaultConstant.h"
#include "Pretties.h"
#include "AnimatedPretties.h"
#include "Tile.h"
#include "ExplodeBridge.h"
#include "BulletFalcon.h"
#include "BulletBag.h"
#include "FloatingBulletBag.h"
#include "FloatingRock.h"

#include "Mobs.h"

#include "Quadtree.h"
#include "Collision.h"

#include "RunningMan.h"
#include "StandCanon.h"
#include "Canon.h"
#include "HiddenSniper.h"
#include "Sniper.h"
#include "CrossHair.h"
#include "FallingStone.h"
#include "Fire.h"
#include "DiveShooter.h"
#include "Tentacle.h"
#include "Mouth.h"
#include "StandShooter.h"
#include "Tank.h"
#include "Droid.h"
#include "BlueBoomGenerator.h"

#include "DX9Framework\MergedTexture.h"
#include "DX9Framework\DefaultMethods.h"
#include "DX9Framework\DefaultConstant.h"

#define SIGNATURE_EXPLODE_BRIDGE	"EXPLODE_BRIDGE"
#define SIGNATURE_BULLET_BAG		"BULLET_BAG"
#define SIGNATURE_FLOATING_BAG		"FLOATING_BAG"
#define SIGNATURE_FALLING_STONE		"FALLING_STONE"
#define SIGNATURE_FIRE				"FIRE"
#define SIGNATURE_FLOATING_ROCK		"FLOATING_ROCK"
#define SIGNATURE_BLUE_BOOM			"BLUE_BOOM"

#define BOSS_WALL	"WALL"
#define BOSS_ALIEN	"ALIEN"
#define BOSS_DROID	"DROID"

class Sniper;

typedef TileType TouchLevel;

class Map
{
public:
	static void		update();
	static void		draw();
	static Map&		getInstance();
	static void		toNextMap();
	static int		getCameraType();
	static void		assignPlayer(Object* player);
	static void		generateFalcon(D3DXVECTOR3 position, BulletFalconType type);
	static int		getStageIndex();
	static tstring	getStageName();
	static tstring	getSoundtrackLink();
	static int		getMapSize_();
	static void		addGameObject(Object* object);
	static void		reload();
	static void		reset();
	static D3DXVECTOR3 computeRespawPos();

	static void		affectedByGameObjects(Object* obj, DWORD key);
	static void		affectedByTiles(Object* obj, bool isWaterCollis, int collisLevel);
	static D3DXVECTOR3	getPlayerSpawPos();

private:
	Map();
	Map(int stage);
	~Map();
	void	loadInfo();
	void	loadPretties();
	void	loadAnimatedPretties();
	void	loadTile();
	void	loadMobs();
	void	loadSignatureObjects();

	// Signature Object loading functions
	void	loadExplodeBridge(fstream& data);
	void	loadBulletBag(fstream& data);
	void	loadFloatingBag(fstream& data);
	void	loadFallingStone(fstream& data);
	void	loadFire(fstream& data);
	void	loadFloatingRock(fstream& data);
	void	loadBlueBoom(fstream& data);

private:
	static	Map*				_instance;
	static	Object*				_player;
	vector<Pretties*>			_pretties;
	vector<Tile*>				_tiles;
	vector<Object*>				_gameObjects;
	Boss*						_boss;
	QuadTree*					_mapObjects;
	int							_mapSize;
	int							_stage;
	int							_cameraType;
	tstring						_mapName;
	tstring						_soundtrackLink;
	D3DXVECTOR3					_spawPos;
};


#include "Sniper.h"

#endif