#pragma once
#include "opencv.hpp"
#include "MarkList.h"
#include <list>
typedef unsigned ID;
#define DELETE_V 10
enum Type{ LOW, MIDDLE, HIGH };

class Label {
//		�󺧸� �� ��� ��, ��ü���� ��Ÿ��
//
//				------------- �����Լ� -------------
//		bool mContainPoint(cv::Point Loc) : Loc�� �󺧿� ���Ե��ִ��� Ȯ��
//		bool mDeleteLabel()			:	Label�� ����
//		void Label::mMoving()		:	���� ������ Ȯ��
//		inline void mClearIMG()		:	���� �̹��� ����
private:
	ID mvID;
	cv::Point mvCenter;
	cv::Point mvLT;
	cv::Point mvRB;
	size_t mvPixel_N;
	double mvTilt;
	bool mvIsMoved;
	MarkList* mvMarkList;
	ID mvMarkId;
	cv::Mat mvPrev_Img;
	double mvCalculate_Angle(cv::Mat& IMG);
	void mvMoving(cv::Mat& IMG);
public:
	Label();
	Label(int* Range, int Pixel_Num, int id, MarkList* _MarkList, cv::Mat& Img, cv::Mat& prev_img);
	Label(int* Ranges,int Pixel_Num, int Val);
	~Label();
	ID mGetID();
	ID mGetMarkID();
	cv::Point mGetCenter();
	void mSetMark();
	void mSet_Angle(cv::Mat& IMG);
	bool mContainPoint(const cv::Point Loc);
	Label Label::operator=(const Label& copy);
	bool mModify(cv::Mat& IMG);
	void mClearIMG(){
		for (size_t y = (size_t)mvLT.y; y<(size_t)mvPrev_Img.rows && y < (size_t)mvRB.y; y++){
			uchar* ptrIMG = mvPrev_Img.ptr<uchar>(y);
			for (size_t x = (size_t)mvLT.x; x < (size_t)mvPrev_Img.cols && x < (size_t)mvRB.x; x++)
				ptrIMG[x] = 0;
		}
	}
};