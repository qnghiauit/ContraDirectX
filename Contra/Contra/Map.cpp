#include "Map.h"
#include "GameState.h"

Object* Map::_player = nullptr;
Map* Map::_instance = nullptr;


Map::Map()
{
	this->_stage = 1;

	this->_boss = nullptr;
	this->_mapObjects = nullptr;
	this->_spawPos = POSITION_ZERO;
}


Map::Map(int stage)
{
	_mapSize = 0;

	this->_stage = stage;

	loadInfo();
	loadPretties();
	loadAnimatedPretties();
	loadTile();
	loadMobs();
	loadSignatureObjects();

	// Concatenate all map object's reference to split into 4tree.
	vector<Object*> mapObjects;
	mapObjects.insert(mapObjects.end(), _pretties.begin(), _pretties.end());

	_mapObjects = new QuadTree(new RECT(Default::createRECT(0, _mapSize, _mapSize, 0)));
	_mapObjects->assign(mapObjects);
}


Map::~Map()
{
	SAFE_RELEASE(_mapObjects);

	FOR(_pretties.size())		SAFE_RELEASE(_pretties.at(i));		_pretties.clear();
	FOR(_gameObjects.size())	SAFE_RELEASE(_gameObjects.at(i));	_gameObjects.clear();
}


void Map::loadPretties()
{
	// Get resource info.
	tstring linkRS(LINK_PRETTIES_RS + Default::tostr(_stage));
	fstream data(linkRS + INFO_FILE);

	int columns, rows, prettiesWidth, prettiesHeight;
	if (data.is_open()) {
		data >> columns >> rows >> prettiesWidth >> prettiesHeight;
	}
	else {
		WARNING_BOX(tstring(WARNING_CANNOT_OPEN_INFO_FILE + linkRS).c_str());
		return;
	}

	data.close();

	// Read pretties map.
	tstring linkIF = tstring(LINK_PRETTIES_IF + Default::tostr(_stage));
	data.open(linkIF.c_str());

	if (data.is_open()) {
		int sizeMapWidth, nPretties;
		data >> nPretties;
		data >> sizeMapWidth;

		int value;
		FOR(nPretties) {
			data >> value;
			_pretties.push_back(new Pretties((linkRS + EXTENSION_PICTURE).c_str(), value, rows, columns,
				POSITION(i, sizeMapWidth, prettiesWidth, prettiesHeight)));
		}
	}
	else {
		WARNING_BOX(tstring(WARNING_CANNOT_OPEN_INFO_FILE + linkIF).c_str());
		return;
	}
	data.close();
}


void Map::loadTile()
{
	fstream data;
	tstring linkIF(LINK_TILE_IF + Default::tostr(_stage));
	data.open(linkIF);

	index_t tileMark = ~0;

	if (data.is_open()) {

		while (!data.eof())
		{
			long l, r, t, b;
			int characterTouch, tileType;
			data >> l >> r >> t >> b >> characterTouch >> tileType;
			_tiles.push_back(new Tile(l, r, t, b, characterTouch, (TileType)tileType));
		}

		data.close();
	}
	else {
		WARNING_BOX(tstring(WARNING_CANNOT_OPEN_INFO_FILE + linkIF).c_str());
		return;
	}
}


void Map::loadAnimatedPretties()
{
	tstring linkRS(LINK_A_PRETTIES_RS + Default::tostr(_stage));
	fstream data(linkRS + INFO_FILE);

	int rows, columns, tileSizeW, tileSizeH;
	if (data.is_open()) {

		data >> columns >> rows >> tileSizeW >> tileSizeH;
		data.close();

	}
	else {
		WARNING_BOX(tstring(WARNING_CANNOT_OPEN_INFO_FILE + linkRS).c_str());
		return;
	}

	tstring linkIF(LINK_A_PRETTIES_IF + Default::tostr(_stage));
	data.open(linkIF);

	index_t tileMark = ~0;
	index_t index;

	if (data.is_open()) {

		int nTileWidth, nTiles;
		data >> nTiles >> nTileWidth;

		int i = 0;
		while (!data.eof()) {

			data >> index;
			if (index == MARK_OFSET) {
				data >> tileMark;
			}
			else {
				_pretties.push_back(new AnimatedPretties(linkRS + EXTENSION_PICTURE, tileMark,
					rows, POSITION(index, nTileWidth, tileSizeW, tileSizeH), columns));
				i++;
			}

		}
		data.close();
	}
	else {
		WARNING_BOX(tstring(WARNING_CANNOT_OPEN_INFO_FILE + linkRS).c_str());
		return;
	}
}


void Map::loadMobs()
{
	tstring linkIF(LINK_MOBS_IF + Default::tostr(_stage));
	fstream data(linkIF);

	if (data.is_open())
	{
		int mobType;
		while (!data.eof())
		{
			data >> mobType;
			D3DXVECTOR3 pos(0.0F, 0.0F, DEFAULT_DEPTH);
			Size visible;
			int intPropertie;


			switch ((MobTypes)mobType)
			{
			case MobTypes::RUNNING_MAN:
				data >> pos.x >> pos.y >> intPropertie;
				_gameObjects.push_back(
					new RunningMan(pos, 
									D3DXVECTOR3(RUNNING_MAN_MOVE_SPEED_ABSOLUTE_VALUE * GET_SIGN(_player->getPosition().x - pos.x)
									, 0.0f, 0.0f), intPropertie));
				break;

			case MobTypes::SNIPER:
				data >> pos.x >> pos.y;
				_gameObjects.push_back(new Sniper(pos, _player));
				break;

			case MobTypes::HIDDEN_SNIPER:
				data >> pos.x >> pos.y >> visible.width >> visible.height >> intPropertie;
				_gameObjects.push_back(new HiddenSniper(pos, _player, visible, intPropertie));
				break;

			case MobTypes::STAND_CANON:
				data >> pos.x >> pos.y;
				_gameObjects.push_back(new StandCanon(pos, _player));
				break;

			case MobTypes::CANON:
				data >> pos.x >> pos.y;
				_gameObjects.push_back(new Canon(pos, _player));
				break;

			case MobTypes::DIVE_SHOOTER:
				data >> pos.x >> pos.y;
				_gameObjects.push_back(new DiveShooter(pos, _player));
				break;

			case MobTypes::STAND_SHOOTER:
				data >> pos.x >> pos.y;
				_gameObjects.push_back(new StandShooter(pos));
				break;

			case MobTypes::TANK:
				data >> pos.x >> pos.y;
				_gameObjects.push_back(new Tank(pos, _player));
				break;

			default:
				break;
			}
		}
	}
	else {
		WARNING_BOX(tstring(WARNING_CANNOT_OPEN_INFO_FILE + linkIF).c_str());
		return;
	}
}


void Map::update()
{
#if DEBUG == DEBUG_MAP_UPDATE_TIME
	clock_t time = clock();
#endif

	FOR(Map::getInstance()._gameObjects.size())
	{
		Map::getInstance()._gameObjects.at(i)->update();
		if (Map::getInstance()._gameObjects.at(i)->isTerminated())
		{
			Map::getInstance()._gameObjects.erase(Map::getInstance()._gameObjects.begin() + i);
			i--;
		}
	}

	FOR(Map::getInstance()._tiles.size())
		Map::getInstance()._tiles.at(i)->update();

	getInstance()._boss->update();

	if (getInstance()._boss->isDead())
	{
		FreeSound::stop(getSoundtrackLink());
		if (getInstance()._boss->getTimeCounter() > TIME_TO_NEXT_STAGE)
		{
			toNextMap();
		}
	}

#if DEBUG == DEBUG_MAP_UPDATE_TIME
	time = clock() - time;
	OutputDebugString(Default::tostr(time).c_str());
	OutputDebugString(L"\n");
#endif
}


void Map::draw()
{
#if DEBUG == DEBUG_MAP_DRAW_TIME
	clock_t time = clock();
#endif

	Map::getInstance()._mapObjects->draw(Camera::getViewPort());
	FOR(Map::getInstance()._gameObjects.size())
		Map::getInstance()._gameObjects.at(i)->draw();

	FOR(Map::getInstance()._tiles.size())
		Map::getInstance()._tiles.at(i)->draw();

	getInstance()._boss->draw();

#if DEBUG == DEBUG_MAP_DRAW_TIME
	time = clock() - time;
	OutputDebugString(Default::tostr(time).c_str());
	OutputDebugString(L"\n");
#endif
}


Map& Map::getInstance()
{
	if (Map::_instance == nullptr) {
		Map::_instance = new Map();
		return *Map::_instance;
	}

	return *Map::_instance;
}


void Map::toNextMap()
{
	int stage;
	if (Map::_instance == nullptr)
		stage = 1;
	else
		stage = Map::_instance->_stage + 1;

	SAFE_RELEASE(Map::_instance);
	Map::_instance = new Map(stage);
	GameState::switchState(StartingState::get());
}


void Map::loadSignatureObjects()
{
	fstream data;
	tstring linkIF(LINK_SIGNATURE_IF + Default::tostr(_stage));
	data.open(linkIF);

	string objectMark;

	if (data.is_open()) {

		D3DXVECTOR3 pos = GAMEPOS_TOP_LEFT;
		data >> objectMark >> pos.x >> pos.y;

		SAFE_RELEASE(_boss);
		if (objectMark == BOSS_WALL)
			_boss = new CrossHair(pos, _player);
		else if (objectMark == BOSS_ALIEN)
			_boss = new Mouth(pos, _player);
		else if (objectMark == BOSS_DROID)
			_boss = new Droid(pos);

		while (!data.eof())
		{
			data >> objectMark;

			if (objectMark == SIGNATURE_EXPLODE_BRIDGE)
				loadExplodeBridge(data);
			else if (objectMark == SIGNATURE_BULLET_BAG)
				loadBulletBag(data);
			else if (objectMark == SIGNATURE_FLOATING_BAG)
				loadFloatingBag(data);
			else if (objectMark == SIGNATURE_FALLING_STONE)
				loadFallingStone(data);
			else if (objectMark == SIGNATURE_FIRE)
				loadFire(data);
			else if (objectMark == SIGNATURE_FLOATING_ROCK)
				loadFloatingRock(data);
			else if (objectMark == SIGNATURE_BLUE_BOOM)
				loadBlueBoom(data);
		}
		data.close();
	}
	else {
		WARNING_BOX(tstring(WARNING_CANNOT_OPEN_INFO_FILE + linkIF).c_str());
		return;
	}
}


void Map::loadExplodeBridge(fstream& data)
{
	int left, right;
	data >> left >> right;

	ExplodeBridge::createBridge(Map::_player, _tiles[left], _tiles[right], &_tiles);
}


void Map::assignPlayer(Object* player)
{
	_player = player;
}


void Map::generateFalcon(D3DXVECTOR3 position, BulletFalconType type)
{
	getInstance()._gameObjects.push_back(new BulletFalcon(position, type));
}


void Map::affectedByGameObjects(Object* obj, DWORD key)
{
	FOR(getInstance()._gameObjects.size())
		if (getInstance()._gameObjects.at(i)->affect(obj, key))
		{
			SAFE_RELEASE(getInstance()._gameObjects.at(i));
			getInstance()._gameObjects.erase(getInstance()._gameObjects.begin() + i);
		}

	getInstance()._boss->affect(obj, key);
}


void Map::affectedByTiles(Object* obj, bool isWaterCollis, int collisLevel)
{
	DWORD key = KEY_MOVEABLE;
	if (isWaterCollis)
		key = KEY_MOVEABLE | KEY_WATER_COLLIS;

	FOR(getInstance()._tiles.size())
		getInstance()._tiles.at(i)->affect(obj, key, (TileType)collisLevel);
}


void Map::loadBulletBag(fstream& data)
{
	D3DXVECTOR3 position = GAMEPOS_TOP_LEFT;
	int falconType;

	data >> position.x >> position.y;
	data >> falconType;

	_gameObjects.push_back(new BulletBag(position, (BulletFalconType)falconType));
}


void Map::loadFloatingBag(fstream& data)
{
	D3DXVECTOR3 position = GAMEPOS_TOP_LEFT;
	int falconType;
	D3DXVECTOR3 velocity;

	data >> position.x >> position.y;
	data >> falconType;
	data >> velocity.x >> velocity.y;

	_gameObjects.push_back(new FloatingBulletBag(position, (BulletFalconType)falconType, velocity));
}


void Map::loadInfo()
{
	ifstream data(tstring(LINK_MAP_IF) + Default::tostr(_stage));
	if (data.is_open())
	{
		string buffer;
		getline(data, buffer);
		_mapName = tstring(buffer.begin(), buffer.end());

		data >> _cameraType;

		data >> buffer;
		_soundtrackLink = tstring(buffer.begin(), buffer.end());
		data >> _mapSize;

		data >> _spawPos.x >> _spawPos.y;
	}
}


int Map::getCameraType()
{
	return getInstance()._cameraType;
}


void Map::loadFallingStone(fstream& data)
{
	D3DXVECTOR3 position = GAMEPOS_TOP_LEFT;

	data >> position.x >> position.y;
	_gameObjects.push_back(new FallingStone(position));
}


void Map::loadFire(fstream& data)
{
	D3DXVECTOR3 position = GAMEPOS_TOP_LEFT;
	D3DXVECTOR3 velocity = SPEED_NO;
	RECT	moveRange;
	memset(&moveRange, 0, sizeof(moveRange));

	data >> position.x >> position.y;
	data >> velocity.x >> velocity.y;
	data >> moveRange.left >> moveRange.right;

	_gameObjects.push_back(new Fire(position, velocity, moveRange));
}


void Map::loadBlueBoom(fstream& data)
{
	D3DXVECTOR3 position = GAMEPOS_TOP_LEFT;

	data >> position.x >> position.y;

	_gameObjects.push_back(new BlueBoomGenerator(position));
}

void Map::loadFloatingRock(fstream& data)
{
	D3DXVECTOR3 position = GAMEPOS_TOP_LEFT;
	D3DXVECTOR3 velocity = SPEED_NO;
	RECT	moveRange;
	memset(&moveRange, 0, sizeof(moveRange));

	data >> position.x >> position.y;
	data >> velocity.x >> velocity.y;
	data >> moveRange.left >> moveRange.right;

	FloatingRock* theRock = new FloatingRock(position, velocity, moveRange);

	_gameObjects.push_back(theRock);
	_tiles.push_back(theRock);
}

int Map::getStageIndex()
{
	return getInstance()._stage;
}

tstring Map::getStageName()
{
	return getInstance()._mapName;
}

tstring Map::getSoundtrackLink()
{
	return getInstance()._soundtrackLink;
}

int Map::getMapSize_()
{
	return getInstance()._mapSize;
}

void Map::addGameObject(Object* object)
{
	getInstance()._gameObjects.push_back(object);
}

void Map::reload()
{
	int stage = Map::getInstance().getStageIndex();
	SAFE_RELEASE(Map::_instance);
	Map::_instance = new Map(stage);
}

D3DXVECTOR3 Map::getPlayerSpawPos()
{
	return getInstance()._spawPos;
}

void Map::reset()
{
	SAFE_RELEASE(Map::_instance);
}

D3DXVECTOR3 Map::computeRespawPos()
{
	RECT view = Camera::getViewPort();

	RECT range = view;
	if (getCameraType() == CAMERA_Y)
		range.top = range.bottom + (range.top - range.bottom) * 2 / 3;

	while (true)
	{
		range.left += SIZE_TILE_W;

		if (range.left > view.right)
		{
			range.left = 0;
		}

		range.right = range.left + SIZE_TILE_W;

		FOR(getInstance()._tiles.size())
			if (Default::checkIfBoundedNotEqual(&getInstance()._tiles.at(i)->getRect(), &range))
			{
				return D3DXVECTOR3((range.right + range.left) / 2, range.top, 0.0f);
			}
	}
}
