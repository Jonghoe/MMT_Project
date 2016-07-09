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
	int const Width ;
	int const Height;
	static DebugViewer* Alive;
	void* mvConnected[2];
	cv::Mat* mvRealIMG;
	cv::Mat* mvChangedIMG;
	cv::Mat mvDbugWindow;
	DebugViewer():Width(800),Height(800){
		mvConnected[0] = NULL;
		mvConnected[1] = NULL;
		mvRealIMG = NULL;
		mvChangedIMG = NULL;
		mvDbugWindow.create(cv::Size(Width, Height),CV_8UC3 );
	}
public:
	void mSetRIMG(cv::Mat* IMG){ mvRealIMG = IMG;  }
	void mSetChangedIMG(cv::Mat* IMG){ mvChangedIMG = IMG;  }
	static DebugViewer* mMakeViewer(){
		if (Alive != NULL)	return Alive;
		Alive = new DebugViewer();
		return Alive;
	}
	void mConnect(MVC Type,void* Vp){ 
		if (Type == MVC::M){ 
			mvConnected[0] = Vp; 
		}
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
	void mDeleteViewer(){
		if (Alive == NULL)
			return;
		delete this;
	}
	void mShowViewer();
};
