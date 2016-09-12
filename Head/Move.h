#pragma once
#include"Motion.h"
class Move :public Motion{
public:
	Move() :Motion(){ }
	Move(int _r ) :Motion(_r){ }
	Move(int _r , int _c ) :Motion(_r, _c){ }
	Move(Point l) :Motion(l){}
	void operator()(void* fr);
};