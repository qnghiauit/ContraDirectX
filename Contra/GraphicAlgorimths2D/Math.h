#ifndef __MATH_H__
#define __MATH_H__

#include <d3dx9.h>
#include "CircleEquation.h"

class Math
{
public:

	// -----------------------------------
	// Desc: Caculating the distance between 2 point.
	// -----------------------------------
	static double caculate2PointDistanceXY(D3DXVECTOR3 point1, D3DXVECTOR3 point2)
	{
		D3DXVECTOR3 distanceVector;
		distanceVector.x = pow(point1.x - point2.x, 2);
		distanceVector.y = pow(point1.y - point2.y, 2);

		return sqrt(distanceVector.x + distanceVector.y);
	}



	// -----------------------------------
	// Desc: Caculating the angle between a line through 2 point with [y = 0] line.
	// Point 1 is the base.
	// Return value in Radian.
	// -----------------------------------
	static double caculateLineAngle(D3DXVECTOR3 point1, D3DXVECTOR3 point2)
	{
		return
			CircleEquation(point1, point2).getDegreeFromCoordinates(point2);
	}
};

#endif