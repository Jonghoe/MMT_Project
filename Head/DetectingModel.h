#pragma once
#include"Label.h"
#include"MVCPatter.h"
#include"FrameSubject.h"
// 디버깅 전용 컨트롤러  뷰어 < -- > 컨트롤러 < -- > 모델
// 싱글레톤 사용
#define MaxLabel  20
#define StartLabel 200
class DetectingModel {
//			Main 대신 프로그램의 중심 역할을 하는 클래스
//					----------- 내장함수 -----------
//  void mSendLabel()					:		라벨의 내용을 통신 클래스로 전달
//	void mStart()						:		프로그램 시작
//	void mEnd()							:		프로그램 종료
//	void mFindLabel()					:		라벨 찾기	
//	void mAddLabel(Label* ob)			:		라벨 추가
//	void mRemoveLabel(Label* OB)		:		라벨 제거
//	void mFindDeleteLabel(Target t)	:		제거될 라벨 찾기
//	void mSetNextLabelID()			:		라벨의 ID값 설정
//	friend	bool MakeLabel(DetectingModel* Table, Label** ML, cv::Point FL, Type scale, uchar NOT): 라벨 생성
	
private:
	int mvKey;
	static DetectingModel* Alive;
	enum Target{DELETEALL,SUFFICIENT};
	enum Command{  EXIT = 27, WAIT= 's',PLAY};
	MarkList* mvMarkList;
	cv::VideoCapture mvVc;
	cv::Mat mvPrevIMG;
	cv::Mat mvIMG;
	uchar mvLabelNum;
	std::list<Label*> mvLabelList;

	void* mvConnected[2];
	void mvMoveLabel();
	void mvSendLabel();
	void mvEnd();
	void mvFindLabel();
	void mvAddLabel(Label* ob);
	void mvRemoveLabel(Label* OB);
	void mvSetNextLabelID();
	void mvModifyLabel(Target t);
	static bool MakeLabel(DetectingModel* Model, Label** ML, cv::Point FL, Type scale);
	FrameSubject * mvSubject;
	DetectingModel();
	~DetectingModel();
public:
	void mConnect(MVC Type, void* Vp){
		if (Type == MVC::V)			mvConnected[0] = Vp;
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
	static DetectingModel* mMakeModel(){
		if (Alive != NULL)return Alive;
		Alive = new DetectingModel();
		return Alive;
	}
	void mDeleteModel(){
		if (Alive == NULL)
			return;
		delete this;
	}
	bool mExit();
	void mDetecting();
};
