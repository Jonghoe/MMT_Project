#pragma once
#include"FrameFactory.h"
#include"Subject.h"
#include"Frame.h"
//Frame ���� Ŭ����
//���� ���� Ŭ����
class FrameSubject :public Subject
{
private:
	bool mvStop;							// ������� ���۽� ������Ʈ�� ���
											// ��� ���߰Բ� �ϴ� ����
	static int MarkID, LabelID;				// ��ũID ������ ��� ����
											// ��ID ���� ��ũID������ ����
	static FrameSubject * Alive;			// �̱۷��� ��� ����� ���� ����
	FrameFactory * Factory;					//  Frame�� ����� Ŭ���� ������
	int mvObArSz;							// ������ �迭�� ũ��
	int mvHdArSz;							// �հ��� �迭�� ũ��
	Frame* mvObArray[20];				// ������Ʈ�� ������ �迭
	Label* mvHand[20];						// ������Ʈ�� �հ��� �迭
	Frame* mvOb[20][20];					// ������ ���� �迭
	vector<Label*>	mvVec;					// ������Ʈ�� ���� ���� 
	FrameSubject::FrameSubject() :mvObArSz(0), mvHdArSz(0){
		Factory = FrameFactory::mMakeFactory();
	}
	void mRegistOb(Observer*);				//Observer ���
	void mUnRegistOb(Observer*);			//Observer ����
	bool mFindOb(Label* Lb);				//Observer �˻�
	void mNotify();							//Observer ������Ʈ
	void mvArrangement();					//Observer ����
	void mvHandThrow();						//�̺�Ʈ Throw
public:
	void mSetStop(bool s){					//���� ����
		mvStop = s;
	}
	void mPush(Label*);						//�������� ����
	static FrameSubject* mMakeSubject(){	// �̱۷��� ���� �Լ�
		if (Alive == NULL)
			Alive = new FrameSubject();
		return Alive;
	}

	void mDeleteController(){				// �̱۷��� �ı� �Լ�
		if (Alive == NULL)
			return;
		delete this;
	}
};
