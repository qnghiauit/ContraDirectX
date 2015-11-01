#ifndef __MATH_DEFAULT_CONSTATNT_H__
#define __MATH_DEFAULT_CONSTATNT_H__

#include <d3dx9.h>

#define POINT_O D3DXVECTOR3(0, 0, 0)

#define DEGREE_TO_RADIAN(A)			((A) * 3.14 / 180)
#define RADIAN_TO_DEGREE(A)			((A) * 180 / 3.14)


#define PI			3.14159265358979323
#define PI_2		(2 * PI)

// About rotate macros.
#define ANGLE_NO					0.0F
#define ANGLE_CLOCKWISE(A)			(-A)
#define ANGLE_COUNTER_CLOCKWISE(A)	A
#define ANGLE_DEGREE_LIM			360
#define DEGREE_TO_RADIAN(A)			((A) * PI / 180.0)
#define RADIAN_TO_DEGREE(A)			((A) * 180.0 / PI)

#define GET_SIGN(A)	((A > 0) - (A < 0))

#endif