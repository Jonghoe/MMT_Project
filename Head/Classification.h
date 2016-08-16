#pragma once
#include"FrameController.h"
#include"EventHandler.h"
class Classification{
private:
	static Classification* Alive;
	FrameController* frameController;
	vector<Label*> info;
	
	static bool IsMark(const Label* obj){
		if (obj->mGetMarkID() == 0)
			return false;
		return true;
	}
	Classification():info(){
		frameController = FrameController::mMakeController();
	}
public:
	static Classification* mMakeClassify(){
		if (Alive == nullptr)
			Alive = new Classification();
		return Alive;
	}
	static void mDeleteClassify(){
		if (Alive == nullptr)
			return;
		delete Alive;
	}
	void mGetInfo(Label* _i){	info.push_back(_i);	}

	void classify(){
		vector<Label*>::iterator it;
		for (it = info.begin(); it != info.end(); ++it)
			if (IsMark(*it))
				frameController->mPushMark(*it);
			else
				frameController->mPushEvent(*it);
		info.resize(0);
	}
};