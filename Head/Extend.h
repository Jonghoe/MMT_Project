#pragma once
#include"Motion.h"
class Extend :public Motion{
public:
	Extend() :Motion(){ }
	Extend(int _r) :Motion(_r){ };
	Extend(int _r, int _c) :Motion(_r, _c){ }
	Extend(Point l) :Motion(l){}
	void operator()(void* fr);
};