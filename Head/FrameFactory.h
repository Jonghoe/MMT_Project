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
	typedef unsigned ID;
private:

public:
	Frame* operator()(Label* _label,ID id);// 프레임 생성 
										// 라벨의 Mark값으로 Frame종류 결정
};