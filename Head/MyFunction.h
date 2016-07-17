#pragma once
#include "opencv.hpp"
#include "DetectingModel.h"
#include "Label.h"
#include<Windows.h>
#include<sstream>
using namespace std;
#define ULIMIT 1500						//��ũ ���� ���Ѽ�
#define DLIMIT 300						//��ũ ���� ���Ѽ�
struct TRMatchingInform{				//���ø� ��Ī ����ü
	cv::Mat CompareIMG;
	cv::Mat MarkIMG;
	int Index;
	double MaxVal;
};
unsigned WINAPI FindMark(void* TMI);	// ���ø� ��Ī �Լ�
// �� �˻�
void FindLabel(const cv::Mat IMG, cv::Mat& PrevIMG, list<Label*>&VLabel,const size_t& LabelN,const Type scale);
// ���� Ȯ�� ( ULIMIT ~ DLIMIT )
bool DeleteCheck(cv::Mat&Label, vector<cv::Point>& con,const size_t PixelNum,const Type scale);
// int ���� string ����
string IToS(const int info,const int len);
string LabeltoString(Label& obj);//�������� string���� ��ȯ
inline void SetRange(const cv::Point Loc,int Range[4]){	//  L U D R  ������ ����
	Loc.x < Range[0] ? Range[0] = Loc.x : (Range[3] < Loc.x ? Range[3] = Loc.x : Loc.x);
	Loc.y < Range[1] ? Range[1] = Loc.y : (Range[2] < Loc.y ? Range[2] = Loc.y : Loc.y);
}

inline void IMGSetting(cv::Mat& IMG){		//�̹��� ���� ��ó��
	cv::cvtColor(IMG, IMG, CV_BGR2GRAY);
	cv::threshold(IMG, IMG, 240, 255, CV_THRESH_BINARY);
}
	