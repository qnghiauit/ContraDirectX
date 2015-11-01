#ifndef __DEFAULT_METHODS_H__
#define __DEFAULT_METHODS_H__

#include <d3dx9.h>
#include <math.h>

#include "DefaultStructure.h"
#include "DefaultConstant.h"
#include "GraphicAlgorimths2D/MathDefaultConstant.h"

#include <vector>
using namespace std;

class Default
{
public:

	// -----------------------------------------------
	// Name: Default:checkIfBounded()
	// Desc: Check 2 passed Rectangle if it bounded each other.
	// -----------------------------------------------
	static bool checkIfBounded(const RECT* rect1, const RECT* rect2)
	{
		if ((rect2->right - rect1->left) < 0 || (rect1->right - rect2->left) < 0)
			return false;
		if ((rect2->top - rect1->bottom) < 0 || (rect1->top - rect2->bottom) < 0)
			return false;

		return true;
	}

	// -----------------------------------------------
	// Name: Default:checkIfBoundedNotEqual()
	// Desc: Check 2 passed Rectangle if it bounded each other.
	// 2 RECT JUST BOUND BY LINE ARE TREATED AS NOT BOUNDED
	// -----------------------------------------------
	static bool checkIfBoundedNotEqual(const RECT* rect1, const RECT* rect2)
	{
		if ((rect2->right - rect1->left) <= 0 || (rect1->right - rect2->left) <= 0)
			return false;
		if ((rect2->top - rect1->bottom) <= 0 || (rect1->top - rect2->bottom) <= 0)
			return false;

		return true;
	}


	// -----------------------------------------------
	// Name: Default:createRect()
	// Desc: Fast create a RECT follow passed variable.
	// -----------------------------------------------
	static RECT createRECT(long left, long right, long top, long bottom)
	{
		RECT result;

		result.left = left;
		result.right = right;
		result.top = top;
		result.bottom = bottom;

		return result;
	}


	// -----------------------------------------------
	// Name: Default:createRectFromCenter()
	// Desc: Fast create a RECT follow passed variable.
	// -----------------------------------------------
	static RECT createRectFromCenter(D3DXVECTOR3 position, Size frameSize)
	{
		return createRectFromDrawCenter(position, frameSize, DrawCenter::CENTER);
	}


	static RECT createRectFromDrawCenter(D3DXVECTOR3 position, Size frameSize, DrawCenter drawCenter)
	{
		RECT result;
		switch (drawCenter)
		{
		case TOPLEFT:
			result.left = (long)(position.x);
			result.top = (long)(position.y);
			result.right = (long)(position.x + frameSize.width);
			result.bottom = (long)(position.y - frameSize.height);
			break;
		case TOPRIGHT:
			result.left = (long)(position.x - frameSize.width);
			result.top = (long)(position.y);
			result.right = (long)(position.x);
			result.bottom = (long)(position.y - frameSize.height);
			break;
		case BOTTOMCENTER:
			result.left = (long)(position.x - frameSize.width / 2.0);
			result.top = (long)(position.y + frameSize.height);
			result.right = (long)(position.x + frameSize.width / 2.0);
			result.bottom = (long)(position.y);
			break;
		case CENTER:
			result.left = (long)(position.x - frameSize.width / 2.0);
			result.top = (long)(position.y + frameSize.height / 2.0);
			result.right = (long)(position.x + frameSize.width / 2.0);
			result.bottom = (long)(position.y - frameSize.height / 2.0);
			break;
		case BOTTOMLEFT:
			result.left = (long)(position.x);
			result.top = (long)(position.y + frameSize.height);
			result.right = (long)(position.x + frameSize.width);
			result.bottom = (long)(position.y);
			break;
		case BOTTOMRIGHT:
			result.left = (long)(position.x - frameSize.width);
			result.top = (long)(position.y + frameSize.height);
			result.right = (long)(position.x);
			result.bottom = (long)(position.y);
			break;
		case TOPCENTER:
			result.left = (long)(position.x - frameSize.width / 2.0);
			result.top = (long)(position.y);
			result.right = (long)(position.x + frameSize.width / 2.0);
			result.bottom = (long)(position.y - frameSize.height);
			break;
		default:
			break;
		}
		return result;
	}


	// -----------------------------------------------
	// Name: Default:createRectFromCenter()
	// Desc: Fast create a RECT follow passed variable.
	// -----------------------------------------------
	static RECT createRectFromBottomCenter(D3DXVECTOR3 position, Size frameSize)
	{
		RECT result;

		result.left = (long)(position.x - frameSize.width / 2);
		result.right = (long)(position.x + frameSize.width / 2);
		result.top = (long)(position.y + frameSize.height);
		result.bottom = (long)(position.y);

		return result;
	}


	// -----------------------------------------------
	// Name: Default:createRectFrom2Image()
	// Desc: Fast create a RECT with center at bottom frame1 and top frame2.
	// -----------------------------------------------
	static RECT createRectFrom2Image(D3DXVECTOR3 position, Size frameSize1, Size frameSize2)
	{
		RECT result;

		result.left = (long)(position.x - frameSize1.width / 2);
		result.right = (long)(position.x + frameSize1.width / 2);
		result.top = (long)(position.y + frameSize1.height / 2);
		result.bottom = (long)(position.y);

		return result;
	}


	// -----------------------------------------------
	// Name: Default<T>:tostr()
	// Desc: Convert the passed content to a string, char* or wchar_t* based on compiler setting.
	// -----------------------------------------------
	template <class T>
	static tstring tostr(const T& content)
	{
		tstringstream os;
		os << content;
		return os.str();
	}


	// -----------------------------------------------
	// Name: Default:createVector3()
	// Desc: Fast create a D3DXVECTOR3 by passed variables.
	// -----------------------------------------------
	static D3DXVECTOR3 createVector3(float x, float y, float z)
	{
		D3DXVECTOR3 result;

		result.x = x;
		result.y = y;
		result.z = z;

		return result;
	}


	// -----------------------------------------------
	// Name: Default:computeCenter()
	// Desc: Fast create a D3DXVECTOR3 for the center of the rect.
	// -----------------------------------------------
	static D3DXVECTOR3 computeCenter(RECT rect)
	{
		D3DXVECTOR3 result;

		result.x = (rect.left + rect.right) / 2.0F;
		result.y = (rect.top + rect.bottom) / 2.0F;
		result.z = DEFAULT_DEPTH;

		return result;
	}


	// -----------------------------------------------
	// Name: Default:checkIfPointInRect()
	// Desc: To check if the imported point is contained by the rect.
	// Base on Decard Axis.
	// -----------------------------------------------
	static bool checkIfPointInRect(D3DXVECTOR3 point, RECT rect)
	{
		bool inRangeX = (((point.x - rect.right) < 0) &&
			((point.x - rect.left) > 0));

		bool inRangeY = (((point.y - rect.top) < 0) &&
			((point.y - rect.bottom) > 0));

		return (inRangeX & inRangeY);
	}


	static RECT multipleRectWithDouble(RECT rect, double scaleValue)
	{
		double width = rect.right - rect.left;
		double height = rect.top - rect.bottom;
		width *= scaleValue;
		height *= scaleValue;
		width /= 2.0;
		height /= 2.0;

		D3DXVECTOR3 center = Default::computeCenter(rect);

		RECT result;
		result.left = (LONG)(center.x - width);
		result.right = (LONG)(center.x + width);
		result.top = (LONG)(center.y + height);
		result.bottom = (LONG)(center.y - height);

		return result;
	}


	static int fixAngle(int angle)
	{
		while (angle >= 360 || angle < 0)
		{
			if (angle >= 360)
				angle -= 360;
			else
				angle += 360;
		}

		return angle;
	}


	static double sin(double angleInRandian)
	{
		static double result[360];
#pragma region Initialize
		static bool isInitialized = false;

		if (!isInitialized)
		{
			FOR(360)
				result[i] = -2;
			isInitialized = !isInitialized;
		}
#pragma endregion

		int angle = Default::fixAngle((int)RADIAN_TO_DEGREE(angleInRandian));
		if (result[angle] < -1)
			result[angle] = std::sin(angleInRandian);

		return result[angle];
	}


	static double cos(double angleInRandian)
	{
		static double result[360];
#pragma region Initialize
		static bool isInitialized = false;

		if (!isInitialized)
		{
			FOR(360)
				result[i] = -2;
			isInitialized = !isInitialized;
		}
#pragma endregion

		int angle = Default::fixAngle((int)RADIAN_TO_DEGREE(angleInRandian));
		if (result[angle] < -1)
			result[angle] = std::cos(angleInRandian);

		return result[angle];
	}


	static double asin(double sinValue)
	{
		static double result[180];
#pragma region Initialize
		static bool isInitialized = false;

		if (!isInitialized)
		{
			FOR(180)
				result[i] = -2;
			isInitialized = !isInitialized;
		}
#pragma endregion

		int angle;
		FOR(180)
			if (result[i] == sinValue)
			{
			angle = i;
			goto RETURN;
			}

		angle = (int)RADIAN_TO_DEGREE(std::asin(sinValue));
		result[angle] = sinValue;

	RETURN:;
		return DEGREE_TO_RADIAN(angle);
	}

	// returning degree instead of radian
	static double acos(double cosValue)
	{
		static double result[180];
#pragma region Initialize
		static bool isInitialized = false;

		if (!isInitialized)
		{
			FOR(180)
				result[i] = -2;
			isInitialized = !isInitialized;
		}
#pragma endregion
		int angle;

		FOR(180)
			if (result[i] == cosValue)
			{
			angle = i;
			goto RETURN;
			}

		angle = (int)RADIAN_TO_DEGREE(std::acos(cosValue));
		result[angle] = cosValue;

	RETURN:;

		return angle;
	}


	static double computeDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
	{
		return std::sqrt(pow(pos1.x - pos2.x, 2.0) + pow(pos1.y - pos2.y, 2.0));
	}


	static double round(double value, double coefficient)
	{
		return coefficient * (int)(value / coefficient + 0.5);
	}


	template<class T>
	static void swap(T& a, T& b)
	{
		T c = a;
		a = b;
		b = c;
	}


	static RECT moveRECT(RECT& sourceRECT, D3DXVECTOR3 moveDistance)
	{
		sourceRECT.left += moveDistance.x;
		sourceRECT.right += moveDistance.x;
		sourceRECT.top += moveDistance.y;
		sourceRECT.bottom += moveDistance.y;

		return sourceRECT;
	}

	
	static vector<tstring> splitString(tstring source, char splitChar = '\n')
	{
		vector<tstring> result;
		int currentIndex = 0;
		FOR(source.length())
		{
			if (source[i] == splitChar)
			{
				result.push_back(tstring(source.substr(currentIndex, i)));
				currentIndex = i + 1;
			}
		}

		return result;
	}
};

#endif