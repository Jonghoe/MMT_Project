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
	mvFr[MID][LID]->mEnd();
	mvFr[MID][LID] = nullptr;
}

void FrameController::mNotify(){
	State Update = UPDATE;
	for (size_t i = 0; i < mvObVc.size(); ++i)
		if (mvObVc[i]->mIsAllSet())
			mvObVc[i]->mUpdate(&Update);
}
void FrameController::mvCreateFr(){
	mvPriority = 255 - mvObVc.size();
	vector<Label*>::const_iterator it;
	for (it = mvCrFrVc.begin(); it != mvCrFrVc.end(); ++it){
		Frame* New = mvCrFr(*it, mvPriority--);
		mvObVc.push_back(New);
		mRegistOb(New);
		New->mAction();
	}
	mvCrFrVc.resize(0);
}

void FrameController::mvAdjust(){
	//버퍼들 중 이미 있는 놈들은 제외
	vector<Label*>::const_iterator it1;
	vector<Frame*>::iterator it2;
	// 프레임이 들어오는지 안들어오는지 확인하기 위한 절차
	for (size_t i = 0; i < mvObVc.size(); i++)
		mvObVc[i]->mGetBaseFr() ? NULL : mvObVc[i]->mAlive(false);
	
	for (it1 = mvBuffer.begin(); it1 != mvBuffer.end(); ++it1){
		ID MID = (*it1)->mGetMarkID();
		ID LID = (*it1)->mGetID();
		if (mvFindOb(*it1) == false)
 			mvCrFrVc.push_back(*it1);
		else
			mvFr[MID][LID]->mAlive(true);
	}
	//버퍼 사용후 사이즈 0 으로 초기화FillBlank
	mvBuffer.resize(0);
	
	//기존에 있던 Frame 들중 죽은 경우 삭제될 Frame을 mvDlFr로 이동
	size_t Count = 0;
	while (Count < mvObVc.size())
		if (mvObVc[Count]->mGetAlive() == false){
			mvDlFrVc.push_back(mvObVc[Count]);
			swap(mvObVc[Count], mvObVc[mvObVc.size() - 1]);
			mvObVc.pop_back();
		}
		else
			++Count;
}
// 라벨 입력 완료후 호출
void FrameController::mvDeleteFrame(){
	vector<Frame*>::iterator it;
	for (it = mvDlFrVc.begin(); it != mvDlFrVc.end();){
		Frame* tmp = *it;
		mUnRegistOb(*it);
		WaitForSingleObjectEx((*it++)->mGetHandle(), INFINITE,true);
		delete tmp;
	}
	mvDlFrVc.resize(0);
}
void FrameController::mvModifyPr(size_t l, size_t r){
	// nullptr 의 경우 신경을 써야하는지 아닌지 아직 의문
#define Pr(x) x->mGetPriority()
	if (l >= r)
		return;
	//가장 앞 Ob가 nullptr일경우 nullptr이 아닌 놈을 찾아서 스왑
	if (mvObVc[l] == nullptr){
		size_t i = l;
		for ( ; i < r&&!(mvObVc[i] != nullptr); ++i);
		if (i < r)
			swap(mvObVc[l], mvObVc[i]);
	}
	//quick sort 구간
	size_t ll = l + 1, rr = r - 1;
	while (ll < rr){
		for (; ll < r &&!(mvObVc[ll] == nullptr 
			|| Pr(mvObVc[ll])> Pr(mvObVc[l])); ++ll)
				;
		for (; rr >= ll && !(mvObVc[rr] != nullptr 
			|| Pr(mvObVc[rr]) < Pr(mvObVc[l])); --rr)
		if (ll < rr)
			swap(mvObVc[ll], mvObVc[rr]);
	}
	swap(mvObVc[l], mvObVc[rr]);
	//우선순위 변환
	mvObVc[rr]->mSetPriority(255-rr);
	mvModifyPr(l, rr);
	mvModifyPr(rr + 1, r);
}

Frame* FrameController::mvRightFrame(const Event& e)const{
	vector<Frame*>::const_iterator it;
	for (it = mvObVc.begin(); it != mvObVc.end(); ++it)
		if ((*it)->mIn(e.mGetLoc()))
			return *it;
	return nullptr;
}
void FrameController::mvThrowEvent(){
	vector<Label*>::const_iterator it;
	Event tmp;
	for (it = mvHand.begin(); it != mvHand.end(); ++it)
		mvRightFrame(tmp)->mEventPush(Event::LtoE(**it));
	mNotify();
}
void FrameController::mControll(){
	// 삭제할 Frame 찾기
	mvAdjust();
	// 삭제할 Frame 삭제
	mvDeleteFrame();
	// 우선순위 정렬
	mvModifyPr(0, mvObVc.size());
	// 이벤트 전달
	mvThrowEvent();
	if (mvCrFrVc.size() != 0)
	// Frame 생성
		mvCreateFr();
	
}
