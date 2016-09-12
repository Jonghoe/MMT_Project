#include"Frame.h"
unsigned WINAPI Frame::mainPerform(void* fr){
	Frame* info = (Frame*)fr;
	EventHandler* EH = info->mGetEventHandler();
	while (!info->mIsEnd()){
		switch (info->mGetState()){
		case START:
			info->mvStart();
			info->mPlay();
			break;
		case STOP:
			info->mvStop();
			break;
		case PLAY:
			info->mvPlay();
			break;
		case UPDATE:
			info->mvUpdate();
			break;
		default:
			break;
		}
	}
	return 0;
}
