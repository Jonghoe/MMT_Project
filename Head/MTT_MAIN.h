#include"DebugViewer.h"
#include"LabelController.h"
#include"FrameController.h"
#include"Classification.h"
#include"ExtractBlob.h"
using namespace std;
using namespace cv;
#define _MAIN_  int main(){ MAIN m;m.Do();}
class MAIN{
private:
	VideoCapture vc;
	LabelController LbC;
	FrameController* FrC = FrameController::mMakeController();
	ExtractBlob exB;
	Classification* Clf = Classification::mMakeClassify();
	Mat s;
	MarkList* Ml = MarkList::mMakeMarkList();

public:
	MAIN() :vc("Sample.avi"), LbC(), exB(){
		FrC = FrameController::mMakeController();
		Clf = Classification::mMakeClassify();
		Ml = MarkList::mMakeMarkList();
	}


	void Do(){
		Ml->mAddMark(imread("Event.jpg"));
		Ml->mAddMark(imread("Mark_1.jpg"));
		Ml->mAddMark(imread("Mark_2.jpg"));
		while (waitKey(10) != 27){
			vc >> s;
			imshow("A", s);
			LbC.mModify(exB(s));
			Clf->mGetInfo(LbC.mGetVc());
			Clf->classify();
			FrC->mControll();
		}
	}
};

