//==============================
// Last change: 26/9/2014
// T6_Framework
// QuadraticEquation v1.0
// This class allow you to create a Line or Parabol Equation to get the Coodirate from input x or y.
//==============================
// it form: ax^2 + bx + c = y
//==============================
#ifndef __QUADRATIC_EQUATION_H__
#define __QUADRATIC_EQUATION_H__

#include <vector>

#include "Equation.h"
#include "DX9Framework\SmartPointer.h"

class QuadraticEquation : public Equation
{
public:
	QuadraticEquation(double a = 0, double b = 0, double c = 0);
	QuadraticEquation(const QuadraticEquation& source);
	~QuadraticEquation(void);

	vector<double>* caculateSolutionForZeroY();

	vector<double>* getXFromY(const double& y);

	SmartPointer<QuadraticEquation> operator+ (const QuadraticEquation& secondEquation);
	SmartPointer<QuadraticEquation> operator- (const QuadraticEquation& secondEquation);

	friend static SmartPointer<QuadraticEquation> operator- (const QuadraticEquation& equation);

private:
	vector<double>* caculateLineEquation();
	vector<double>* caculateParabolEquation();
};

#endif