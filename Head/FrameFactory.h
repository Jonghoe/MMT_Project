#pragma once
#include<vector>
#include<stack>
#include"Label.h"
#include"Subject.h"
#include"Frame.h"
using namespace std;
// ������ ����� ���� ���丮 Ŭ����
class FrameFactory 
{
	typedef unsigned ID;
private:

public:
	Frame* operator()(Label* _label,ID id);// ������ ���� 
										// ���� Mark������ Frame���� ����
};