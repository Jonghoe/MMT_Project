#pragma once
#include "opencv.hpp"
#include <deque>
class MarkList{
private:
	static MarkList* Alive;
	size_t mvSize;								//리스트의 크기	
	 std::deque<cv::Mat> mvList;				//마크 저장 리스트
	 MarkList();								//생성자
public:
	static MarkList* mMakeMarkList();			//싱글레톤 생성자
	void mDeleteMarkList();						//싱글레톤 파괴자
	void mAddMark( cv::Mat&);					//마크 추가
	void mDeleteMark(const cv::Mat&);			//마크 삭제
	int mFindMark(const cv::Mat& src)const;		//마크 검색
	size_t mGetSize(){ return mvSize; }
};
