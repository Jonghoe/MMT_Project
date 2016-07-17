#pragma once
#include"Observer.h"
#include"opencv.hpp"
// ������ ( â���� �ǹ� )
//�켱���� ������ ��� ���� ���� ������ ����.
class Frame : public Observer{
/*					�뷫���� Ʋ�� ����							*/
protected:
	virtual void mvAction() = 0;
	static int FrameNum;
	int mvPriority;							//Frame�� �켱����
	int mvHdSz;								//Frame���� �հ��� ��
	cv::Point mvHdAr[10];					//Frame���� �հ��� �迭
	cv::Point mvCenter, mvLT, mvRB;			//Frame �߽�,������ �����ʾƷ� ��ǥ
	bool Alive;								//Frame�� ���� ����
	const int mvMarkID;						//Mark��
	const int mvLabelID;					//�󺧰�
	
public:
	void mvUpdate(void* info){};
	Frame(int MID=0,int LID=0):mvMarkID(MID),mvLabelID(LID){ //MarkID����LabelID�����޾� ����
		mvHdSz = 0, Alive = true;
		mvPriority = FrameNum--;
	}
	int mGetPriority(){ return mvPriority; }//�켱���� ��ȯ
	bool mIn(cv::Point Loc){				// Loc�� Frame �ȿ� �ִ��� Ȯ��
		bool inCol = mvLT.x < Loc.x&&Loc.x < mvRB.x;
		bool inRow = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return inCol&&inRow;
	}
};