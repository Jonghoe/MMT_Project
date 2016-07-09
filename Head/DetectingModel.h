#pragma once
#include"Label.h"
#include"MVCPatter.h"
#include"FrameSubject.h"
// ����� ���� ��Ʈ�ѷ�  ��� < -- > ��Ʈ�ѷ� < -- > ��
// �̱۷��� ���
#define MaxLabel  20
#define StartLabel 200
class DetectingModel {
//			Main ��� ���α׷��� �߽� ������ �ϴ� Ŭ����
//					----------- �����Լ� -----------
//  void mSendLabel()					:		���� ������ ��� Ŭ������ ����
//	void mStart()						:		���α׷� ����
//	void mEnd()							:		���α׷� ����
//	void mFindLabel()					:		�� ã��	
//	void mAddLabel(Label* ob)			:		�� �߰�
//	void mRemoveLabel(Label* OB)		:		�� ����
//	void mFindDeleteLabel(Target t)	:		���ŵ� �� ã��
//	void mSetNextLabelID()			:		���� ID�� ����
//	friend	bool MakeLabel(DetectingModel* Table, Label** ML, cv::Point FL, Type scale, uchar NOT): �� ����
	
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
