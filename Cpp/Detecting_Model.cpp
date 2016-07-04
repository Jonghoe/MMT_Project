#include"Detecting_Model.h"
#include"MyFunction.h"
#include"Debug_Viewer.h"
Detecting_Model* Detecting_Model::Alive = NULL;

Detecting_Model::Detecting_Model(){
	mvVc = cv::VideoCapture("Sample.avi");
	//mvVc.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	//mvVc.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
	cv::Size size = cv::Size((int)mvVc.get(cv::CAP_PROP_FRAME_WIDTH), (int)mvVc.get(cv::CAP_PROP_FRAME_HEIGHT));
	mvPrev_IMG.create(size, CV_8UC3);
	mvPrev_IMG = cv::Scalar::all(0);
	IMG_Setting(mvPrev_IMG);
	mvLabel_Num = StartLabel;
	mvMarkList = new MarkList();
	mvSubject = FrameSubject::mMakeSubject();
	mvKey = PLAY;
	mvMarkList->mAddMark(cv::imread("Mark_0.jpg", cv::IMREAD_COLOR));
	mvMarkList->mAddMark(cv::imread("Mark_1.jpg", cv::IMREAD_COLOR));
	mvMarkList->mAddMark(cv::imread("Mark_2.jpg", cv::IMREAD_COLOR));
}

bool Detecting_Model::mExit(){
	if (mvKey == EXIT){
		mvEnd();
		return true;
	}
	return false;

}

Detecting_Model::~Detecting_Model(){
	cout << "Detecting_Model_Delete" << endl;
	mvEnd();
	Alive = NULL;
}

void Detecting_Model::mvSet_Next_Label_ID(){
	//	LabelID ���۰� 200 ���� 220 ( ���� �ִ� ���� 20)���� ��
	//	�� ID�� ã�Ƽ� ���� LabelID ������ ����
	ID Label_ID[MaxLabel] = { 0 };
	size_t i = 0;
	auto iter = mvLabelList.begin();
	for (; iter != mvLabelList.end(); iter++)
		Label_ID[(*iter)->mGetID() % StartLabel] = 1;
	
	for (i = 0; i < MaxLabel; i++)
		if (Label_ID[i] == 0){
			mvLabel_Num = StartLabel + i;
			break;
		}
}

void Detecting_Model::mvFind_Label(){
	//	�ȼ��� ���� 255�̸鼭 � ���� ���� ���� �ƴ� ��� �� �ǽ� �������� �����Ͽ� 
	//	Make_Label�Լ� ȣ��.
	Label* Label_shell = NULL;
	uchar* ptrM = NULL, *ptrV = NULL;
	for (int y = 10; y < mvIMG.rows; y+=10){
		ptrM = mvIMG.ptr<uchar>(y);
		ptrV = mvPrev_IMG.ptr<uchar>(y);
		for (int x = 10; x < mvIMG.cols; x+=10)
			if(ptrM[x] != 0 && ptrV[x] <StartLabel){ 
				mvSet_Next_Label_ID();
				Label_shell = NULL;
				if (Make_Label(this, &Label_shell, cv::Point(x, y), LOW)){
					cout << Label_shell->mGetID() << "is Created!! " << endl;
					mvLabelList.push_back(Label_shell);
				}
			}
	}

	for (int y = 0; y < mvIMG.rows; y++){
		ptrM = mvIMG.ptr<uchar>(y);
		ptrV = mvPrev_IMG.ptr<uchar>(y);
		for (int x = 0; x < mvIMG.cols; x ++)
			if (ptrV[x] == DELETE_V)
				ptrV[x] = 0;
	}
}
void Detecting_Model::mvModify_Label(Target T){
	//	Target �� ALL�� ��� ��� ���� ����
	//	Target �� SUFFICIENT�� ��� ������ �����ϴ� ���� ����

	auto iter = mvLabelList.begin();
	int i = 0;
	list<Label*>::iterator DeleteContainer[MaxLabel] ;
	switch (T){
	case Detecting_Model::DELETE_ALL:
		for (; iter != mvLabelList.end(); iter++){
			(*iter)->mClearIMG();
			DeleteContainer[i++] = iter;
		}
		break;
	case Detecting_Model::SUFFICIENT:
		for (; iter != mvLabelList.end(); iter++){
			if (!(*iter)->mModify(mvIMG))
				DeleteContainer[i++] = iter;
		}
		break;
		
	default:
		break;
	}

	for (int j = 0; j < i; j++)
		mvRemove_Label(*DeleteContainer[j]);
	
	
}

void Detecting_Model::mDetecting(){
	//	Detecting_Model Ŭ������ ���� ������ �ϴ� �Լ� 
	//	�ݺ��Ǹ鼭 Blob , Detect �� �̷����� ������ �Ѵ�.
	if (((Debug_Controller*)mvConnected[1])->mKey_Pushed()){
		int Tmp_K = ((Debug_Controller*)mvConnected[1])->mPut_Key();
		bool Stop_K = Tmp_K == 's';
		mvKey = Stop_K ? mvKey==PLAY ? WAIT:PLAY : Tmp_K;
	}
	
	if (mvKey == PLAY){
		
		mvVc >> mvIMG;
		((Debug_Viewer*)mvConnected[0])->mSet_R_IMG(&mvIMG);
		((Debug_Viewer*)mvConnected[0])->mSet_Changed_IMG(&mvPrev_IMG);

		IMG_Setting(mvIMG);
		mvModify_Label(SUFFICIENT);
		mvFind_Label();
		mvSend_Label();
	}
}
void Detecting_Model:: mvSend_Label(){
	//// Sender �� ���� Tcp/IP ������� ���� ����
	////	���� Objective_C �� ����Ͽ� ��������� ���ġ�ʰ� �̿�
	string Info;
	auto iter = mvLabelList.begin();
	for (; iter != mvLabelList.end(); iter++){
		Info = Label_to_String(*(*iter));
		cout << Info << endl;
		//mvSender.mSend(Info);
	}
//	auto iter = mvLabelList.begin();
//	for (; iter != mvLabelList.end(); ++iter)
//		mvSubject->mPush(*iter);
}

void Detecting_Model::mvEnd(){
	//	�����Ҵ� ��� ����
	mvModify_Label(DELETE_ALL);
	delete mvMarkList;
}

void Detecting_Model::mvAdd_Label(Label* OB){
	mvLabelList.push_back(OB);
}

void Detecting_Model::mvRemove_Label(Label* OB){
	delete OB;
	mvLabelList.remove(OB);
}