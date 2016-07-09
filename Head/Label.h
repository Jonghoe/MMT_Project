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
	ID mGetID()const;
	ID mGetMarkID()const;
	cv::Point mGetCenter()const;
	void mSetMark();
	void mSet_Angle(cv::Mat& IMG);
	bool mContainPoint(const cv::Point Loc)const;
	Label Label::operator=(const Label& copy);
	bool mModify(cv::Mat& IMG);
	void mClearIMG(){
		for (int y = mvLT.y; y<mvPrev_Img.rows && y < mvRB.y; y++){
			uchar* ptrIMG = mvPrev_Img.ptr<uchar>(y);
			for (int x = mvLT.x; x < mvPrev_Img.cols && x < mvRB.x; x++)
				ptrIMG[x] = 0;
		}
	}
};