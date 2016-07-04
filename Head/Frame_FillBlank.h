#pragma once
#include"Frame.h"
using namespace std;
class FillBlank :public Frame
{
protected:
	void mvAction();
public:
	void mvUpdate(void* info);
	FillBlank(int ID=0) :Frame(ID){};

};