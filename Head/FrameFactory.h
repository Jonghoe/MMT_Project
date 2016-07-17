#pragma once
#include<vector>
#include<stack>
#include"Label.h"
#include"Subject.h"
#include"Frame.h"
using namespace std;
// 프레임 만들어 내는 팩토리 클래스
class FrameFactory 
{
private:
	static FrameFactory * Alive;			//싱글레톤 기법사용을 위한 변수

public:
	Frame* FrameFactory::mvCreate(Label* _label);// 프레임 생성 
												 // 라벨의 Mark값으로 Frame종류 결정

	static FrameFactory* mMakeFactory(){	//싱글레톤 생성 함수
		if (Alive == NULL)
			Alive = new FrameFactory();
		return Alive;
	}
	void mDeleteController(){				// 싱글레톤 파괴 함수
		if (Alive == NULL)
			return;
		delete this;
	}
};