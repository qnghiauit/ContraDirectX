//==============================
// Last change: 26/9/2014
// T6_Framework
// Equation v1.0
// This the base class for the other kinds of Equation
// + Quadratic Equation
// + Circling Equation
//==============================
#ifndef __EQUATION_H__
#define __EQUATION_H__

#include <vector>
using namespace std;

union EquationParameter
{
	double	_parameter[3];

	struct {
		double c;
		double b;
		double a;
	};

	struct {
		double r;
		double b;
		double a;
	};

	struct {
		double r;
		double y;
		double x;
	};

	EquationParameter(double aOrX, double bOrY, double cOrR)
	{
		_parameter[0] = cOrR;
		_parameter[1] = bOrY;
		_parameter[2] = aOrX;
	}

	EquationParameter(const EquationParameter& source)
	{
		memcpy(this, &source, sizeof(source));
	}
};

class Equation
{
protected:
	EquationParameter* _parameters;
};

#endif