#pragma once
#include "opencv.hpp"
#include "Motion.h"
#include "MarkList.h"
#include"Blob.h"
#include"Common.h"
#include <list>
#define DELETEV 10
class Label :public Blob{
	//�� ������ ����
private:
	ID mvMarkID;								// ���� ��ũ �ĺ� ��
	cv::Mat mvZone;								// �̹������� ���� ����
	double mvTilt;								// ���� ����
	bool mvIsMoved;								// ���������� Ȯ��
	void mvRotateImg();							// �̹��� ȸ��
	double mvCalculateAngle(cv::Mat& IMG);		// ���� ���
	void mvMoving(cv::Mat& IMG);				// ���� Point ���� ����
	void mvClearIMG(){							// ���� ������ ����
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
	ID mGetMarkID()const;						// ��ũ �ĺ��� ��ȯ
	void mSetMark(const MarkList& mvMarkList);	// ��ũ ����
	void mSetAngle(cv::Mat& IMG);				// ���� ����
	bool mContainPoint(const cv::Point& Loc)const;//���� �����ϴ��� Ȯ��
	Label operator=(const Label& copy);	         //�� ���� ������
	bool mModify(cv::Mat& IMG);					// ���� ����
	void mSetChanged(bool ch);
	bool mGetChanged();
};