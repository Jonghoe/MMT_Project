#pragma once
#include"Frame.h"
using namespace std;
//¿¹½Ã Frame 
class FillBlank :public Frame
{
protected:
	static unsigned WINAPI mainPerform(void*);

public:
	void mvUpdate(void* info);
	FillBlank(Label* lb,ID id) :Frame(lb,id){
		mvWidth = 100;
		mvHeight = 100;
		mvRB.x = mvLT.x + mvWidth;
		mvRB.y = mvLT.y + mvHeight;
	};
	void mAction();
};