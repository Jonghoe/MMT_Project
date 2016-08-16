#pragma once
#include<Windows.h>
#include<process.h>
#include"Observer.h"
#include"opencv.hpp"
#include"Label.h"
using namespace std;
// ������ ( â���� �ǹ� )
//�켱���� ������ ��� ���� ���� ������ ����.
class Frame : public Observer{
	
	static unsigned WINAPI mainPerform(void*);

protected:
	ID mvPriority;							//Frame�� �켱����
	int mvHdSz;								//Frame���� �հ��� ��
	cv::Point mvHdAr[10];					//Frame���� �հ��� �迭
	cv::Point mvCenter, mvLT, mvRB;			//Frame �߽�,������ �����ʾƷ� ��ǥ
	bool Alive;								//Frame�� ���� ����
	bool mvEnd;								//Frame ����� true
	bool mvStop;							//Frame ������ true
	const ID mvMarkID;						//Mark��
	const ID mvLabelID;						//�󺧰�
	int mvWidth, mvHeight;
public:
	
	Frame(Label* lb,ID id):mvMarkID(lb->mGetMarkID()),mvLabelID(lb->mGetID()),
	mvLT(lb->mGetCenter()){ //MarkID����LabelID�����޾� ����
		mvHdSz = 0;
		Alive = false;
		mvPriority = id;
		mvEnd = false;
		mvStop = false;
	}
	void mvUpdate(Label* info){};				
	void mStop();
	ID mGetMID(){ return mvMarkID; }
	ID mGetLID(){ return mvLabelID; }
	ID mGetPriority(){ return mvPriority; }//�켱���� ��ȯ
	void mSetPriority(ID pr){ mvPriority = pr; }//�켱���� ����
	void mAlive(bool _live){ Alive = _live; }
	void mEnd(){ mvEnd = true; }
	bool mGetAlive(){ return Alive; }
	bool mIn(cv::Point Loc){				// Loc�� Frame �ȿ� �ִ��� Ȯ��
		bool inCol = mvLT.x < Loc.x&&Loc.x < mvRB.x;
		bool inRow = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return inCol&&inRow;
	}
	bool mGetStop(){ return mvStop; }
	bool mGetEnd(){ return mvStop; }
	virtual void mAction() {
		_beginthreadex(NULL,NULL,mainPerform,this,NULL,NULL);
	}
};