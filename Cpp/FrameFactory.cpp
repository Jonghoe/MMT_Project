#include"FrameFactory.h"
#include"FrFillBlank.h"
Frame* FrameFactory::operator()(Label* _label,ID id){
	Frame* newFrame = NULL;
	ID MID = _label->mGetMarkID();
	ID LID = _label->mGetID();
	//switch (MID){
	//case 1:
		newFrame = new FrFillBlank(_label,id,800,600);
		
		//break;
	//default:
//		break;
//	}
	
	return newFrame;
}