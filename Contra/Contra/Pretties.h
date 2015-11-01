#ifndef __PRETTIES_H__
#define __PRETTIES_H__

#include "Object.h"

#include "DX9Framework\MergedTexture.h"

class Pretties : virtual public Object
{
public:
	Pretties(){}
	Pretties(tstring path, index_t index, size_t rows, size_t columns, D3DXVECTOR3 position,
		int resourceType = RESOURCE_MT, DrawCenter	drawCenter = DrawCenter::BOTTOMLEFT);
	Pretties(const Pretties& p);
	virtual ~Pretties();

			Pretties* clone() const;
			Pretties* clone(D3DXVECTOR3 newPos) const;
	virtual void draw();
	virtual void draw() const;
	virtual void update();

	virtual bool  affect(Object* obj, DWORD key);

	virtual bool isTerminated();

protected:
	index_t		_index;
	DrawCenter	_drawCenter;
};

#endif