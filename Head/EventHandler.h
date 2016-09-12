#pragma once
#include<vector>
#include"MotionCreater.h"
#include"Circle.h"
#include"Event.h"

using namespace std;

class EventHandler{
private:
	bool mvUpdate;
	Circle mvMChecker;
	vector<Event> mvEVc;
	vector<Event> mvEVcT;
	vector<Motion> mvMVc;
	MotionCreater mvCreater;
	cv::Point* mvFrC;
	void mvIsRelate();
public:
	EventHandler(cv::Point* p ) :mvEVc(), mvEVcT(), mvCreater(), mvMChecker(),mvFrC(p){ mvUpdate = false; }
	bool mGetUpdate(){ return mvUpdate; }
	void mPush(Event& e){
		if (mvUpdate ==false)
			mvUpdate = true;
		mvEVcT.push_back(e);
	}
	vector<Motion> mPutMotion();
};