#include"FrameController.h"
#include"Frame.h"
#include<algorithm>
FrameController* FrameController::Alive=nullptr;

void FrameController::mRegistOb(Observer* Ob){
	ID MID = ((Frame*)Ob)->mGetMID();
	ID LID = ((Frame*)Ob)->mGetLID();

	mvFr[MID][LID] = (Frame*)(Ob);
}
void FrameController::mUnRegistOb(Observer* Ob){
	ID MID = ((Frame*)Ob)->mGetMID();
	ID LID = ((Frame*)Ob)->mGetLID();
	mvFr[MID][LID]->mStop();
	mvFr[MID][LID] = nullptr;
}

void FrameController::mNotify(){
	
}
void FrameController::mvCreate(){

	vector<Label*>::const_iterator it;
	
	for (it = mvVec.begin(); it != mvVec.end(); ++it){
		ID MId = (*it)->mGetMarkID();
		ID LId = (*it)->mGetID();
		if (mvFindOb(*it) == false){
			mvObVc.push_back((Factory->mvCreate(*it,mvPriority)));
			mRegistOb(mvObVc.back());
		}
	}
}

void FrameController::mvFindDelete(){
	vector<Label*>::const_iterator it1;
	for (it1 = mvVec.begin(); it1 != mvVec.end(); ++it1){
		ID MID = (*it1)->mGetMarkID();
		ID LID = (*it1)->mGetID();
		if (mvFindOb(*it1) == false)
			mvCreateFr.push_back(*it1);
		else
			mvFr[MID][LID]->mAlive();
	}
	vector<Frame*>::iterator it2;
	for (it2 = mvObVc.begin(); it2 != mvObVc.end();++it2)
		if ((*it2)->mGetAlive() == false){
			vector<Frame*>::iterator tmp = it2--;
			mvDeleteFr.push_back(*tmp);
			swap(*tmp, *(mvObVc.end()-1));
			mvObVc.pop_back();
		}
		
}
// 라벨 입력 완료후 호출
void FrameController::mvDeleteFrame(){
	vector<Frame*>::iterator it;
	for (it = mvDeleteFr.begin(); it != mvDeleteFr.end(); ++it){
		ID MID = (*it)->mGetMID();
		ID LID = (*it)->mGetLID();
		(*it)->mStop();
		mvFr[MID][LID] = nullptr;
		delete (*it);
	}
}
void FrameController::mControll(){
	// 삭제 Frame 찾기
	mvFindDelete();
	mvDeleteFrame();

	// 우선순위 정렬
	// Frame 생성
}
