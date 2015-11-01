#ifndef __AUTO_AIMING_OBJECT_H__
#define __AUTO_AIMING_OBJECT_H__

#include "Object.h"
#include "GraphicAlgorimths2D/CircleEquation.h"

class AutoAimingObject : virtual public Object
{
public:
	AutoAimingObject();
	virtual ~AutoAimingObject();
	int aim();
	void setAimingProperties(Object* target, double measure, D3DXVECTOR3 basePoint = POSITION_ZERO);
	virtual bool isTerminated() = 0;

protected:
	virtual void setBasicProperties();

protected:
	Object* _target;
	int _currentAimingAngle;
	double _aimingMeasure;
	D3DXVECTOR3 _basePoint;
};

#endif // AutoAimingObject_h__
