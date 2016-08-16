#pragma once
#include<Windows.h>
#include<process.h>
#include"Observer.h"
#include"opencv.hpp"
#include"Label.h"
using namespace std;
// 프레임 ( 창들을 의미 )
//우선순위 생성을 어떻게 할지 아직 정하지 않음.
class Frame : public Observer{
	
	static unsigned WINAPI mainPerform(void*);

protected:
	ID mvPriority;							//Frame의 우선순위
	int mvHdSz;								//Frame안의 손가락 수
	cv::Point mvHdAr[10];					//Frame안의 손가락 배열
	cv::Point mvCenter, mvLT, mvRB;			//Frame 중심,왼쪽위 오른쪽아래 좌표
	bool Alive;								//Frame의 동작 여부
	bool mvEnd;								//Frame 종료시 true
	bool mvStop;							//Frame 정지시 true
	const ID mvMarkID;						//Mark값
	const ID mvLabelID;						//라벨값
	int mvWidth, mvHeight;
public:
	
	Frame(Label* lb,ID id):mvMarkID(lb->mGetMarkID()),mvLabelID(lb->mGetID()),
	mvLT(lb->mGetCenter()){ //MarkID값과LabelID값을받아 생성
		mvHdSz = 0;
		Alive = false;
		mvPriority = id;
		mvEnd = false;
		mvStop = false;
	}
	void mvUpdate(Label* info){};				
	void mStop();
	ID mGetMID(){ return mvMarkID; }
	ID mGetLID(){ return mvLabelID; }
	ID mGetPriority(){ return mvPriority; }//우선순위 반환
	void mSetPriority(ID pr){ mvPriority = pr; }//우선순위 설정
	void mAlive(bool _live){ Alive = _live; }
	void mEnd(){ mvEnd = true; }
	bool mGetAlive(){ return Alive; }
	bool mIn(cv::Point Loc){				// Loc가 Frame 안에 있는지 확인
		bool inCol = mvLT.x < Loc.x&&Loc.x < mvRB.x;
		bool inRow = mvLT.y < Loc.y&&Loc.y < mvRB.y;
		return inCol&&inRow;
	}
	bool mGetStop(){ return mvStop; }
	bool mGetEnd(){ return mvStop; }
	virtual void mAction() {
		_beginthreadex(NULL,NULL,mainPerform,this,NULL,NULL);
	}
};