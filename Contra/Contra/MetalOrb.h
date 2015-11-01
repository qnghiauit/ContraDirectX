#ifndef __METAL_ORB_H__
#define __METAL_ORB_H__

#include "Mobs.h"

#define METAL_ORB_BOUND_SIZE	Size(16.0F, 16.0F)
#define METAL_ORB_RESOURCE_LINK	(tstring(_T("Resource//Bosses//ShadowEntrance//MetalOrb")) + EXTENSION_PICTURE)
#define METAL_ORB_RESOURCE_ROWS	1
#define METAL_ORB_RESOURCE_COLUMNS	1

class MetalOrb :
	public Mobs
{
public:
	MetalOrb(){}
	MetalOrb(D3DXVECTOR3 position);
	virtual ~MetalOrb();

	virtual unsigned int getScore();

	virtual void waitUpdate();

	virtual void activateUpdate();

	virtual void deadUpdate();

	virtual bool affect(Object* obj, DWORD key);

	virtual void setBasicProperties();

	virtual void deadEffect();

			void setPos(D3DXVECTOR3 newPos);

			virtual void draw();

};

#endif // MetalOrb_h__
