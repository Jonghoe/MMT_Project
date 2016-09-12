#include"EventHandler.h"
#include<algorithm>
#include"MyFunction.h"
void EventHandler::mvIsRelate(){
	for (size_t i = 0; i < mvEVcT.size(); ++i){
		auto it = find_if(mvEVc.begin(), mvEVc.end(), EqualEvent(mvEVcT[i]));
		if (it != mvEVcT.end())
			mvEVcT[i].mSetTilt(*mvFrC, *it);
	}
	mvEVc.resize(0);
	copy(mvEVcT.begin(), mvEVcT.end(), mvEVc.begin());
	mvEVcT.resize(0);
}

vector<Motion> EventHandler::mPutMotion(){
	mvIsRelate();
	return	mvCreater(mvMChecker, mvEVc);
}