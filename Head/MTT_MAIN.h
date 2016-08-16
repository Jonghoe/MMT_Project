#include"DetectingModel.h"
#include"DebugViewer.h"
using namespace std;
using namespace cv;
#define DEBUG_LV2
#define DEBUG_LV1
#define _MAIN_  int main(){ MAIN m;m.Do();}
class MAIN{
public:
	MAIN(){}
	void Do(){
		DetectingModel* DM = DetectingModel::mMakeModel();
		MarkList* mark = MarkList::mMakeMarkList();
		Classification* Cl = Classification::mMakeClassify();
		FrameController* Fc = FrameController::mMakeController();
#ifdef DEBUG_LV1
		DebugViewer* DV = DebugViewer::mMakeViewer();
		DebugController* DC = DebugController::mMakeController();
		DV->mConnect(MVC::C, DC);
		DV->mConnect(MVC::M, DM);
		
		DM->mConnect(MVC::C, DC);
		DM->mConnect(MVC::V, DV);
		
		DC->mConnect(MVC::M, DM);
		DC->mConnect(MVC::V, DV);
#endif
//		시작 전 기본 데이터 구축(마크 모양)----------------------------
		vector<Mat> MarkVc;
		MarkVc.push_back(cv::imread("Event.jpg", cv::IMREAD_COLOR));
		MarkVc.push_back(cv::imread("Mark_1.jpg", cv::IMREAD_COLOR));
		MarkVc.push_back(cv::imread("Mark_2.jpg", cv::IMREAD_COLOR));
			
		for (size_t i = 0; i < MarkVc.size();++i)
			mark->mAddMark(MarkVc[i]);

//		-------------------------------------------------------------
		while (DM->mAction()){
#ifdef DEBUG_LV1
			DV->mShowViewer();
#else if
			
#endif
			Cl->classify();
			Fc->mControll();
		}
	}
};