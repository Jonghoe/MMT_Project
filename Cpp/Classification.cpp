#include"Classification.h"
Classification* Classification::Alive = nullptr;
void Classification::classify(){
	vector<Label*>::iterator it;
	for (it = info.begin(); it != info.end(); ++it)
		if (IsMark(*it))
			frameController->mPushMark(*it);
		else
			frameController->mPushEvent(*it);
	info.resize(0);
}