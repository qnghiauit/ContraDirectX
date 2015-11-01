#include "BulletManager.h"

BulletManager* BulletManager::_instance = 0;


BulletManager::BulletManager()
{
}


void BulletManager::initialize()
{
	getInstance()._pool.insert(BulletPair(BulletType::MOBS,				new Bullet*[BULLET_MAX_MOBS]));
	getInstance()._pool.insert(BulletPair(BulletType::BASIC,			new Bullet*[BULLET_MAX_BASIC]));
	getInstance()._pool.insert(BulletPair(BulletType::F,				new Bullet*[BULLET_MAX_F]));
	getInstance()._pool.insert(BulletPair(BulletType::S,				new Bullet*[BULLET_MAX_S]));
	getInstance()._pool.insert(BulletPair(BulletType::M,				new Bullet*[BULLET_MAX_M]));
	getInstance()._pool.insert(BulletPair(BulletType::L,				new Bullet*[BULLET_MAX_L]));
	getInstance()._pool.insert(BulletPair(BulletType::RED_BOOM,			new Bullet*[BULLET_MAX_RED_BOOM]));
	getInstance()._pool.insert(BulletPair(BulletType::RED_SPREAD_BOOM,	new Bullet*[BULLET_MAX_RED_SPREAD_BOOM]));
	getInstance()._pool.insert(BulletPair(BulletType::FIRE_CIRCLE,		new Bullet*[BULLET_MAX_FIRE_CIRCLE]));
	getInstance()._pool.insert(BulletPair(BulletType::ENEMY_MACHINE,	new Bullet*[BULLET_MAX_MOBS]));

	getCounter(BulletType::MOBS)			= BULLET_MAX_MOBS				- 1;
	getCounter(BulletType::BASIC)			= BULLET_MAX_BASIC				- 1;
	getCounter(BulletType::F)				= BULLET_MAX_F					- 1;
	getCounter(BulletType::S)				= BULLET_MAX_S					- 1;
	getCounter(BulletType::M)				= BULLET_MAX_M					- 1;
	getCounter(BulletType::L)				= BULLET_MAX_L					- 1;
	getCounter(BulletType::RED_BOOM)		= BULLET_MAX_RED_BOOM			- 1;
	getCounter(BulletType::RED_SPREAD_BOOM)	= BULLET_MAX_RED_SPREAD_BOOM	- 1;
	getCounter(BulletType::FIRE_CIRCLE)		= BULLET_MAX_FIRE_CIRCLE		- 1;
	getCounter(BulletType::ENEMY_MACHINE)	= BULLET_MAX_MOBS				- 1;

	FOR(BULLET_MAX_MOBS)	{getPool(BulletType::MOBS)[i]		= new BulletBasic(ALLY_MOBS);	getPool(BulletType::MOBS )[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_BASIC)	{getPool(BulletType::BASIC)[i]		= new BulletBasic(ALLY_PLAYER);	getPool(BulletType::BASIC)[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_F)		{getPool(BulletType::F)[i]			= new BulletFire();				getPool(BulletType::F	 )[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_S)		{getPool(BulletType::S)[i]			= new BulletSpread();			getPool(BulletType::S	 )[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_M)		{getPool(BulletType::M)[i]			= new BulletMachine();			getPool(BulletType::M	 )[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_L)		{getPool(BulletType::L)[i]			= new BulletLaser();			getPool(BulletType::L	 )[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_RED_BOOM)		{getPool(BulletType::RED_BOOM)[i]			= new RedBoom();				getPool(BulletType::RED_BOOM		)[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_FIRE_CIRCLE)		{getPool(BulletType::FIRE_CIRCLE)[i]		= new FireCircle();				getPool(BulletType::FIRE_CIRCLE		)[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_RED_SPREAD_BOOM)	{getPool(BulletType::RED_SPREAD_BOOM)[i]	= new RedSpreadBoom();			getPool(BulletType::RED_SPREAD_BOOM	)[i]->assignManager(getInstance());}
	FOR(BULLET_MAX_MOBS)			{getPool(BulletType::ENEMY_MACHINE)[i]		= new BulletMachine(ALLY_MOBS);	getPool(BulletType::MOBS			)[i]->assignManager(getInstance());}
}


BulletManager::~BulletManager()
{
	FOR_e(BulletType::NUM_OF_BULLET_TYPE) {
		FORJ(_counter[i])
			SAFE_RELEASE(_pool.at((BulletType)i)[j]);
		SAFE_RELEASE(_pool.at((BulletType)i));
	}

	SAFE_RELEASE(_instance);
}


BulletManager& BulletManager::getInstance()
{
	if (BulletManager::_instance == 0) {
		BulletManager::_instance = new BulletManager();
		BulletManager::initialize();
	}
	return *BulletManager::_instance;
}


void BulletManager::push(Bullet* newBullet)
{
	BulletManager::getInstance()._livingBullet.push_back(newBullet);
}


void BulletManager::putInPool(Bullet* popedBullet)
{
	BulletType index = popedBullet->getType();
	BulletManager::getCounter(index)++;
	BulletManager::getPool(index)[BulletManager::getCounter(index)] = popedBullet;
	popedBullet = 0;
}


Bullet* BulletManager::pop(index_t index)
{
	Bullet* popedBullet;
	auto* livingBullet = &BulletManager::getInstance()._livingBullet;

	popedBullet = livingBullet->at(index);
	livingBullet->erase(livingBullet->begin() + index);

	putInPool(popedBullet);

	popedBullet->fixPosition(-popedBullet->getPosition());

	return popedBullet;
}


void BulletManager::update()
{
	auto* livingBullet = &BulletManager::getInstance()._livingBullet;
	auto* pool = &BulletManager::getInstance()._pool;

	FOR(livingBullet->size()) {

		livingBullet->at(i)->update();

		if (livingBullet->at(i)->isTerminated()) {
			pop(i);
		}
	}
}


void BulletManager::draw()
{
	auto* livingBullet = &BulletManager::getInstance()._livingBullet;

	FOR(livingBullet->size()) {
		livingBullet->at(i)->draw();
	}
}


Bullet* BulletManager::shoot(BulletType type, BYTE ally, D3DXVECTOR3 startPoint, int angle, float scaleX)
{
	auto* livingBullet = &BulletManager::getInstance()._livingBullet;
	auto* pool = &BulletManager::getInstance()._pool;
	auto* counter = BulletManager::getInstance()._counter;

	if (counter[type] < 0 && type != BulletType::L)
		return 0;
	Bullet* shootedOne;

	if (type != BulletType::L)
		shootedOne = pool->at(type)[counter[type]];
	else
	{
		remove(type);
		shootedOne = pool->at(type)[BULLET_MAX_L - 1];
	}

	shootedOne->shoot(angle, startPoint, ally, scaleX);
	return shootedOne;
}


Bullet* BulletManager::shoot(BulletType type, BYTE ally, D3DXVECTOR3 startPoint, D3DXVECTOR3 velo, float scaleX)
{
	auto* livingBullet = &BulletManager::getInstance()._livingBullet;
	auto* pool = &BulletManager::getInstance()._pool;
	auto* counter = BulletManager::getInstance()._counter;

	if (counter[type] < 0 && type != BulletType::L)
		return 0;
	Bullet* shootedOne;

	if (type != BulletType::L)
		shootedOne = pool->at(type)[counter[type]];
	else
	{
		remove(type);
		shootedOne = pool->at(type)[BULLET_MAX_L - 1];
	}

	shootedOne->shoot(velo, startPoint, ally, scaleX);
	return shootedOne;
}


void BulletManager::updateIfObjectIsShooted(Object* object, DWORD key)
{
	auto* livingBullet = &BulletManager::getInstance()._livingBullet;

	if (!object->isShootable())
		return;

	FOR(livingBullet->size())
	{
		if (livingBullet->at(i)->affect(object, key))
			pop(i);
	}
}


Bullet** BulletManager::getPool(BulletType type)
{
	return getInstance()._pool.at(type);
}


vector<Bullet*>& BulletManager::getLivingBulletList()
{
	return getInstance()._livingBullet;
}


int& BulletManager::getCounter(BulletType type)
{
	return getInstance()._counter[type];
}


void BulletManager::remove(BulletType type)
{
	FOR(getLivingBulletList().size())
		if (type == getLivingBulletList()[i]->getType() || type == BULLET_TYPE_ANY)
		{
			pop(i);
			i--;
		}
}