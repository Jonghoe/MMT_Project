#pragma once
#include"Label.h"
#include"MVC_Patter.h"
#include"FrameSubject.h"
// 디버깅 전용 컨트롤러  뷰어 < -- > 컨트롤러 < -- > 모델
// 싱글레톤 사용
#define MaxLabel  20
#define StartLabel 200
class Detecting_Model {
//			Main 대신 프로그램의 중심 역할을 하는 클래스
//					----------- 내장함수 -----------
//  void mSend_Label()					:		라벨의 내용을 통신 클래스로 전달
//	void mStart()						:		프로그램 시작
//	void mEnd()							:		프로그램 종료
//	void mFind_Label()					:		라벨 찾기	
//	void mAdd_Label(Label* ob)			:		라벨 추가
//	void mRemove_Label(Label* OB)		:		라벨 제거
//	void mFind_Delete_Label(Target t)	:		제거될 라벨 찾기
//	void mSet_Next_Label_ID()			:		라벨의 ID값 설정
//	friend	bool Make_Label(Detecting_Model* Table, Label** ML, cv::Point F_L, Type scale, uchar NOT): 라벨 생성
	
private:
	int mvKey;

	static Detecting_Model* Alive;
	enum Target{DELETE_ALL,SUFFICIENT};
	enum Command{  EXIT = 27, WAIT= 's',PLAY};
	MarkList* mvMarkList;
	cv::VideoCapture mvVc;
	cv::Mat mvPrev_IMG;
	cv::Mat mvIMG;
	uchar mvLabel_Num;
	std::list<Label*> mvLabelList;
	void* mvConnected[2];
	void mvMove_Label();
	void mvSend_Label();
	void mvEnd();
	void mvFind_Label();
	void mvAdd_Label(Label* ob);
	void mvRemove_Label(Label* OB);
	void mvSet_Next_Label_ID();
	void mvModify_Label(Target t);
	static bool Make_Label(Detecting_Model* Model, Label** ML, cv::Point F_L, Type scale);
	FrameSubject * mvSubject;
	Detecting_Model();
	~Detecting_Model();
public:
	void mConnect(MVC Type, void* Vp){
		if (Type == MVC::V)			mvConnected[0] = Vp;
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
	static Detecting_Model* mMake_Model(){
		if (Alive != NULL)return Alive;
		Alive = new Detecting_Model();
		return Alive;
	}
	bool mExit();
	void mDetecting();
};
