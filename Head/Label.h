#pragma once
#include "opencv.hpp"
#include "MarkList.h"
#include <list>
typedef unsigned ID;
#define DELETEV 10
enum Type{ LOW, MIDDLE, HIGH };
enum Act{MAKE,TRACKING};
class Label {
	//�� ������ ����
private:
	ID mvID;					// �� �ĺ� ��
	ID mvMarkID;				// ���� ��ũ �ĺ� ��
	
	cv::Point mvCenter;			// ���� �߽� ��
	cv::Point mvLT;				// �� ���� ��
	cv::Point mvRB;				// �� ������ �Ʒ�
	cv::Mat mvZone;				// �̹������� ���� ����
	size_t mvPixelN;			// ���� �ȼ� ��( �츱�� ������ ����)
	double mvTilt;				// ���� ����
	bool mvIsMoved;				// ���������� Ȯ��
	cv::Mat mvPrevImg;			// ���� �̹���
	void mvRotateImg();			// �̹��� ȸ��
	double mvCalculateAngle(cv::Mat& IMG);		// ���� ���
	void mvMoving(cv::Mat& IMG);				// ���� Point ���� ����
public:
	Label();
	Label(int* Range, int PixelNum, int id,  cv::Mat& previmg);
	Label(int* Ranges,int PixelNum, int Val);
	~Label();
	ID mGetID()const;							// �ĺ� �� ��ȯ
	ID mGetMarkID()const;						// ��ũ �ĺ��� ��ȯ
	cv::Point mGetCenter()const;				// ���� �� ��ȯ
	void mSetMark(const MarkList& mvMarkList);	// ��ũ ����
	void mSetAngle(cv::Mat& IMG);				// ���� ����
	bool mContainPoint(const cv::Point Loc)const;//���� �����ϴ��� Ȯ��
	Label Label::operator=(const Label& copy);	//�� ���� ������
	bool mModify(cv::Mat& IMG);					// ���� ����
	void mClearIMG(){							// ���� ������ ����
		for (int y = mvLT.y; y<mvPrevImg.rows && y < mvRB.y; y++){
			uchar* ptrIMG = mvPrevImg.ptr<uchar>(y);
			for (int x = mvLT.x; x < mvPrevImg.cols && x < mvRB.x; x++)
				ptrIMG[x] = 0;
		}
	}
	//�� ����
	static bool MakeLabel(const cv::Mat& img, cv::Mat& prevImg, Label** ML, const cv::Point FL, const uchar val, const Type scale, Act T);


};