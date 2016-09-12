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
	//���۵� �� �̹� �ִ� ����� ����
	vector<Label*>::const_iterator it1;
	vector<Frame*>::iterator it2;
	// �������� �������� �ȵ������� Ȯ���ϱ� ���� ����
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
	//���� ����� ������ 0 ���� �ʱ�ȭFillBlank
	mvBuffer.resize(0);
	
	//������ �ִ� Frame ���� ���� ��� ������ Frame�� mvDlFr�� �̵�
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
// �� �Է� �Ϸ��� ȣ��
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
	// nullptr �� ��� �Ű��� ����ϴ��� �ƴ��� ���� �ǹ�
#define Pr(x) x->mGetPriority()
	if (l >= r)
		return;
	//���� �� Ob�� nullptr�ϰ�� nullptr�� �ƴ� ���� ã�Ƽ� ����
	if (mvObVc[l] == nullptr){
		size_t i = l;
		for ( ; i < r&&!(mvObVc[i] != nullptr); ++i);
		if (i < r)
			swap(mvObVc[l], mvObVc[i]);
	}
	//quick sort ����
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
	//�켱���� ��ȯ
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
	// ������ Frame ã��
	mvAdjust();
	// ������ Frame ����
	mvDeleteFrame();
	// �켱���� ����
	mvModifyPr(0, mvObVc.size());
	// �̺�Ʈ ����
	mvThrowEvent();
	if (mvCrFrVc.size() != 0)
	// Frame ����
		mvCreateFr();
	
}
