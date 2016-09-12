#pragma once
#include<Windows.h>
#include<sstream>
#include "opencv.hpp"
#include "DetectingModel.h"
#include "Label.h"
#include"Common.h"
using namespace std;
#define ULIMIT 1500						//��ũ ���� ���Ѽ�
#define DLIMIT 300						//��ũ ���� ���Ѽ�
cv::Mat RotationMat2(double d);
void Point2Mat(const cv::Point& p, cv::Mat& m);
void Mat2Point(const cv::Mat& m, cv::Point& p);
Point RotatePoint(double d, Point& p);
unsigned WINAPI FindMark(void* TMI);	// ���ø� ��Ī �Լ�
// �� �˻�
void FindLabel(const cv::Mat IMG, cv::Mat& PrevIMG, list<Label*>&VLabel,const size_t& LabelN,const Type scale);
// ���� Ȯ�� ( ULIMIT ~ DLIMIT )
bool DeleteCheck(cv::Mat&Label, vector<cv::Point>& con,const size_t PixelNum,const Type scale);
//�� ����
class EqualEvent{
	int id;
public:
	EqualEvent(const Event& e) :id(e.mGetID()){}
	bool operator()(Event& e){ return e.mGetID() == id; }
};
class BlobDup{
	Blob cmp;
public:
	BlobDup(const Blob& b) :cmp(b){}
	bool operator()(Label* it){
		return it->mContainPoint(cmp.mGetLT())||
				it->mContainPoint(cmp.mGetRB())||
				it->mContainPoint(Point(cmp.mGetLT().x,cmp.mGetRB().y))||
				it->mContainPoint(Point(cmp.mGetRB().x, cmp.mGetLT().x))||
				it->mContainPoint(cmp.mGetCenter());
	}
};
Label* MakeLabel(const cv::Mat& img, cv::Mat& prevImg, const cv::Point FL, const uchar val, const Type scale, MorT T);
void ModifyLabel(Label* l, const Blob& b);

// int ���� string ����
string IToS(const int info,const int len);
string LabeltoString(Label& obj);//�������� string���� ��ȯ
inline void SetRange(const cv::Point Loc,int Range[4]){	//  L U D R  ������ ����
	Loc.x < Range[0] ? Range[0] = Loc.x : (Range[3] < Loc.x ? Range[3] = Loc.x : Loc.x);
	Loc.y < Range[1] ? Range[1] = Loc.y : (Range[2] < Loc.y ? Range[2] = Loc.y : Loc.y);
}

inline void IMGSetting(cv::Mat& IMG){		//�̹��� ���� ��ó��
	cv::cvtColor(IMG, IMG, CV_BGR2GRAY);
	cv::threshold(IMG, IMG, 150, 255, CV_THRESH_BINARY);
}
	