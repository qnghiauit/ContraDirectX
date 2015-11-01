#ifndef __BULLET_MANAGER_H__
#define __BULLET_MANAGER_H__

#include <vector>
#include <map>

#include "DX9Framework\DefaultStructure.h"

#include "Bullet.h"
#include "BulletBasic.h"
#include "BulletFire.h"
#include "BulletSpread.h"
#include "BulletMachine.h"
#include "BulletLaser.h"
#include "RedBoom.h"
#include "RedSpreadBoom.h"
#include "FireCircle.h"

#include "Collision.h"
#include "EffectManager.h"

typedef map<BulletType, Bullet**> BulletMap;
typedef pair<BulletType, Bullet**> BulletPair;

class BulletManager
{
public:
	static void				update();
	static void				draw();
	static void				updateIfObjectIsShooted(Object* object, DWORD key);
	static BulletManager&	getInstance();
	static Bullet*			shoot(BulletType type, BYTE ally, D3DXVECTOR3 startPoint, int angle, float scaleX);
	static Bullet*			shoot(BulletType type, BYTE ally, D3DXVECTOR3 startPoint, D3DXVECTOR3 velo, float scaleX);
	static Bullet**			getPool(BulletType type);
	static vector<Bullet*>&	getLivingBulletList();
	static int&				getCounter(BulletType type);
	static void				remove(BulletType type);

private:
	vector<Bullet*>			_livingBullet;
	BulletMap				_pool;
	int						_counter[BulletType::NUM_OF_BULLET_TYPE];

private:
	BulletManager();
	~BulletManager();

	static void		initialize();
	static void		push(Bullet* newBullet);
	static Bullet*	pop(index_t index);
	static void		putInPool(Bullet* popedBullet);

	static BulletManager* _instance;
};

#endif