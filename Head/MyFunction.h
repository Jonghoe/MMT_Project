#pragma once
#include "opencv.hpp"
#include "DetectingModel.h"
#include "Label.h"
#include<Windows.h>
using namespace std;
#define ULIMIT 1500
#define DLIMIT 150
struct TRMatchingInform{
	cv::Mat CompareIMG;
	cv::Mat MarkIMG;
	int Index;
	double MaxVal;
};
unsigned WINAPI LabelMain(const void* Label);
unsigned WINAPI FindMark(void* TMI);
void CalculateAngleMain(const cv::Mat& src, Label&label);
void FindLabel(const cv::Mat IMG, cv::Mat& PrevIMG, list<Label*>&VLabel,const size_t& LabelN,const Type scale);
bool MakeLabel(DetectingModel* Model, Label** ML,const cv::Point FL,const Type scale);
//bool MakeLabel(const cv::Mat& IMG, cv::Mat& PrevIMG, Label** ML, const cv::Point FL, const uchar Val, const Type scale);
bool DeleteCheck(cv::Mat&Label, vector<cv::Point>& con,const size_t PixelNum,const Type scale);

string IToS(const int info,const int len);
string LabeltoString(Label& obj);

inline void SetRange(const cv::Point Loc,int Range[4]){
	Loc.x < Range[0] ? Range[0] = Loc.x : (Range[3] < Loc.x ? Range[3] = Loc.x : Loc.x);
	Loc.y < Range[1] ? Range[1] = Loc.y : (Range[2] < Loc.y ? Range[2] = Loc.y : Loc.y);
}

inline void IMGSetting(cv::Mat& IMG){
	cv::cvtColor(IMG, IMG, CV_BGR2GRAY);
	cv::threshold(IMG, IMG, 240, 255, CV_THRESH_BINARY);
}
