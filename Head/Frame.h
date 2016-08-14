#pragma once
#include"Observer.h"
#include"opencv.hpp"
#include"Label.h"
// ������ ( â���� �ǹ� )
//�켱���� ������ ��� ���� ���� ������ ����.
class Frame : public Observer{
/*					�뷫���� Ʋ�� ����							*/
protected:
	ID mvPriority;							//Frame�� �켱����
	int mvHdSz;								//Frame���� �հ��� ��
	cv::Point mvHdAr[10];					//Frame���� �հ��� �迭
	cv::Point mvCenter, mvLT, mvRB;			//Frame �߽�,������ �����ʾƷ� ��ǥ
	bool Alive;								//Frame�� ���� ����
	bool IsEnd;								//Frame ����� true
	const ID mvMarkID;						//Mark��
	const ID mvLabelID;						//�󺧰�
public:
	
	Frame(int MID,int LID,ID id):mvMarkID(MID),mvLabelID(LID){ //MarkID����LabelID�����޾� ����
		mvHdSz = 0;
		Alive = false;
		mvPriority = id;
	}
	void mvUpdate(Label* info){};				
	void mStop();
	ID mGetMID(){ return mvMarkID; }
	ID mGetLID(){ return mvLabelID; }
	ID mGetPriority(){ return mvPriority; }//�켱���� ��ȯ
	ID mSetPriority(){ return mvPriority; }//�켱���� ��ȯ
	void mAlive(){ Alive = !Alive; }
	bool mGetAlive(){ return Alive; }
	bool mIn(cv::Point Loc){				// Loc�� Frame �ȿ� �ִ��� Ȯ��
		bool inCol = mvLT.x < Loc.x&&Loc.x < mvRB.x;
		bool inRow = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return inCol&&inRow;
	}
	virtual void mAction() = 0;
};