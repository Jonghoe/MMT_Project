#pragma once
#include"Label.h"
#include"MVCPatter.h"
#include"Classification.h"
//디버깅시 사용
#define DEBUG

// 디버깅 전용 컨트롤러  뷰어 < -- > 컨트롤러 < -- > 모델
// 싱글레톤 사용
#define MaxLabel  20
#define StartLabel 200
class DetectingModel {
private:
	enum Target{ DELETEALL, SUFFICIENT };
	enum Command{ EXIT = 27, ADD = 'a', WAIT = 's', PLAY };
	Classification * mvClassify;		//라벨 정보로 Frame생성 및 관리하는 객체 포인터
	int mvKey;							//DbugController한테 받은 키값을 저장
	static DetectingModel* Alive;		//싱그렐톤 기법을 위한 변수
	MarkList* mvMarkList;				//마크 리스트 포인터
	cv::VideoCapture mvVc;				//영싱 캡처를 위한 변수
	cv::Mat mvPrevIMG;					//현 영상과 비교하기위해 필요 한 이전 영상
	cv::Mat mvIMG;						//mvVc로 받은 영상 변수
	uchar mvLabelNum;					//라벨 생성 숫자
	std::list<Label*> mvLabelList;		//현재 영상에서 살아 있는 라벨들의 리스트
	void* mvConnected[2];				//DbugController,Viewer 객체 포인터
	void mvSelectAct();					//mvKey 값(/Command)으로 할 행동을 정하는 함수
	void mvDetecting();					//영상에서 라벨 검색
	void mvEnd();						//메모리 해제가 필요한 내용들을 메모리 해제
										//해제하는 함수
	void mvFindLabel();					//라벨 찾기
	void mvAddMark();					//마크 추가
	void mvCheckLabel(cv::Point& firstP);//이 임시라벨이 타당한 라벨인지 확인
	void mvMoveLabel();					//라벨의 움직임 체크
	void mvSendLabel();					//라벨 정보를 FrameController로 전달
	void mvAddLabel(Label* ob);			//라벨 추가
	void mvRemoveLabel(Label* OB);		//라벨 삭제
	void mvSetNextLabelID();			//라벨의 다음 ID값 설정
	void mvModifyLabel(Target t);		//라벨의 정보 변경이 필요한지 확인
										//Target으로 모든 라벨을 할지 조건을
										//충족하는 라벨만 할지 결정

	DetectingModel();					//Default 생성자 
										//싱글레톤이기에 private으로 설정

	~DetectingModel();					//파괴자
										//싱글레톤이기에 private으로 설정
public:
	void mConnect(MVC Type, void* Vp){  //Dbug Controller,Viewr 연결을 위한 함수
		if (Type == MVC::V)			mvConnected[0] = Vp;
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
	static DetectingModel* mMakeModel(){ //싱글레톤 생성 함수
		if (Alive != NULL)return Alive;
		Alive = new DetectingModel();
		return Alive;
	}
	void mDeleteModel(){				// 싱글레톤 파괴 함수
		if (Alive == NULL)
			return;
		delete this;
	}
	bool mAction();						// Model 행동 함수
										// mSelectAct()호출하여 사용
};