#include "QuadraticEquation.h"

// a*X^2 + b*x + c = y
QuadraticEquation::QuadraticEquation(double a, double b, double c)
{
	_parameters = new EquationParameter(a, b, c);
}



QuadraticEquation::QuadraticEquation(const QuadraticEquation& source)
{
	_parameters	=	new EquationParameter(*source._parameters);
}



QuadraticEquation::~QuadraticEquation(void)
{
	delete _parameters;
}



vector<double>* QuadraticEquation::caculateLineEquation()
{
	if (_parameters->b == 0)
		return 0;
	else {
		vector<double>* solution = new vector<double>;
		solution->push_back((-_parameters->c) / _parameters->b);
		return solution;
	}
}



vector<double>* QuadraticEquation::caculateParabolEquation()
{
	double delta =	_parameters->b * _parameters->b -
					4 * _parameters->a * _parameters->c;	// b*b - 4*a*c

	if (delta < 0)
		return NULL;
	else {

		vector<double>* solution = new vector<double>;

		if (delta == 0) {
			solution->push_back((-_parameters->b) / (2 * _parameters->a));	// -b / 2*a
		}
		else {
			solution->push_back((-_parameters->b) + sqrt(delta) / (2 * _parameters->a));	// (-b + sqrt(delta)) / (2*a)
			solution->push_back((-_parameters->b) - sqrt(delta) / (2 * _parameters->a));	// (-b - sqrt(delta)) / (2*a)
		}

		return solution;
	}
}



vector<double>* QuadraticEquation::caculateSolutionForZeroY()
{
	if (_parameters->a == 0)
		return caculateLineEquation();
	else
		return caculateParabolEquation();
}



vector<double>* QuadraticEquation::getXFromY(const double& y)
{
	QuadraticEquation equationAfterReplatedY = *(*this - QuadraticEquation(0, 0, y));
	
	return equationAfterReplatedY.caculateSolutionForZeroY();
}



SmartPointer<QuadraticEquation> QuadraticEquation::operator+ (const QuadraticEquation& secondEquation)
{
	double a, b, c;
	a = _parameters->a + secondEquation._parameters->a;
	b = _parameters->b + secondEquation._parameters->b;
	c = _parameters->c + secondEquation._parameters->c;

	return SmartPointer<QuadraticEquation>(new QuadraticEquation(a, b, c));
}



SmartPointer<QuadraticEquation> operator- (const QuadraticEquation& equation)
{
	return SmartPointer<QuadraticEquation>(
		new QuadraticEquation(	-equation._parameters->a,
								-equation._parameters->b,
								-equation._parameters->c));
}



SmartPointer<QuadraticEquation> QuadraticEquation::operator- (const QuadraticEquation& secondEquation)
{
	return (*this + *(- secondEquation));
}