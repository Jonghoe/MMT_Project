#pragma once
#include<Windows.h>
#include<process.h>
#include"Common.h"
#include"Observer.h"
#include"Label.h"
#include"EventHandler.h"
using namespace std;
using namespace cv;
// 프레임 ( window을 의미 )
// 기본적으로 생성되는 프레임(Fr로시작)  --- 마크에 의해 생기는 프레임(MF으로 시작)으로 나눠짐.
class Frame : public Observer{
	
protected:
	bool mvBaseFr;								//Frame이 Base프레임인지 아닌지 확인
	State mvSt;									//Frame의 상태
	EventHandler mvEH;							//Frame의 이벤트 핸들러
	ID mvPriority;								//Frame의 우선순위
	int mvHdSz;									//Frame안의 손가락 수
	Point mvHdAr[10];							//Frame안의 손가락 배열
	Point mvCenter, mvLT, mvRB;					//Frame 중심,왼쪽위 오른쪽아래 좌표
	bool Alive;									//Frame의 동작 여부
	bool mvEnd;									//Frame 종료시 true
	const ID mvMarkID;							//Mark값
	const ID mvLabelID;							//라벨값
	size_t mvWidth, mvHeight;					//window의 너비와 높이
	HANDLE mvThread;
	static unsigned WINAPI mainPerform(void*);
	virtual void mvStart(){};					//프레잇 첫 시작시 실행 함수
	virtual void mvStop(){};					//프레임 정지시 실행 함수
	virtual void mvPlay(){};					//프레임 동작중 실행 함수
	void mvUpdate(){							//프레임 업데이트시 실행 함수
		mCatchMotion(mvEH.mPutMotion());
	}
public:
	Frame(Label* lb, ID id, size_t w, size_t h) :mvEH(&mvCenter),
	mvMarkID(lb->mGetMarkID()),mvLabelID(lb->mGetID()),
	mvLT(lb->mGetCenter()),mvWidth(w),mvHeight(h){ //MarkID값과LabelID값을받아 생성
		mvBaseFr = true;
		mvSt = PLAY;
		mvRB.x = mvLT.x + mvWidth;
		mvRB.y = mvLT.y + mvHeight;
		mvHdSz = 0;
		Alive = false;
		mvPriority = id;
		mvEnd = false;
	}
	Point& mGetCenter(){ return mvCenter; }
	ID mGetMID(){ return mvMarkID; }
	ID mGetLID(){ return mvLabelID; }
	ID mGetPriority(){ return mvPriority; }		//우선순위 반환
	void mSetPriority(ID pr){ mvPriority = pr; }//우선순위 설정
	State mGetState(){ return mvSt; }
	void mCatchMotion(vector<Motion> m){ 
		for (size_t i = 0; i < m.size(); ++i)
		m[i](this); 
	}
	bool mIsAllSet(){ return mvEH.mGetUpdate(); }//한프레임에서 모든 이벤트가 들어왔는지 확인
	void mUpdate(void* st){ mvSt = *(State*)st; }//Subject로부터 업데이트
	void mStart(){ mvSt = START; }
	void mStop(){ mvSt = STOP; }				//멈춤
	void mPlay(){ mvSt = PLAY; }				//동작
	void mEnd(){ mvSt = END; }					//종료
	bool mIsEnd(){ return mvSt == END; }
	bool mGetAlive(){ return Alive; }
	void mAlive(bool _live){ Alive = _live; }
	void mEventPush(Event& e){ mvEH.mPush(e); }
	
	EventHandler* mGetEventHandler(){ return &mvEH; }
	HANDLE& mGetHandle(){ return mvThread; }
	void mResize(double v){
		mvWidth *= (int)v;
		mvHeight *= (int)v;
		mvCenter *= (int)v;
		mvLT *= (int)v;
		mvRB *= (int)v;
	};

	bool mIn(const Point& Loc)const{				// Loc가 Frame 안에 있는지 확인
		bool inCol = mvLT.x < Loc.x&&Loc.x < mvRB.x;
		bool inRow = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return inCol&&inRow;
	}
	bool mGetBaseFr(){ return mvBaseFr; }
	virtual void mAction() {
		mvThread = (HANDLE)_beginthreadex(NULL,NULL,mainPerform,this,NULL,NULL);
	}
};