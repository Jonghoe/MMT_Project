#pragma once
#include "opencv.hpp"
#include "Motion.h"
#include "MarkList.h"
#include"Blob.h"
#include"Common.h"
#include <list>
#define DELETEV 10
class Label :public Blob{
	//라벨 정보를 저장
private:
	ID mvMarkID;								// 라벨의 마크 식별 값
	cv::Mat mvZone;								// 이미지에서 라벨의 구역
	double mvTilt;								// 라벨의 각도
	bool mvIsMoved;								// 움직였는지 확인
	void mvRotateImg();							// 이미지 회전
	double mvCalculateAngle(cv::Mat& IMG);		// 각도 계산
	void mvMoving(cv::Mat& IMG);				// 라벨의 Point 값들 조정
	void mvClearIMG(){							// 라벨의 구역을 삭제
		cv::Range row(mvLT.y, mvRB.y);
		cv::Range col(mvLT.x, mvRB.x);
		mvPrevImg(row, col) -= mvPrevImg(row, col);
	}
public:
	Label();
	Label(int* Range, int PixelNum, int id,  cv::Mat& previmg);
	Label(const Blob& b);
	Label(int* Ranges, int PixelNum, int Val);
	
	~Label();
	ID mGetMarkID()const;						// 마크 식별값 반환
	void mSetMark(const MarkList& mvMarkList);	// 마크 설정
	void mSetAngle(cv::Mat& IMG);				// 각도 설정
	bool mContainPoint(const cv::Point& Loc)const;//점을 포함하는지 확인
	Label operator=(const Label& copy);	         //라벨 대입 연산자
	bool mModify(cv::Mat& IMG);					// 라벨을 조정
	void mSetChanged(bool ch);
	bool mGetChanged();
};