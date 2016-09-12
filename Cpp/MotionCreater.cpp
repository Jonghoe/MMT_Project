#include"MotionCreater.h"
#include"MyFunction.h"
Circle MotionCreater::mvMakeChecker(vector<Event>& v){
	Circle nextC;
	int diffX, diffY;
	switch (v.size()){
	case 1:
		 nextC.mSetCenter(v[0].mGetLoc());
		 nextC.mSetR(0.5);
		break;
	case 2:
		diffX = (v[0].mGetLoc() - v[1].mGetLoc()).x;
		diffY = (v[0].mGetLoc() - v[1].mGetLoc()).y;
		nextC.mSetCenter((v[0].mGetLoc() + v[1].mGetLoc()) / 2);
		nextC.mSetR(sqrt(pow(diffX, 2) + pow(diffY, 2))/2);
		break;
	case 3:
		break;
	default:
		break;
	}
	v.resize(0);
	return nextC;
}

vector<Motion> MotionCreater::operator()(Circle& c,vector<Event>& vc){
	Circle nextC = mvMakeChecker(vc);
	vector<Motion> motionVc;
	double tilt = 0;
	for (size_t i = 0; i < vc.size(); ++i)
		tilt += vc[i].mGetTilt();
	tilt /= vc.size();
	bool IsMoved = c.mGetCenter() != nextC.mGetCenter();
	bool IsExOrRe = c.mGetR() != nextC.mGetR();
	bool IsRotate = tilt != 0.0;
	if (nextC.mGetR() != -1)
		return motionVc;
	if (IsMoved)
		motionVc.push_back(Move(nextC.mGetCenter()-c.mGetCenter()));
	if (IsExOrRe)
		motionVc.push_back(Extend(nextC.mGetR(), c.mGetR()));
	if (IsRotate)
		motionVc.push_back(Move());
	c = nextC;
	return motionVc;
}
