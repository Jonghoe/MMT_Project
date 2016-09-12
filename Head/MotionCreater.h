#pragma once
#include"opencv.hpp"
#include"Common.h"
#include"Circle.h"
#include"Event.h"
#include"Motion.h"
#include"Move.h"
#include"Extend.h"
#include<vector>
using std::vector;
class MotionCreater{
private:
	Circle mvMakeChecker(vector<Event>& v);
public:
	MotionCreater(){}
	vector<Motion> operator()(Circle& prev,vector<Event>& v);
};