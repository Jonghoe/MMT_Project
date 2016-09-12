#pragma once
#include"Shape.h"
class Circle : public Shape{
	double mvRadius;
public:
	Circle():Shape(){}
	Circle(const Point& p) :Shape(p){}
	Circle(const Point& p, double r) :Shape(p, r*2, r*2), mvRadius(r){}
	void mSetR(double r){ mvRadius = r; mvWidth = 2 * r; mvHeight = 2 * r; }
	const double& mGetR()const{ return mvRadius; }
	double& mGetR(){ return mvRadius; }
	virtual double mArea()const{return 2 * mvRadius*mvRadius*CV_PI;}
	virtual void mRotate(double degree){ ; }
};