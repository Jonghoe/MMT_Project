#pragma once
#include "opencv.hpp"
#include "MarkList.h"
#include <list>
typedef unsigned ID;
#define DELETEV 10
enum Type{ LOW, MIDDLE, HIGH };
enum Act{MAKE,TRACKING};
class Label {
	//라벨 정보를 저장
private:
	ID mvID;					// 라벨 식별 값
	ID mvMarkID;				// 라벨의 마크 식별 값
	
	cv::Point mvCenter;			// 라벨의 중심 점
	cv::Point mvLT;				// 라벨 왼쪽 위
	cv::Point mvRB;				// 라벨 오른쪽 아래
	cv::Mat mvZone;				// 이미지에서 라벨의 구역
	size_t mvPixelN;			// 라벨의 픽셀 수( 살릴지 죽일지 결정)
	double mvTilt;				// 라벨의 각도
	bool mvIsMoved;				// 움직였는지 확인
	cv::Mat mvPrevImg;			// 이전 이미지
	void mvRotateImg();			// 이미지 회전
	double mvCalculateAngle(cv::Mat& IMG);		// 각도 계산
	void mvMoving(cv::Mat& IMG);				// 라벨의 Point 값들 조정
public:
	Label();
	Label(int* Range, int PixelNum, int id,  cv::Mat& previmg);
	Label(int* Ranges,int PixelNum, int Val);
	~Label();
	ID mGetID()const;							// 식별 값 반환
	ID mGetMarkID()const;						// 마크 식별값 반환
	cv::Point mGetCenter()const;				// 센터 점 반환
	void mSetMark(const MarkList& mvMarkList);	// 마크 설정
	void mSetAngle(cv::Mat& IMG);				// 각도 설정
	bool mContainPoint(const cv::Point Loc)const;//점을 포함하는지 확인
	Label Label::operator=(const Label& copy);	//라벨 대입 연산자
	bool mModify(cv::Mat& IMG);					// 라벨을 조정
	void mClearIMG(){							// 라벨의 구역을 삭제
		for (int y = mvLT.y; y<mvPrevImg.rows && y < mvRB.y; y++){
			uchar* ptrIMG = mvPrevImg.ptr<uchar>(y);
			for (int x = mvLT.x; x < mvPrevImg.cols && x < mvRB.x; x++)
				ptrIMG[x] = 0;
		}
	}
	//라벨 생성
	static bool MakeLabel(const cv::Mat& img, cv::Mat& prevImg, Label** ML, const cv::Point FL, const uchar val, const Type scale, Act T);


};