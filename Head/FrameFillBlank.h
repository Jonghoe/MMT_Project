#pragma once
#include"Frame.h"
using namespace std;
//¿¹½Ã Frame 
class FillBlank :public Frame
{
protected:
	void mvAction();
public:
	void mvUpdate(void* info);
	FillBlank(int MID=0,int LID =0) :Frame(MID,LID){};

};