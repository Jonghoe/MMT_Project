#pragma once
#include "opencv.hpp"
#include <deque>
class MarkList{
private:
	size_t mvSize;							//����Ʈ�� ũ��	
	 std::deque<cv::Mat> mvList;			//��ũ ���� ����Ʈ
public:
	MarkList();								//������
	void mAddMark( cv::Mat&);				//��ũ �߰�
	void mDeleteMark(const cv::Mat&);		//��ũ ����
	int mFindMark(const cv::Mat& src)const;	//��ũ �˻�
	size_t mGetSize(){ return mvSize; }
};
