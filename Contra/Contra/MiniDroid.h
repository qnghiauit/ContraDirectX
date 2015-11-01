#ifndef MiniDroid_h__
#define MiniDroid_h__

#include "Mobs.h"
#include "MoveObject.h"

#define MINI_DROID_RESOURCE_LINK	(tstring(_T("Resource//Bosses//PhantomDroid//MiniDroid")) + EXTENSION_PICTURE)
#define MINI_DROID_RESOURCE_ROWS	1
#define MINI_DROID_RESOURCE_COLUMNS	3
#define MINI_DROID_MOVE_SPEED_X(scaleX)	(D3DXVECTOR3(60.0F * scaleX, 0.0F, 0.0F))
#define MINI_DROID_MOVE_SPEED_Y			(D3DXVECTOR3(0.0F, -60.0F, 0.0F))
#define MINI_DROID_WAIT_TIME		1000
#define MINI_DROID_BOUND_SIZE		Size(26.0F, 18.0F)
#define SCORE_MINI_DROID			50

enum MiniDroidState
{
	MINI_DROID_MOVE_X = 0,
	MINI_DROID_MOVE_Y,
	MINI_DROID_GO_OUT
};

class MiniDroid :
	public Mobs,
	public MoveObject
{
public:
	MiniDroid(D3DXVECTOR3 pos, D3DXVECTOR2 scale);
	virtual ~MiniDroid();

	virtual unsigned int getScore();

	virtual void draw();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual void activate();

	virtual bool affect(Object* obj, DWORD key);

	virtual void setBasicProperties();

	virtual void deadEffect();

	virtual void update();

	virtual bool isTerminated();

private:
	DWORD timeCounter_;
};

#endif // MiniDroid_h__
