#pragma once
#include "opencv.hpp"
#include <deque>
class MarkList{
private:
	size_t mvSize;
	 std::deque<cv::Mat> mvList;
public:
	MarkList();
	void mAddMark( cv::Mat&);
	void mDeleteMark(const cv::Mat&);
	int mFindMark(const cv::Mat& src);
};
