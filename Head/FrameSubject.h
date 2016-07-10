#pragma once
#include"FrameFactory.h"
#include"Subject.h"
class FrameSubject :public Subject
{
private:
	bool mvStop;
	static int MarkID, LabelID;
	static FrameSubject * Alive;
	static FrameFactory * Factory;
	int mvObArSz;
	int mvHdArSz;
	Observer* mvObArray[20];
	Label* mvHand[20];
	Observer* mvOb[20][20];
	vector<Label*>	mvVec;
	FrameSubject::FrameSubject() :mvObArSz(0), mvHdArSz(0){
		Factory = FrameFactory::mMakeFactory();
	}
	void mRegistOb(Observer*);
	void mUnRegistOb(Observer*);
	bool mFindOb(Label* Lb);
	void mNotify();
	void mvArrangement();
	void mvHandThrow();
public:
	void mSetStop(bool s){
		mvStop = s;
	}
	void mPush(Label*);
	static FrameSubject* mMakeSubject(){
		if (Alive == NULL)
			Alive = new FrameSubject();
		return Alive;
	}

};
