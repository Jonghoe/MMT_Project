#pragma once
#include<vector>
#include<stack>
#include"Label.h"
#include"Subject.h"
using namespace std;
// ������ ����� ���� ���丮 Ŭ����
class FrameFactory 
{
private:
	static FrameFactory * Alive;

public:
	Observer* FrameFactory::mvCreate(Label* _label);

	static FrameFactory* mMakeFactory(){
		if (Alive == NULL)
			Alive = new FrameFactory();
		return Alive;
	}
	
};