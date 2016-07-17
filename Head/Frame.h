#pragma once
#include"Observer.h"
#include"opencv.hpp"
// 프레임 ( 창들을 의미 )
//우선순위 생성을 어떻게 할지 아직 정하지 않음.
class Frame : public Observer{
/*					대략적인 틀만 잡음							*/
protected:
	virtual void mvAction() = 0;
	static int FrameNum;
	int mvPriority;							//Frame의 우선순위
	int mvHdSz;								//Frame안의 손가락 수
	cv::Point mvHdAr[10];					//Frame안의 손가락 배열
	cv::Point mvCenter, mvLT, mvRB;			//Frame 중심,왼쪽위 오른쪽아래 좌표
	bool Alive;								//Frame의 동작 여부
	const int mvMarkID;						//Mark값
	const int mvLabelID;					//라벨값
	
public:
	void mvUpdate(void* info){};
	Frame(int MID=0,int LID=0):mvMarkID(MID),mvLabelID(LID){ //MarkID값과LabelID값을받아 생성
		mvHdSz = 0, Alive = true;
		mvPriority = FrameNum--;
	}
	int mGetPriority(){ return mvPriority; }//우선순위 반환
	bool mIn(cv::Point Loc){				// Loc가 Frame 안에 있는지 확인
		bool inCol = mvLT.x < Loc.x&&Loc.x < mvRB.x;
		bool inRow = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return inCol&&inRow;
	}
};