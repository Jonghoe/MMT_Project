#pragma once
#include"FrameController.h"
#include"EventHandler.h"
class Classification{
private:
	static Classification* Alive;
	FrameController* frameController;
	vector<Label*> info;
	bool mvStop;
	bool mvStopFirst;
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
	void mGetInfo(Label* _i){ info.push_back(_i); }
	void mGetInfo(const std::vector<Label*>& vc){
		for(size_t i=0;i<vc.size();++i)
			info.push_back(vc[i]);
	}
	void classify();
};