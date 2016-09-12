#pragma once
#include "Common.h"

class Blob{
protected:
	ID mvID;										// 라벨 식별 값
	cv::Point mvCenter;								// 라벨의 중심 점
	cv::Point mvLT;									// 라벨 왼쪽 위
	cv::Point mvRB;									// 라벨 오른쪽 아래
	size_t mvPixelN;								// 라벨의 픽셀 수( 살릴지 죽일지 결정)
	cv::Mat mvPrevImg;
public:
	Blob();
	Blob(const Blob& b);
	Blob(int* Range, int PixelNum, int id,const cv::Mat& previmg);
	Blob(int* Ranges, int PixelNum, int Val);
	ID mGetID()const;									// 식별 값 반환
	cv::Point& mGetCenter();							// 중점 반환
	const cv::Point& mGetCenter()const;					// 중점 반환
	cv::Point& mGetLT();								// LT 반환
	const cv::Point& mGetLT()const;						// LT 반환
	cv::Point& mGetRB();								// RB 반환
	const cv::Point& mGetRB()const;						// RB 반환
	cv::Mat& mGetImg();
	void mChange(const Blob& b);
};