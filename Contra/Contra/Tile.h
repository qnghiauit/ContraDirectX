#ifndef __TILE_H__
#define __TILE_H__

#include "MoveObject.h"
#include "Pretties.h"

#include "DX9Framework\DefaultStructure.h"
#include "DX9Framework\DefaultMethods.h"
#include "DX9Framework\MergedTexture.h"


enum TileType {
	TOP_ONLY = 0,
	TOP_UNDROP,
	FOUR_FACE,
	FOUR_DESTROYABLE,
	FOUR_IMMORTAL,
	NUM_OF_TILE_TYPE
};

class Tile : virtual public Object
{
public:
	Tile();
	Tile(long left, long right, long top, long bottom,
		int characterTouch, TileType type);
	virtual ~Tile();
	virtual void		update();
	virtual void		draw();
			void		pushAPretties(Pretties* newPretties);
			TileType	getType();
			int			getTouchedStatus();
	virtual RECT		getRect();
	virtual void		activate();
			bool		isWater();

			virtual void setBasicProperties();
			
			virtual bool  affect(Object* obj, DWORD key);
			virtual bool  affect(Object* obj, DWORD key, TileType collisLevel);

			virtual bool isTerminated();

protected:
	vector<const Pretties*> _linkedPretties;

private:
	TileType _type;
	int _characterTouch;
};

#endif