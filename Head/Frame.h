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
	bool Alive;
	cv::Point mvCenter,mvLT,mvRB;
	const int mvMarkID;
	
public:
	int mGetPriority(){ return mvPriority; }
	bool mIn(cv::Point Loc){
		bool COL = mvLT.x < Loc.x&&Loc.x < mvRB.y;
		bool ROW = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return COL&&ROW;
	}
	Frame(int ID=0):mvMarkID(ID){ 
		mvHdSz = 0, Alive = false;
		mvPriority = FrameNum--;
	}
};