#include"FrameFactory.h"
#include"FrameFillBlank.h"
FrameFactory* FrameFactory::Alive = NULL;
Frame* FrameFactory::mvCreate(Label* _label){
	Frame* newFrame = NULL;
	int MID = _label->mGetMarkID();
	int LID = _label->mGetID();
	switch (MID){
	case 1:
		newFrame = new FillBlank(MID, LID);
		break;
	default:
		break;
	}
	return newFrame;
}