#include "CircleEquation.h"

CircleEquation::CircleEquation(D3DXVECTOR3 center, D3DXVECTOR3 aPointOnCircle)
{
	_parameters = new EquationParameter(
						center.x,
						center.y,
						sqrt(
							pow(center.x - aPointOnCircle.x, 2) +
							pow(center.y - aPointOnCircle.y, 2)));
}



CircleEquation::CircleEquation(double x, double y, double r)
{
	_parameters = new EquationParameter(x, y, r);
}



CircleEquation::CircleEquation(const CircleEquation& source)
{
	_parameters	=	new EquationParameter(*source._parameters);
}

CircleEquation::CircleEquation(D3DXVECTOR3 center, double radius)
{
	_parameters = new EquationParameter(center.x, center.y, radius);
}



CircleEquation::~CircleEquation(void)
{
	delete _parameters;
}



D3DXVECTOR3 CircleEquation::getCoordinatesFromDegree(const double& t)
{
	return 
		D3DXVECTOR3((float)(_parameters->x + _parameters->r * Default::cos(DEGREE_TO_RADIAN(t))),
					(float)(_parameters->y + _parameters->r * Default::sin(DEGREE_TO_RADIAN(t))),
				DEFAULT_DEPTH);
}



double CircleEquation::getDegreeFromCoordinates(D3DXVECTOR3 point)
{
	double variable;
	variable = (point.x - _parameters->a) / _parameters->r;
	return (Default::acos(variable) * GET_SIGN(((int)(point.y - this->getCenter().y))));
}



void CircleEquation::increaseR(double difference)
{
	_parameters->r += difference;
}



void CircleEquation::setCenter(double x, double y)
{
	_parameters->x = x;
	_parameters->y = y;
}


EquationParameter CircleEquation::getParameter()
{
	return *_parameters;
}


D3DXVECTOR3	CircleEquation::getCenter()
{
	return D3DXVECTOR3((float)_parameters->x, (float)_parameters->y, DEFAULT_DEPTH);
}


void CircleEquation::setR(double r)
{
	_parameters->r = r;
}