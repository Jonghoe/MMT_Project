#pragma once
#include"Observer.h"
#include"opencv.hpp"
// 프레임 ( 창들을 의미 )
class Frame : public Observer{
protected:
	static int FrameNum;
	int mvPriority;
	int mvHdSz;
	cv::Point mvHdAr[10];
	cv::Point mvCenter, mvLT, mvRB;
	bool Alive;
	const int mvMarkID;
	const int mvLabelID;
	
public:
	int mGetPriority(){ return mvPriority; }
	bool mIn(cv::Point Loc){
		bool inCol = mvLT.x < Loc.x&&Loc.x < mvRB.y;
		bool inRow = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return inCol&&inRow;
	}
	Frame(int MID=0,int LID=0):mvMarkID(MID),mvLabelID(LID){ 
		mvHdSz = 0, Alive = true;
		mvPriority = FrameNum--;
	}
};