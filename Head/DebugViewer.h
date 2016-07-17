#pragma once
#include<opencv.hpp>
#include<iostream>
#include"MVCPatter.h"
#include"DebugController.h"
// �ܼ��� ������� �ϱ� ���� ���� Ŭ����
// ����� ���� ��Ʈ�ѷ�  ��� < -- > ��Ʈ�ѷ� < -- > �� < -- > ���
// �̱۷��� ���
class DebugViewer{
private:
	int const Width ;							//ȭ���� �ʺ�
	int const Height;							//ȭ���� ����
	static DebugViewer* Alive;					//�̱۷����� ���� ����
	void* mvConnected[2];						//Contoller,Model ��ü ������
	cv::Mat* mvRealIMG;							//���� �̹���(Model��ü �̹���)
	cv::Mat* mvChangedIMG;						//��ȭ �̹���(Model��ü �̹���)
	cv::Mat mvDbugWindow;						//�⺻ Ʋ
	DebugViewer(int _w=800,int _h=800):Width(_w),Height(_h){//������ 
		mvConnected[0] = NULL;							
		mvConnected[1] = NULL;
		mvRealIMG = NULL;
		mvChangedIMG = NULL;
		mvDbugWindow.create(cv::Size(Width, Height),CV_8UC3 );
	}
public:
	void mSetRIMG(cv::Mat* IMG){ mvRealIMG = IMG;  }//�����̹��� ����
	void mSetChangedIMG(cv::Mat* IMG){ mvChangedIMG = IMG;  }//��ȭ�̹��� ����
	static DebugViewer* mMakeViewer(){			//�̱۷��� ���� �Լ�
		if (Alive != NULL)	return Alive;
		Alive = new DebugViewer();
		return Alive;
	}
	void mDeleteViewer(){						//�̱۷��� �ı� �Լ�
		if (Alive == NULL)
			return;
		delete this;
	}
	void mConnect(MVC Type,void* Vp){			//Model,Controller ���� �Լ�
		if (Type == MVC::M){ 
			mvConnected[0] = Vp; 
		}
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
	
	void mShowViewer();							//Viewer �̹����� ���
};
