#pragma once
#include"Label.h"
#include"MVCPatter.h"
#include"FrameSubject.h"
// µð¹ö±ë Àü¿ë ÄÁÆ®·Ñ·¯  ºä¾î < -- > ÄÁÆ®·Ñ·¯ < -- > ¸ðµ¨
// ½Ì±Û·¹Åæ »ç¿ë
#define MaxLabel  20
#define StartLabel 200
class DetectingModel {
private:
	int mvKey;
	static DetectingModel* Alive;
	enum Target{DELETEALL,SUFFICIENT};
	enum Command{  EXIT = 27,ADD='a', WAIT= 's',PLAY};
	MarkList* mvMarkList;
	cv::VideoCapture mvVc;
	cv::Mat mvPrevIMG;
	cv::Mat mvIMG;
	uchar mvLabelNum;
	std::list<Label*> mvLabelList;
	void* mvConnected[2];
	void mvSelectAct();
	void mvDetecting();
	void mvExit();
	void mvEnd();
	void mvAddMark();
	void mvCheckLabel(cv::Point& firstP);
	void mvMoveLabel();
	void mvSendLabel();
	void mvFindLabel();
	void mvAddLabel(Label* ob);
	void mvRemoveLabel(Label* OB);
	void mvSetNextLabelID();
	void mvModifyLabel(Target t);

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
	bool mAction();
};
