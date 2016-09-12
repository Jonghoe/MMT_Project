#pragma once
#include"opencv.hpp"
using cv::Point;
class Motion{
protected:
	int row, col;
public:
	Motion() :row(), col(){ }
	Motion(int _r) :row(_r), col(_r){ }
	Motion(int _r , int _c ) :row(_r), col(_c){ }
	Motion(Point l) :row(l.y), col(l.x){}
	virtual void operator()(void* p);
};
