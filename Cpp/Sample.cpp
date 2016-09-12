#include"Common.h"
#include<iostream>
#include<opencv.hpp>
#include"ExtractBlob.h"
#include"Classification.h"
#include"LabelController.h"
#include"FrameController.h"
using namespace std;
using namespace cv;
int main(){
	VideoCapture vc("Sample.avi");
	LabelController LbC;
	FrameController* FrC = FrameController::mMakeController();
	ExtractBlob exB;
	Classification* Clf = Classification::mMakeClassify();
	Mat s;
	MarkList* Ml = MarkList::mMakeMarkList();

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
	return 0;
}