#pragma once
#include<opencv.hpp>
#include<iostream>
#include"MVC_Patter.h"
#include"Debug_Controller.h"
// 디버깅 전용 컨트롤러  뷰어 < -- > 컨트롤러 < -- > 모델 < -- > 뷰어
// 싱글레톤 사용
class Debug_Viewer{
private:
	int const Width ;
	int const Height;
	static Debug_Viewer* Alive;
	void* mvConnected[2];
	cv::Mat* mvReal_IMG;
	cv::Mat* mvChanged_IMG;
	cv::Mat mvDbug_Window;
	Debug_Viewer():Width(800),Height(800){
		mvConnected[0] = NULL;
		mvConnected[1] = NULL;
		mvReal_IMG = NULL;
		mvChanged_IMG = NULL;
		mvDbug_Window.create(cv::Size(Width, Height),CV_8UC3 );
	}
public:
	void mSet_R_IMG(cv::Mat* IMG){ mvReal_IMG = IMG;  }
	void mSet_Changed_IMG(cv::Mat* IMG){ mvChanged_IMG = IMG;  }
	static Debug_Viewer* mMake_Viewer(){
		if (Alive != NULL)	return Alive;
		Alive = new Debug_Viewer();
		return Alive;
	}
	void mConnect(MVC Type,void* Vp){ 
		if (Type == MVC::M){ 
			mvConnected[0] = Vp; 
		}
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
	void mDelete_Viewer(){
		if (Alive == NULL)
			return;
		delete this;
	}
	void mShow_Viewer(){
		cv::Mat A, B,IMG_ROI;
		resize(*mvReal_IMG, A, cv::Size(Width, Height/ 2));
		resize(*mvChanged_IMG, B, cv::Size(Width, Height/ 2));
		
		cv::imshow("Real", *mvReal_IMG);
		cv::imshow("Changed", *mvChanged_IMG);
		/*IMG_ROI = mvDbug_Window(cv::Rect(0, Height/2, Width, Height -1));
		
		cv::imshow("mvDbug_Window", mvDbug_Window);
		mvDbug_Window.adjustROI(0, Height / 2, 0, Width) = A;
		mvDbug_Window.adjustROI(Height / 2, Height, 0, Width) = B;
			cv::imshow("mvDbug_Window", mvDbug_Window);
		*/
		((Debug_Controller*)mvConnected[1])->mPush_Key(cv::waitKey(10));

	}
};
