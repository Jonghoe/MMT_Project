#include"DebugViewer.h"
DebugViewer * DebugViewer::Alive = NULL;
void DebugViewer::mShowViewer(){
	cv::Mat A, B, IMGROI;
	resize(*mvRealIMG, A, cv::Size(Width, Height / 2));
	resize(*mvChangedIMG, B, cv::Size(Width, Height / 2));

	cv::imshow("Real", *mvRealIMG);
	cv::imshow("Changed", *mvChangedIMG);
	
	((DebugController*)mvConnected[1])->mPushKey(cv::waitKey(10));

}