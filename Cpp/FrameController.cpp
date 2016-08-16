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
	
	for (it = mvCreateFr.begin(); it != mvCreateFr.end(); ++it){
		Frame* New = Factory->mvCreate(*it, mvPriority);
		mvObVc.push_back(New);
		mRegistOb(New);
		New->mAction();
	}
	mvCreateFr.resize(0);
}

void FrameController::mvAdjust(){
	//버퍼들 중 이미 있는 놈들은 제외
	vector<Label*>::const_iterator it1;
	vector<Frame*>::iterator it2;
	for (size_t i = 0; i < mvObVc.size(); i++){
		mvObVc[i]->mAlive(false);
	}
	for (it1 = mvBuffer.begin(); it1 != mvBuffer.end(); ++it1){
		ID MID = (*it1)->mGetMarkID();
		ID LID = (*it1)->mGetID();
		if (mvFindOb(*it1) == false)
 			mvCreateFr.push_back(*it1);
		else
			mvFr[MID][LID]->mAlive(true);
	}
	//버퍼 사용후 사이즈 0 으로 초기화
	mvBuffer.resize(0);
	
	//기존에 있던 Frame 들중 죽은 경우 삭제될 Frame으로 지정
	int Count = 0;
	for (it2 = mvObVc.begin(); it2 != mvObVc.end()-Count;++it2)
		if ((*it2)->mGetAlive() == false){
			vector<Frame*>::iterator tmp = it2;
			if (mvObVc.size() != 1){
				--it2;
				++Count;
			}
			mvDeleteFr.push_back(*tmp);
			cv::Point A((*(mvObVc.end() - 1))->mGetLID(), (*(mvObVc.end() - 1))->mGetLID());
			cv::Point B((*(tmp))->mGetLID(), (*(tmp))->mGetLID());
			swap(*tmp, *(mvObVc.end()-1-Count));
		}
	for (int i = 0; i < mvDeleteFr.size(); ++i)
		mvObVc.pop_back();
}
// 라벨 입력 완료후 호출
void FrameController::mvDeleteFrame(){
	vector<Frame*>::iterator it;
	for (it = mvDeleteFr.begin(); it != mvDeleteFr.end(); ++it){
		ID MID = (*it)->mGetMID();
		ID LID = (*it)->mGetLID();
		(*it)->mEnd();
		mvFr[MID][LID] = nullptr;
		delete (*it);
	}
	mvDeleteFr.resize(0);
}
void FrameController::mvModifyPr(size_t l, size_t r){
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
void FrameController::mControll(){
	// 삭제할 Frame 찾기
	mvAdjust();
	// 삭제할 Frame 삭제
	mvDeleteFrame();
	// 우선순위 정렬
	mvModifyPr(0, mvObVc.size());
	mvPriority = 255 - mvObVc.size();
	// Frame 생성
	mvCreate();
}
