#pragma once
#include "opencv.hpp"
#include "MarkList.h"
#include <list>
typedef unsigned ID;
#define DELETEV 10
enum Type{ LOW, MIDDLE, HIGH };
enum Act{MAKE,TRACKING};
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
	ID mvMarkID;

	cv::Point mvCenter;
	cv::Point mvLT;
	cv::Point mvRB;

	size_t mvPixelN;
	double mvTilt;
	bool mvIsMoved;
	cv::Mat mvPrevImg;
	double mvCalculateAngle(cv::Mat& IMG);
	void mvMoving(cv::Mat& IMG);

public:

	Label();
	Label(int* Range, int PixelNum, int id,  cv::Mat& previmg);
	Label(int* Ranges,int PixelNum, int Val);
	~Label();
	ID mGetID()const;
	ID mGetMarkID()const;
	cv::Point mGetCenter()const;
	int mSetMark(const MarkList& mvMarkList);
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
	static bool MakeLabel(const cv::Mat& img, cv::Mat& prevImg, Label** ML, const cv::Point FL, const uchar val, const Type scale, Act T);

};