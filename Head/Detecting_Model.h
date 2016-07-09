#pragma once
#include"Label.h"
#include"MVC_Patter.h"
#include"FrameSubject.h"
// ����� ���� ��Ʈ�ѷ�  ��� < -- > ��Ʈ�ѷ� < -- > ��
// �̱۷��� ���
#define MaxLabel  20
#define StartLabel 200
class Detecting_Model {
//			Main ��� ���α׷��� �߽� ������ �ϴ� Ŭ����
//					----------- �����Լ� -----------
//  void mSend_Label()					:		���� ������ ��� Ŭ������ ����
//	void mStart()						:		���α׷� ����
//	void mEnd()							:		���α׷� ����
//	void mFind_Label()					:		�� ã��	
//	void mAdd_Label(Label* ob)			:		�� �߰�
//	void mRemove_Label(Label* OB)		:		�� ����
//	void mFind_Delete_Label(Target t)	:		���ŵ� �� ã��
//	void mSet_Next_Label_ID()			:		���� ID�� ����
//	friend	bool Make_Label(Detecting_Model* Table, Label** ML, cv::Point F_L, Type scale, uchar NOT): �� ����
	
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
