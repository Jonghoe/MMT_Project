#pragma once
#include "opencv.hpp"
#include "Detecting_Model.h"
#include "Label.h"
#include<Windows.h>
using namespace std;
#define U_LIMIT 1500
#define D_LIMIT 150
struct TR_Matching_Inform{
	cv::Mat CompareIMG;
	cv::Mat MarkIMG;
	int Index;
	double MaxVal;
};
unsigned WINAPI Label_Main(const void* Label);
unsigned WINAPI Find_Mark(void* TMI);
void Calculate_Angle_Main(const cv::Mat& src, Label&label);
void Find_Label(const cv::Mat IMG, cv::Mat& Prev_IMG, list<Label*>&V_Label,const size_t& Label_N,const Type scale);
bool Make_Label(Detecting_Model* Model, Label** ML,const cv::Point F_L,const Type scale);
bool Make_Label(const cv::Mat& IMG, cv::Mat& Prev_IMG, Label** ML, const cv::Point F_L, const uchar Val, const Type scale);
bool Delete_Check(cv::Mat&Label, vector<cv::Point>& con,const size_t Pixel_Num,const Type scale);

string I_To_S(const int info,const int len);
string Label_to_String(Label& obj);

inline void SetRange(const cv::Point Loc,int Range[4]){
	Loc.x < Range[0] ? Range[0] = Loc.x : (Range[3] < Loc.x ? Range[3] = Loc.x : Loc.x);
	Loc.y < Range[1] ? Range[1] = Loc.y : (Range[2] < Loc.y ? Range[2] = Loc.y : Loc.y);
}

inline void IMG_Setting(cv::Mat& IMG){
	cv::cvtColor(IMG, IMG, CV_BGR2GRAY);
	cv::threshold(IMG, IMG, 240, 255, CV_THRESH_BINARY);
}
