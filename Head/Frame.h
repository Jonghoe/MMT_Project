#pragma once
#include"Observer.h"
#include"opencv.hpp"
// 프레임 ( 창들을 의미 )
class Frame : public Observer{
protected:
	int mvHdSz;
	cv::Point mvHdAr[10];
	Frame();
	bool Alive;
	cv::Point mvCenter,mvLT,mvRB;
	const int mvMarkID;
	
	bool mvIsIn(cv::Point Loc){
		bool COL = mvLT.x < Loc.x&&Loc.x < mvRB.y;
		bool ROW = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return COL&&ROW;
	}
public:
	Frame(int ID=0):mvMarkID(ID){ mvHdSz = 0, Alive = false; }
};