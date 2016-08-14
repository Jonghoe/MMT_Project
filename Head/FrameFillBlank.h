#pragma once
#include"Frame.h"
using namespace std;
//¿¹½Ã Frame 
class FillBlank :public Frame
{
protected:
	void mAction();
public:
	void mvUpdate(void* info);
	FillBlank(int MID,int LID,ID id) :Frame(MID,LID,id){};

};