#pragma once
#include"Motion.h"
class Rotate :public Motion{
public:
	Rotate() :Motion(){ }
	Rotate(int _r) :Motion(_r){ }
	Rotate(int _r, int _c) :Motion(_r, _c){ }
	Rotate(Point l) :Motion(l){}
	void operator()(void* fr);
};