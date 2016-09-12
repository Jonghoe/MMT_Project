#pragma once
#include"opencv.hpp"
using  cv::Point;
class Shape{
protected:
	Point mvCenter;
	double mvWidth, mvHeight;
public:
	Shape() :mvCenter(), mvWidth(), mvHeight(){}
	Shape(const Point& p) :mvCenter(p), mvWidth(), mvHeight(){}
	Shape(const Point& p, double w, double h) :mvCenter(p), mvWidth(w), mvHeight(h){}
	virtual double mArea()const = 0;
	virtual void mRotate(double degree) = 0;

	const Point& mGetCenter()const{ return mvCenter; }
	Point& mGetCenter(){ return mvCenter; }
	void mSetCenter(const Point& p){ mvCenter = p; }
	const double& mGetW()const{ return mvWidth; }
	double& mGetW(){ return mvWidth; }
	void mSetW(double w){mvWidth = w;}
	const double& mGetH()const{ return mvHeight; }
	double& mGetH(){ return mvHeight; }
	void mSetH(double h){ mvHeight = h; }
};