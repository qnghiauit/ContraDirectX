//==============================
// Last change: 5/10/2014
// T6_Framework
// QuadraticEquation v1.1
// This class allow you to create a Circling Equation to get it coordinates by degree.
// 5/10/2014 : Add the function getDegreeFromCoodirate, fix the equation form.
// 5/10/2014 : Constructor from 2 point.
//==============================
// it form:
// x = a + r.cos(t)
// y = b + r.sin(t)
//==============================
#ifndef __CIRCLE_EQUATION_H__
#define __CIRCLE_EQUATION_H__

#include <vector>
#include <d3dx9.h>

#include "Equation.h"
#include "DX9Framework\DefaultMethods.h"
#include "DX9Framework\DefaultConstant.h"

#include "MathDefaultConstant.h"

class CircleEquation : public Equation
{
public:
	CircleEquation(D3DXVECTOR3 center, D3DXVECTOR3 aPointOnCircle);
	CircleEquation(double x = 0, double y = 0, double r = 0);
	CircleEquation(D3DXVECTOR3 center, double radius);
	CircleEquation(const CircleEquation& source);
	~CircleEquation(void);

	// -----------------------------------------------
	// Name:: CircleEquation::getCoodirateFromDegree()
	// Desc:: Caculating the coodirate on circle which place at input degree.
	// Import a value in 360 degree system, may be real number, as 22.5
	// -----------------------------------------------
	D3DXVECTOR3 getCoordinatesFromDegree(const double& t);


	// -----------------------------------------------
	// Name:: CircleEquation::getDegreeFromCoodirate()
	// Desc:: Caculating the degree between input point and const y line through circle center.
	// Import a value in Descartes Coordinates System, may be real number.
	// Return value in Degree.
	// Return a wrong value if the point isn't on Circle Line.
	// -----------------------------------------------
	double getDegreeFromCoordinates(D3DXVECTOR3 point);

	void setCenter(double x, double y);

	EquationParameter getParameter();

	D3DXVECTOR3	getCenter();

	void setR(double r);

	void increaseR(double difference);

private:
};

#endif