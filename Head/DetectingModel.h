#pragma once
#include"Label.h"
#include"MVCPatter.h"
#include"Classification.h"
//������ ���
#define DEBUG

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
	void* mvConnected[2];				//DbugController,Viewer ��ü ������
	void mvSelectAct();					//mvKey ��(/Command)���� �� �ൿ�� ���ϴ� �Լ�
	void mvDetecting();					//���󿡼� �� �˻�
	void mvEnd();						//�޸� ������ �ʿ��� ������� �޸� ����
										//�����ϴ� �Լ�
	void mvFindLabel();					//�� ã��
	void mvAddMark();					//��ũ �߰�
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
public:
	void mConnect(MVC Type, void* Vp){  //Dbug Controller,Viewr ������ ���� �Լ�
		if (Type == MVC::V)			mvConnected[0] = Vp;
		else if (Type == MVC::C)	mvConnected[1] = Vp;
	}
	static DetectingModel* mMakeModel(){ //�̱۷��� ���� �Լ�
		if (Alive != NULL)return Alive;
		Alive = new DetectingModel();
		return Alive;
	}
	void mDeleteModel(){				// �̱۷��� �ı� �Լ�
		if (Alive == NULL)
			return;
		delete this;
	}
	bool mAction();						// Model �ൿ �Լ�
										// mSelectAct()ȣ���Ͽ� ���
};