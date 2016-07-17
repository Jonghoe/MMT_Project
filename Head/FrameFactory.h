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
private:
	static FrameFactory * Alive;			//�̱۷��� �������� ���� ����

public:
	Frame* FrameFactory::mvCreate(Label* _label);// ������ ���� 
												 // ���� Mark������ Frame���� ����

	static FrameFactory* mMakeFactory(){	//�̱۷��� ���� �Լ�
		if (Alive == NULL)
			Alive = new FrameFactory();
		return Alive;
	}
	void mDeleteController(){				// �̱۷��� �ı� �Լ�
		if (Alive == NULL)
			return;
		delete this;
	}
};