#pragma once
#include"Blob.h"
class ExtractBlob{
	cv::Mat mvTmp;
	const int mvDLimit;
	int mvRange[4];
	int mvPixelNum;
	int mvBlobNum;
	void mvSetRange(const cv::Point& Loc);	//  L U D R  순으로 정리
	void mvInitRange();
	void mvBlobing(int row, int col, const cv::Mat& img);
public:
	ExtractBlob() :mvDLimit(100){ mvBlobNum = 200; }
	std::vector<Blob>operator()(cv::Mat& img);
};