#pragma once
#include<Windows.h>
#include<process.h>
#include"Common.h"
#include"Observer.h"
#include"Label.h"
#include"EventHandler.h"
using namespace std;
using namespace cv;
// ������ ( window�� �ǹ� )
// �⺻������ �����Ǵ� ������(Fr�ν���)  --- ��ũ�� ���� ����� ������(MF���� ����)���� ������.
class Frame : public Observer{
	
protected:
	bool mvBaseFr;								//Frame�� Base���������� �ƴ��� Ȯ��
	State mvSt;									//Frame�� ����
	EventHandler mvEH;							//Frame�� �̺�Ʈ �ڵ鷯
	ID mvPriority;								//Frame�� �켱����
	int mvHdSz;									//Frame���� �հ��� ��
	Point mvHdAr[10];							//Frame���� �հ��� �迭
	Point mvCenter, mvLT, mvRB;					//Frame �߽�,������ �����ʾƷ� ��ǥ
	bool Alive;									//Frame�� ���� ����
	bool mvEnd;									//Frame ����� true
	const ID mvMarkID;							//Mark��
	const ID mvLabelID;							//�󺧰�
	size_t mvWidth, mvHeight;					//window�� �ʺ�� ����
	HANDLE mvThread;
	static unsigned WINAPI mainPerform(void*);
	virtual void mvStart(){};					//������ ù ���۽� ���� �Լ�
	virtual void mvStop(){};					//������ ������ ���� �Լ�
	virtual void mvPlay(){};					//������ ������ ���� �Լ�
	void mvUpdate(){							//������ ������Ʈ�� ���� �Լ�
		mCatchMotion(mvEH.mPutMotion());
	}
public:
	Frame(Label* lb, ID id, size_t w, size_t h) :mvEH(&mvCenter),
	mvMarkID(lb->mGetMarkID()),mvLabelID(lb->mGetID()),
	mvLT(lb->mGetCenter()),mvWidth(w),mvHeight(h){ //MarkID����LabelID�����޾� ����
		mvBaseFr = true;
		mvSt = PLAY;
		mvRB.x = mvLT.x + mvWidth;
		mvRB.y = mvLT.y + mvHeight;
		mvHdSz = 0;
		Alive = false;
		mvPriority = id;
		mvEnd = false;
	}
	Point& mGetCenter(){ return mvCenter; }
	ID mGetMID(){ return mvMarkID; }
	ID mGetLID(){ return mvLabelID; }
	ID mGetPriority(){ return mvPriority; }		//�켱���� ��ȯ
	void mSetPriority(ID pr){ mvPriority = pr; }//�켱���� ����
	State mGetState(){ return mvSt; }
	void mCatchMotion(vector<Motion> m){ 
		for (size_t i = 0; i < m.size(); ++i)
		m[i](this); 
	}
	bool mIsAllSet(){ return mvEH.mGetUpdate(); }//�������ӿ��� ��� �̺�Ʈ�� ���Դ��� Ȯ��
	void mUpdate(void* st){ mvSt = *(State*)st; }//Subject�κ��� ������Ʈ
	void mStart(){ mvSt = START; }
	void mStop(){ mvSt = STOP; }				//����
	void mPlay(){ mvSt = PLAY; }				//����
	void mEnd(){ mvSt = END; }					//����
	bool mIsEnd(){ return mvSt == END; }
	bool mGetAlive(){ return Alive; }
	void mAlive(bool _live){ Alive = _live; }
	void mEventPush(Event& e){ mvEH.mPush(e); }
	
	EventHandler* mGetEventHandler(){ return &mvEH; }
	HANDLE& mGetHandle(){ return mvThread; }
	void mResize(double v){
		mvWidth *= (int)v;
		mvHeight *= (int)v;
		mvCenter *= (int)v;
		mvLT *= (int)v;
		mvRB *= (int)v;
	};

	bool mIn(const Point& Loc)const{				// Loc�� Frame �ȿ� �ִ��� Ȯ��
		bool inCol = mvLT.x < Loc.x&&Loc.x < mvRB.x;
		bool inRow = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return inCol&&inRow;
	}
	bool mGetBaseFr(){ return mvBaseFr; }
	virtual void mAction() {
		mvThread = (HANDLE)_beginthreadex(NULL,NULL,mainPerform,this,NULL,NULL);
	}
};