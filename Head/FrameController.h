#pragma once
#include"FrameFactory.h"
#include"Subject.h"
#include"Frame.h"
#include<vector>

//Frame ���� Ŭ����
class FrameController :public Subject
{
	typedef unsigned ID;
private:
											// ��ID ���� ��ũID������ ����

	static FrameController * Alive;			// �̱۷��� ��� ����� ���� ����
	FrameFactory * Factory;					//  Frame�� ����� Ŭ���� ������
	vector<Frame*> mvObVc;		      		// ������Ʈ�� ������ ����
	vector<Label*> mvHand;					// ������Ʈ�� �հ��� �迭
	vector<Frame*> mvDeleteFr;				// ������ Frame ���� ����
	vector<Label*> mvCreateFr;				// ������ Frame ���� ����
	Frame* mvFr[20][3];  					// ������ ���� �迭
	vector<Label*>	mvVec;					// ������Ʈ�� ���� ���� 
	ID mvPriority;							// Frame ������ �켱����
	FrameController::FrameController() :mvObVc(),mvHand(){
		Factory = FrameFactory::mMakeFactory();
		mvPriority = 255;
	}
	void mvSortChange();					//Priority�� �����ϸ鼭 ���� ���� (������������)
	void mRegistOb(Observer*);				//Observer(Frame) ���
	void mUnRegistOb(Observer*);			//Observer(Frame) ����
	bool mvFindOb(Label* Lb){				//Observer(Frame) �˻�
		return mvFr[Lb->mGetMarkID()][Lb->mGetID()] != nullptr;
	}
	void mvDeleteFrame();
	void mNotify();							//Observer(Frame) ������Ʈ
	void mvCreate();	    				//Frame ����
	void mvFindDelete();
public:
	void mPush(Label* lb){ mvVec.push_back(lb); }//�������� ����
	
	static FrameController* mMakeSubject(){	// �̱۷��� ���� �Լ�
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
