#pragma once
#include<vector>
#include<stack>
#include"Label.h"
#include"Subject.h"
using namespace std;
// 프레임 만들어 내는 팩토리 클래스
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