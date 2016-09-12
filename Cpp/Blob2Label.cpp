#include"Blob2Label.h"
#include"MyFunction.h"
#include<algorithm>
using namespace cv;
using namespace std;
Blob2Label::Blob2Label() :mvLbVc(){ mvML = MarkList::mMakeMarkList(); }

void Blob2Label::mvSetLabel(Label* l){
	l->mSetAngle(l->mGetImg());
	l->mSetMark(*mvML);
}
Label* Blob2Label::operator()(const Blob& b,vector<Label*> vc){
	Label* newLabel = new Label(b);
	mvSetLabel(newLabel);
	return newLabel;
}