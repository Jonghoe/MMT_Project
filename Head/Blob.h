#pragma once
#include "Common.h"

class Blob{
protected:
	ID mvID;										// �� �ĺ� ��
	cv::Point mvCenter;								// ���� �߽� ��
	cv::Point mvLT;									// �� ���� ��
	cv::Point mvRB;									// �� ������ �Ʒ�
	size_t mvPixelN;								// ���� �ȼ� ��( �츱�� ������ ����)
	cv::Mat mvPrevImg;
public:
	Blob();
	Blob(const Blob& b);
	Blob(int* Range, int PixelNum, int id,const cv::Mat& previmg);
	Blob(int* Ranges, int PixelNum, int Val);
	ID mGetID()const;									// �ĺ� �� ��ȯ
	cv::Point& mGetCenter();							// ���� ��ȯ
	const cv::Point& mGetCenter()const;					// ���� ��ȯ
	cv::Point& mGetLT();								// LT ��ȯ
	const cv::Point& mGetLT()const;						// LT ��ȯ
	cv::Point& mGetRB();								// RB ��ȯ
	const cv::Point& mGetRB()const;						// RB ��ȯ
	cv::Mat& mGetImg();
	void mChange(const Blob& b);
};