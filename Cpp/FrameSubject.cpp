#include"FrameSubject.h"

int FrameSubject::MarkID=0;
int FrameSubject::LabelID=0;
FrameFactory* FrameSubject::Factory=NULL;
FrameSubject* FrameSubject::Alive=NULL;

void FrameSubject::mRegistOb(Observer* Ob){
	mvOb[MarkID][LabelID] = Ob;
}
void FrameSubject::mPush(Label* lb){
	mvVec.push_back(lb);
}
bool FrameSubject::mFindOb(Label* Lb){
	return mvOb[Lb->mGetMarkID()][Lb->mGetID()] != NULL;
}

void FrameSubject::mvArrangement(){
	vector<Label*>::iterator it;
	mvObArSz = mvHdArSz = 0;
	for (it = mvVec.begin(); it != mvVec.end(); ++it){
		unsigned MId = (*it)->mGetMarkID();
		unsigned LId = (*it)->mGetID();
		if (MId!= 0 && mFindOb(*it) == false)
			mvObArray[mvObArSz++] = (Factory->mvCreate(*it));
		else if ((*it)->mGetMarkID() != 0)
			mvObArray[mvObArSz++] = mvOb[MId][LId];
		else
			mvHand[mvHdArSz++] = (*it);
	}
}

void FrameSubject::mUnRegistOb(Observer* Ob){
	delete mvOb[MarkID][LabelID];
	mvOb[MarkID][LabelID] = NULL;
}

void FrameSubject::mNotify(){
	for (int i = 0; i < mvObArSz; ++i)
		for (int j = 0; j < mvHdArSz; ++j)
			mvObArray[i]->mvUpdate(mvHand[j]);
}