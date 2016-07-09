#include"FrameFactory.h"
#include"FrameFillBlank.h"
FrameFactory* FrameFactory::Alive = NULL;
Observer* FrameFactory::mvCreate(Label* _label){
	Frame* newFrame;
	switch (_label->mGetMarkID()){
	case 1:	
		newFrame = new FillBlank(); 
		break;
	default:
		break;
	}
	return (Observer*)newFrame;
}

