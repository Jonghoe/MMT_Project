#pragma once
#include"FrameFactory.h"
#include"Subject.h"
#include"Frame.h"
#include<vector>

//Frame ���� Ŭ����
class FrameController :private Subject
{
	typedef unsigned ID;
	friend class Classification;
private:
	// ��ID ���� ��ũID������ ����  [Mark][Label]

	static FrameController * Alive;			// �̱۷��� ��� ����� ���� ����
	FrameFactory * Factory;					//  Frame�� ����� Ŭ���� ������
	vector<Frame*> mvObVc;		      		// ������Ʈ�� ������ ����
	vector<Label*> mvHand;					// ������Ʈ�� �հ��� �迭

	vector<Label*> mvCreateFr;				// ������ Frame ���� ����
	vector<Frame*> mvDeleteFr ;				// ������ Frame ���� ����

	Frame* mvFr[20][3];  					// ������ ���� �迭
	vector<Label*> mvBuffer;				// ������Ʈ�� ���� ���� 
	ID mvPriority;							// Frame ������ �켱����
	FrameController::FrameController() :mvObVc(),mvHand(),mvCreateFr(),mvDeleteFr(),mvBuffer(){
		Factory = FrameFactory::mMakeFactory();
		mvPriority = 255;
		for (int i = 0; i < 20;++i)
			fill(*(mvFr+i), *(mvFr+i) + 3, nullptr);
	}
	void mvModifyPr(size_t l,size_t r);					//Priority�� �����ϸ鼭 ���� ���� (������������)
	void mRegistOb(Observer*);				//Observer(Frame) ���
	void mUnRegistOb(Observer*);			//Observer(Frame) ����
	bool mvFindOb(Label* Lb){				//Observer(Frame) �˻�
		void* tmp = mvFr[Lb->mGetMarkID()][Lb->mGetID()];
		return mvFr[Lb->mGetMarkID()][Lb->mGetID()] != nullptr;
	}
	void mNotify();							//Observer(Frame) ������Ʈ
	void mvCreate();	    				//Frame ����
	
	void mvAdjust();						//Frame ������ ���� ����
	void mvDeleteFrame();					//Frame ����

	void mPushMark(Label* lb){ mvBuffer.push_back(lb); }//��ũ ������ ����
	void mPushEvent(Label* lb){ mvBuffer.push_back(lb); }//�̺�Ʈ ������ ����

public:
	
	static FrameController* mMakeController(){	// �̱۷��� ���� �Լ�
		if (Alive == nullptr)
			Alive = new FrameController();
		return Alive;
	}

	void mDeleteController(){				// �̱۷��� �ı� �Լ�
		if (Alive == nullptr)
			return;
		delete this;
	}
	void mControll();
	
};
