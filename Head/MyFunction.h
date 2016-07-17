#pragma once
#include "opencv.hpp"
#include "DetectingModel.h"
#include "Label.h"
#include<Windows.h>
#include<sstream>
using namespace std;
#define ULIMIT 1500						//마크 삭제 상한선
#define DLIMIT 300						//마크 삭제 하한선
struct TRMatchingInform{				//템플릿 매칭 구조체
	cv::Mat CompareIMG;
	cv::Mat MarkIMG;
	int Index;
	double MaxVal;
};
unsigned WINAPI FindMark(void* TMI);	// 템플릿 매칭 함수
// 라벨 검색
void FindLabel(const cv::Mat IMG, cv::Mat& PrevIMG, list<Label*>&VLabel,const size_t& LabelN,const Type scale);
// 삭제 확인 ( ULIMIT ~ DLIMIT )
bool DeleteCheck(cv::Mat&Label, vector<cv::Point>& con,const size_t PixelNum,const Type scale);
// int 형을 string 으로
string IToS(const int info,const int len);
string LabeltoString(Label& obj);//라벨정보를 string으로 변환
inline void SetRange(const cv::Point Loc,int Range[4]){	//  L U D R  순으로 정리
	Loc.x < Range[0] ? Range[0] = Loc.x : (Range[3] < Loc.x ? Range[3] = Loc.x : Loc.x);
	Loc.y < Range[1] ? Range[1] = Loc.y : (Range[2] < Loc.y ? Range[2] = Loc.y : Loc.y);
}

inline void IMGSetting(cv::Mat& IMG){		//이미지 영상 전처리
	cv::cvtColor(IMG, IMG, CV_BGR2GRAY);
	cv::threshold(IMG, IMG, 240, 255, CV_THRESH_BINARY);
}
	