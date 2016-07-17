#pragma once
#include"FrameFactory.h"
#include"Subject.h"
#include"Frame.h"
//Frame 관리 클래스
//아직 예상 클래스
class FrameSubject :public Subject
{
private:
	bool mvStop;							// 쓰레드로 동작시 업데이트될 경우
											// 잠시 멈추게끔 하는 변수
	static int MarkID, LabelID;				// 마크ID 값으로 기능 설정
											// 라벨ID 값과 마크ID값으로 구별
	static FrameSubject * Alive;			// 싱글레톤 기법 사용을 위해 존재
	FrameFactory * Factory;					//  Frame을 만드는 클래스 포인터
	int mvObArSz;							// 옵저버 배열의 크기
	int mvHdArSz;							// 손가락 배열의 크기
	Frame* mvObArray[20];				// 업데이트될 옵저버 배열
	Label* mvHand[20];						// 업데이트될 손가락 배열
	Frame* mvOb[20][20];					// 옵저버 저장 배열
	vector<Label*>	mvVec;					// 업데이트될 정보 벡터 
	FrameSubject::FrameSubject() :mvObArSz(0), mvHdArSz(0){
		Factory = FrameFactory::mMakeFactory();
	}
	void mRegistOb(Observer*);				//Observer 등록
	void mUnRegistOb(Observer*);			//Observer 해제
	bool mFindOb(Label* Lb);				//Observer 검색
	void mNotify();							//Observer 업데이트
	void mvArrangement();					//Observer 정리
	void mvHandThrow();						//이벤트 Throw
public:
	void mSetStop(bool s){					//멈춤 설정
		mvStop = s;
	}
	void mPush(Label*);						//라벨정보를 저장
	static FrameSubject* mMakeSubject(){	// 싱글레톤 생성 함수
		if (Alive == NULL)
			Alive = new FrameSubject();
		return Alive;
	}

	void mDeleteController(){				// 싱글레톤 파괴 함수
		if (Alive == NULL)
			return;
		delete this;
	}
};
