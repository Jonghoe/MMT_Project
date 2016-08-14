#include"FrameFactory.h"
#include"FrameFillBlank.h"
FrameFactory* FrameFactory::Alive = NULL;
Frame* FrameFactory::mvCreate(Label* _label,ID id){
	Frame* newFrame = NULL;
	ID MID = _label->mGetMarkID();
	ID LID = _label->mGetID();
	switch (MID){
	case 1:
		newFrame = new FillBlank(MID, LID,id);
		break;
	default:
		break;
	}
	
	return newFrame;
}