#pragma once
#include"Label.h"
#include"MVCPatter.h"
#include"Classification.h"
//������ ���
#define DEBUG_LV1

// ����� ���� ��Ʈ�ѷ�  ��� < -- > ��Ʈ�ѷ� < -- > ��
// �̱۷��� ���
#define MaxLabel  20
#define StartLabel 200
class DetectingModel {
private:
	enum Target{ DELETEALL, SUFFICIENT };
	enum Command{ EXIT = 27, ADD = 'a', WAIT = 's', PLAY };
	Classification * mvClassify;		//�� ������ Frame���� �� �����ϴ� ��ü ������
	int mvKey;							//DbugController���� ���� Ű���� ����
	static DetectingModel* Alive;		//�̱׷��� ����� ���� ����
	MarkList* mvMarkList;				//��ũ ����Ʈ ������
	cv::VideoCapture mvVc;				//���� ĸó�� ���� ����
	cv::Mat mvPrevIMG;					//�� ����� ���ϱ����� �ʿ� �� ���� ����
	cv::Mat mvIMG;						//mvVc�� ���� ���� ����
	uchar mvLabelNum;					//�� ���� ����
	std::list<Label*> mvLabelList;		//���� ���󿡼� ��� �ִ� �󺧵��� ����Ʈ
	void mvDetecting();					//���󿡼� �� �˻�
	void mvEnd();						//�޸� ������ �ʿ��� ������� �޸� ����
										//�����ϴ� �Լ�
	
	void mvFindLabel();					//�� ã��
	void mvCheckLabel(cv::Point& firstP);//�� �ӽö��� Ÿ���� ������ Ȯ��
	void mvMoveLabel();					//���� ������ üũ
	void mvSendLabel();					//�� ������ FrameController�� ����
	void mvAddLabel(Label* ob);			//�� �߰�
	void mvRemoveLabel(Label* OB);		//�� ����
	void mvSetNextLabelID();			//���� ���� ID�� ����
	void mvModifyLabel(Target t);		//���� ���� ������ �ʿ����� Ȯ��
										//Target���� ��� ���� ���� ������
										//�����ϴ� �󺧸� ���� ����
	DetectingModel();					//Default ������ 
										//�̱۷����̱⿡ private���� ����
	~DetectingModel();					//�ı���
										//�̱۷����̱⿡ private���� ����
#ifdef DEBUG_LV1
	void* mvConnected[2];				//DbugController,Viewer ��ü ������

	void mvSelectAct();					//mvKey ��(/Command)���� �� �ൿ�� ���ϴ� �Լ�
#endif
public:
	void mvAddMark();					//��ũ �߰�
#ifdef DEBUG_LV1
	void mConnect(MVC Type, void* Vp){  //Dbug Controller,Viewr ������ ���� �Լ�
		if (Type == MVC::V)			mvConnected[0] = Vp;
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
#endif
	static DetectingModel* mMakeModel(){ //�̱۷��� ���� �Լ�
		if (Alive != nullptr)return Alive;
		Alive = new DetectingModel();
		return Alive;
	}
	void mDeleteModel(){				// �̱۷��� �ı� �Լ�
		if (Alive == nullptr)
			return;
		delete this;
	}
	bool mAction();						// Model �ൿ �Լ�
										// mSelectAct()ȣ���Ͽ� ���
};