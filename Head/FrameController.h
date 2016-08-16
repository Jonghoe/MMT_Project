#pragma once
#include"FrameFactory.h"
#include"Subject.h"
#include"Frame.h"
#include<vector>

//Frame 관리 클래스
class FrameController :private Subject
{
	typedef unsigned ID;
	friend class Classification;
private:
	// 라벨ID 값과 마크ID값으로 구별  [Mark][Label]

	static FrameController * Alive;			// 싱글레톤 기법 사용을 위해 존재
	FrameFactory * Factory;					//  Frame을 만드는 클래스 포인터
	vector<Frame*> mvObVc;		      		// 업데이트될 옵저버 벡터
	vector<Label*> mvHand;					// 업데이트될 손가락 배열

	vector<Label*> mvCreateFr;				// 생성할 Frame 저장 벡터
	vector<Frame*> mvDeleteFr ;				// 삭제할 Frame 저장 벡터

	Frame* mvFr[20][3];  					// 옵저버 저장 배열
	vector<Label*> mvBuffer;				// 업데이트될 정보 벡터 
	ID mvPriority;							// Frame 생성시 우선순위
	FrameController::FrameController() :mvObVc(),mvHand(),mvCreateFr(),mvDeleteFr(),mvBuffer(){
		Factory = FrameFactory::mMakeFactory();
		mvPriority = 255;
		for (int i = 0; i < 20;++i)
			fill(*(mvFr+i), *(mvFr+i) + 3, nullptr);
	}
	void mvModifyPr(size_t l,size_t r);					//Priority를 정렬하면서 값을 변경 (오름차순으로)
	void mRegistOb(Observer*);				//Observer(Frame) 등록
	void mUnRegistOb(Observer*);			//Observer(Frame) 해제
	bool mvFindOb(Label* Lb){				//Observer(Frame) 검색
		void* tmp = mvFr[Lb->mGetMarkID()][Lb->mGetID()];
		return mvFr[Lb->mGetMarkID()][Lb->mGetID()] != nullptr;
	}
	void mNotify();							//Observer(Frame) 업데이트
	void mvCreate();	    				//Frame 생성
	
	void mvAdjust();						//Frame 생성및 삭제 구분
	void mvDeleteFrame();					//Frame 삭제

	void mPushMark(Label* lb){ mvBuffer.push_back(lb); }//마크 정보를 저장
	void mPushEvent(Label* lb){ mvBuffer.push_back(lb); }//이벤트 정보를 저장

public:
	
	static FrameController* mMakeController(){	// 싱글레톤 생성 함수
		if (Alive == nullptr)
			Alive = new FrameController();
		return Alive;
	}

	void mDeleteController(){				// 싱글레톤 파괴 함수
		if (Alive == nullptr)
			return;
		delete this;
	}
	void mControll();
	
};
