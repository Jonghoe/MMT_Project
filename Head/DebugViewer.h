#pragma once
#include<opencv.hpp>
#include<iostream>
#include"MVCPatter.h"
#include"DebugController.h"
// 단순히 디버깅을 하기 위해 만든 클래스
// 디버깅 전용 컨트롤러  뷰어 < -- > 컨트롤러 < -- > 모델 < -- > 뷰어
// 싱글레톤 사용
class DebugViewer{
private:
	int const Width ;							//화면의 너비
	int const Height;							//화면의 높이
	static DebugViewer* Alive;					//싱글레톤을 위한 변수
	void* mvConnected[2];						//Contoller,Model 객체 포인터
	cv::Mat* mvRealIMG;							//실제 이미지(Model객체 이미지)
	cv::Mat* mvChangedIMG;						//변화 이미지(Model객체 이미지)
	cv::Mat mvDbugWindow;						//기본 틀
	DebugViewer(int _w=800,int _h=800):Width(_w),Height(_h){//생성자 
		mvConnected[0] = NULL;							
		mvConnected[1] = NULL;
		mvRealIMG = NULL;
		mvChangedIMG = NULL;
		mvDbugWindow.create(cv::Size(Width, Height),CV_8UC3 );
	}
public:
	void mSetRIMG(cv::Mat* IMG){ mvRealIMG = IMG;  }//실제이미지 설정
	void mSetChangedIMG(cv::Mat* IMG){ mvChangedIMG = IMG;  }//변화이미지 설정
	static DebugViewer* mMakeViewer(){			//싱글레톤 생성 함수
		if (Alive != NULL)	return Alive;
		Alive = new DebugViewer();
		return Alive;
	}
	void mDeleteViewer(){						//싱글레톤 파괴 함수
		if (Alive == NULL)
			return;
		delete this;
	}
	void mConnect(MVC Type,void* Vp){			//Model,Controller 연결 함수
		if (Type == MVC::M){ 
			mvConnected[0] = Vp; 
		}
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
	
	void mShowViewer();							//Viewer 이미지를 출려
};
