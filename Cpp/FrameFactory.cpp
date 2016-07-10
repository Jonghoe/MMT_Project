#include"FrameFactory.h"
#include"FrameFillBlank.h"
FrameFactory* FrameFactory::Alive = NULL;
Observer* FrameFactory::mvCreate(Label* _label){
	Frame* newFrame;
	int MID=_label->mGetMarkID();
	int LID=_label->mGetID();
	switch (MID){
	case 1:	
		newFrame = new FillBlank(MID,LID); 
		break;
	default:
		break;
	}
	return (Observer*)newFrame;
}

