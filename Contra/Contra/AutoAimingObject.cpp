#include "AutoAimingObject.h"


AutoAimingObject::AutoAimingObject()
{
}


AutoAimingObject::~AutoAimingObject()
{
}

int AutoAimingObject::aim()
{
	D3DXVECTOR3 base = _basePoint;
	if (base == POSITION_ZERO)
		base = _position;

	D3DXVECTOR3 aimPoint = Default::computeCenter(_target->getRect());

	_currentAimingAngle = (int)CircleEquation(base,
		aimPoint).getDegreeFromCoordinates(aimPoint);

	_currentAimingAngle = Default::fixAngle(_currentAimingAngle);

	_currentAimingAngle = ((int)Default::round(
		_currentAimingAngle,
		_aimingMeasure));

	return _currentAimingAngle;
}

void AutoAimingObject::setBasicProperties()
{
	Object::setBasicProperties();
	_target = nullptr;
	_aimingMeasure = 1.0;
	_currentAimingAngle = 0;
}

void AutoAimingObject::setAimingProperties(Object* target, double measure, D3DXVECTOR3 basePoint)
{
	_target = target;
	_aimingMeasure = measure;
	if (basePoint != POSITION_ZERO)
		this->_basePoint = basePoint;
	else
		this->_basePoint = _position;
}


