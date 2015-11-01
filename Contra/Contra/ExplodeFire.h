#ifndef ExplodeFire_h__
#define ExplodeFire_h__

#include "Mobs.h"
#include "MoveObject.h"

#define EXFIRE_RESOURCE_LINK	(tstring(_T("Resource//Bosses//PhantomDroid//ExplodeFire")) + EXTENSION_PICTURE)
#define EXFIRE_RESOURCE_ROWS	1
#define EXFIRE_RESOURCE_COLUMNS	1
#define EXFIRE_BOUND_SIZE		Size(8.0F, 8.0F)

class ExplodeFire :
	public Mobs,
	public MoveObject
{
public:
	ExplodeFire(D3DXVECTOR3 pos);
	virtual ~ExplodeFire();

	virtual unsigned int getScore();

	virtual void draw();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual bool affect(Object* obj, DWORD key);

	virtual void setBasicProperties();

	virtual void deadEffect();

	virtual bool isTerminated();

	virtual void update();

};

#endif // ExplodeFire_h__