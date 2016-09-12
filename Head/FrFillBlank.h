#pragma once
#include"Frame.h"
using namespace std;
//¿¹½Ã Frame 
class FrFillBlank :public Frame
{
protected:
	virtual void mvStart();
	virtual void mvStop();
	virtual void mvPlay();
public:
	FrFillBlank(Label* lb, ID id, size_t w, size_t h) :Frame(lb, id, w, h){ cout << "Create FrFillBlank" << endl; }
};