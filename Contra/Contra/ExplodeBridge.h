#ifndef __EXPLODE_BRIDGE_H__
#define __EXPLODE_BIRDGE_H__

#pragma warning(disable : 4250)

#include "Tile.h"
#include "AnimatedPretties.h"
#include "EffectManager.h"

enum ExplodeBrideStatus
{
	BRIDGE_WAIT = 0,
	BRIDGE_EXPLODE,
	BRIDGE_DETONATE,
	BRIDGE_DISSAPEAR,
	NUM_OF_BRIDGE_STATUS
};


enum ExplodeBridgeOrder
{
	BRIDGEHEAD_LEFT = 0,
	BRIDGE_MIDDLE,
	BRIDGEHEAD_RIGHT,
	NUM_OF_ORDER
};


#define LINK_BRIDGE				_T("Resource//Miscellaneous//Bridge//Bridge")
#define LINK_BRIDGE_DESTROYED	_T("Resource//Miscellaneous//Bridge//Destroyed")
#define LINK_BRIDGE_PIERS		_T("Resource//Miscellaneous//Bridge//Piers")


// Must be equal with tile height
#define EXPLODE_BRIDGE_THICKNESS		10
#define EXPLODE_BRIDGE_LONG				SIZE_TILE_W
#define EXPLODE_BRIDGE_RESOURCE_LINK	(tstring(LINK_BRIDGE) + EXTENSION_PICTURE)
#define EXPLODE_BRIDGE_RESOURCE_ROWS	(size_t)2
#define EXPLODE_BRIDGE_FRAME_NUM		3

#define EXPLODE_BRIDGE_RANDOM_IN_WIDTH	(rand() % EXPLODE_BRIDGE_LONG)
#define EXPLODE_BRIDGE_RANDOM_IN_HEIGHT	(rand() % SIZE_TILE_H)
#define EXPLODE_NUM_FOR_DESTROY_ANIMATION	4

class ExplodeBridge : virtual public AnimatedPretties, virtual public Tile
{
public:
	virtual ~ExplodeBridge();

	static ExplodeBridge* createBridge(Object* baseObject, Tile* leftside_BrideHead, Tile* rightside_BrideHead,
										vector<Tile*>* tileList,
										ExplodeBridgeOrder order = ExplodeBridgeOrder::BRIDGEHEAD_LEFT);
	virtual void		activate();
			void		update();
			void		draw();
	static	void		releaseExplodeBridgeResource();

	virtual bool isTerminated();

private:
			Object*				_baseObjectForUpdate;
			ExplodeBrideStatus	_currentBridgeStatus;
			ExplodeBridgeOrder	_order;

			Tile**				_bridgehead_LinkedTile;
			DWORD				_destroyTimeRemaining;
	static	Pretties**			_destroyedBridgePretties;

private:
	ExplodeBridge(Object* baseObject, D3DXVECTOR3 position, RECT boundRange, ExplodeBridgeOrder order);
	void	detonate();

	static void initializeBridgeStaticResource();

	virtual bool  affect(Object* obj, DWORD key);


};

#endif