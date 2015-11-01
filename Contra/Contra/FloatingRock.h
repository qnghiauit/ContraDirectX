#ifndef __FLOATING_ROCK__
#define __FLOATING_ROCK__

#include "Tile.h"
#include "MoveObject.h"

#define FLOATING_ROCK_RESOURCE_LINK	(tstring(_T("Resource//Miscellaneous//FloatingRock")) + EXTENSION_PICTURE)
#define FLOATING_ROCK_RESOURCE_ROWS	1
#define FLOATING_ROCK_RESOURCE_COLUMNS	1
#define FLOATING_ROCK_BOUND_SIZE	Size(32.0, 16.0)

class FloatingRock :
	public Tile,
	public MoveObject
{
public:
	FloatingRock(D3DXVECTOR3 position, D3DXVECTOR3 velocity, RECT moveRange);
	virtual ~FloatingRock();

	virtual void update();

	virtual void draw();

	virtual void setBasicProperties();

	virtual bool affect(Object* obj, DWORD key);

	virtual bool affect(Object* obj, DWORD key, TileType collisLevel);

	virtual bool isTerminated();

	virtual D3DXVECTOR3* getVelo();

private:
	RECT _moveRange;
	DWORD _frameTime;
};

#endif // FloatingRock_h__
