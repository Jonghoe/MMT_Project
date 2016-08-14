#pragma once
#include"FrameController.h"
#include"EventHandler.h"
class Classification{
private:
	
	FrameController* frameController;
	EventHandler* eventHandler;
	vector<Label*> info;
	
	static bool IsMark(const Label* obj){
		if (obj->mGetMarkID() == 0)
			return false;
		return true;
	}
	
public:
	Classification(){
		frameController = FrameController::mMakeSubject();
		eventHandler = EventHandler::mMakeEventHandler();
	}
	
	void mGetInfo(Label* _i){
		info.push_back(_i);
	}

	void classify(){
		vector<Label*>::iterator it;
		for (it = info.begin(); it != info.end(); ++it)
			if (IsMark(*it))
				frameController->mPush(*it);
			else
				eventHandler->mPush(*it);
	}
	
};