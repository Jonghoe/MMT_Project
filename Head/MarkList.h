#pragma once
#include "opencv.hpp"
#include <deque>
class MarkList{
private:
	static MarkList* Alive;
	size_t mvSize;								//����Ʈ�� ũ��	
	 std::deque<cv::Mat> mvList;				//��ũ ���� ����Ʈ
	 MarkList();								//������
public:
	static MarkList* mMakeMarkList();			//�̱۷��� ������
	void mDeleteMarkList();						//�̱۷��� �ı���
	void mAddMark( cv::Mat&);					//��ũ �߰�
	void mDeleteMark(const cv::Mat&);			//��ũ ����
	int mFindMark(const cv::Mat& src)const;		//��ũ �˻�
	size_t mGetSize(){ return mvSize; }
};
