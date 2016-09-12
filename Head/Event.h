#pragma once
#include"opencv.hpp"
#include"Label.h"
class Event{
	cv::Point mvLoc;
	ID mvID;
	double mvTilt;
public:
	Event(const cv::Point& l, ID id) :mvLoc(l), mvID(id), mvTilt(){}
	Event() :mvLoc(), mvTilt(),mvID(){}
	static Event LtoE(const Label& l){	return Event(l.mGetCenter(), l.mGetID());}
	const cv::Point& mGetLoc()const{ return mvLoc; }
	cv::Point& mGetLoc(){ return mvLoc; }
	const ID mGetID()const{ return mvID; }
	double mGetTilt(){ return mvTilt; }
	void mSetTilt(cv::Point& c, Event& e);
};