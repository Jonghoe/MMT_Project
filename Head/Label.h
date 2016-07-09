#pragma once
#include "opencv.hpp"
#include "MarkList.h"
#include <list>
typedef unsigned ID;
#define DELETEV 10
enum Type{ LOW, MIDDLE, HIGH };

class Label {
//		라벨링 된 덩어리 즉, 물체들을 나타냄
//
//				------------- 내장함수 -------------
//		bool mContainPoint(cv::Point Loc) : Loc가 라벨에 포함되있는지 확인
//		bool mDeleteLabel()			:	Label을 삭제
//		void Label::mMoving()		:	라벨의 움직임 확인
//		inline void mClearIMG()		:	라벨의 이미지 지움
private:
	ID mvID;
	cv::Point mvCenter;
	cv::Point mvLT;
	cv::Point mvRB;
	size_t mvPixelN;
	double mvTilt;
	bool mvIsMoved;
	MarkList* mvMarkList;
	ID mvMarkId;
	cv::Mat mvPrevImg;
	double mvCalculateAngle(cv::Mat& IMG);
	void mvMoving(cv::Mat& IMG);
public:
	Label();
	Label(int* Range, int PixelNum, int id, MarkList* MarkList, cv::Mat& Img, cv::Mat& previmg);
	Label(int* Ranges,int PixelNum, int Val);
	~Label();
	ID mGetID()const;
	ID mGetMarkID()const;
	cv::Point mGetCenter()const;
	void mSetMark();
	void mSetAngle(cv::Mat& IMG);
	bool mContainPoint(const cv::Point Loc)const;
	Label Label::operator=(const Label& copy);
	bool mModify(cv::Mat& IMG);
	void mClearIMG(){
		for (int y = mvLT.y; y<mvPrevImg.rows && y < mvRB.y; y++){
			uchar* ptrIMG = mvPrevImg.ptr<uchar>(y);
			for (int x = mvLT.x; x < mvPrevImg.cols && x < mvRB.x; x++)
				ptrIMG[x] = 0;
		}
	}
};