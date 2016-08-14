#include"DetectingModel.h"
#include"MyFunction.h"
#include"DebugViewer.h"
DetectingModel* DetectingModel::Alive = NULL;

DetectingModel::DetectingModel(){
	mvVc = cv::VideoCapture("Sample.avi");
	//mvVc.set(CVCAPPROPFRAMEWIDTH, 1920);
	//mvVc.set(CVCAPPROPFRAMEHEIGHT, 1080);
	cv::Size size = cv::Size((int)mvVc.get(cv::CAP_PROP_FRAME_WIDTH),
							(int)mvVc.get(cv::CAP_PROP_FRAME_HEIGHT));
	mvPrevIMG.create(size, CV_8UC3);
	mvPrevIMG = cv::Scalar::all(0);
	IMGSetting(mvPrevIMG);
	mvLabelNum = StartLabel;
	mvMarkList = new MarkList();
	mvClassify = new Classification();
	mvKey = PLAY;
	mvMarkList->mAddMark(cv::imread("Mark_0.jpg", cv::IMREAD_COLOR));
	mvMarkList->mAddMark(cv::imread("Mark_1.jpg", cv::IMREAD_COLOR));
	mvMarkList->mAddMark(cv::imread("Mark_2.jpg", cv::IMREAD_COLOR));
}


DetectingModel::~DetectingModel(){
	cout << "DetectingModelDelete" << endl;
	mvEnd();
	Alive = NULL;
}

void DetectingModel::mvSetNextLabelID(){
	//	LabelID ���۰� 200 ���� 220 ( ���� �ִ� ���� 20)���� ��
	//	�� ID�� ã�Ƽ� ���� LabelID ������ ����
	ID LabelID[MaxLabel] = { 0 };
	size_t i = 0;
	auto iter = mvLabelList.begin();
	for (; iter != mvLabelList.end(); iter++)
		LabelID[(*iter)->mGetID() % StartLabel] = 1;
	
	for (i = 0; i < MaxLabel; i++)
		if (LabelID[i] == 0){
			mvLabelNum = StartLabel + i;
			break;
		}
}
void DetectingModel::mvCheckLabel(cv::Point& firstP){
	Label* Labelshell = NULL;
	mvSetNextLabelID();
	if (Labelshell = MakeLabel(mvIMG, mvPrevIMG, firstP, mvLabelNum, LOW, MAKE)){
		Labelshell->mSetAngle(mvIMG);
		Labelshell->mSetMark(*mvMarkList);
		mvLabelList.push_back(Labelshell);
	}
}

void DetectingModel::mvFindLabel(){
	//	�ȼ��� ���� 255�̸鼭 � ���� ���� ���� �ƴ� ��� �� �ǽ� �������� �����Ͽ� 
	//	MakeLabel�Լ� ȣ��.
	uchar* ptrM = NULL, *ptrV = NULL;
	for (int y = 10; y < mvIMG.rows; y+=10){
		ptrM = mvIMG.ptr<uchar>(y);
		ptrV = mvPrevIMG.ptr<uchar>(y);
		for (int x = 10; x < mvIMG.cols; x += 10)
			if (ptrM[x] != 0 && ptrV[x] < StartLabel)
				mvCheckLabel(cv::Point(x,y));
	}

	for (int y = 0; y < mvIMG.rows; y++){
		ptrM = mvIMG.ptr<uchar>(y);
		ptrV = mvPrevIMG.ptr<uchar>(y);
		for (int x = 0; x < mvIMG.cols; x ++)
			if (ptrV[x] == DELETEV)
				ptrV[x] = 0;
	}
}
void DetectingModel::mvModifyLabel(Target T){
	//	Target �� ALL�� ��� ��� ���� ����
	//	Target �� SUFFICIENT�� ��� ������ �����ϴ� ���� ����

	auto iter = mvLabelList.begin();
	int i = 0;
	list<Label*>::iterator DeleteContainer[MaxLabel] ;
	switch (T){
	case DetectingModel::DELETEALL:
		for (; iter != mvLabelList.end(); iter++){
			(*iter)->mClearIMG();
			DeleteContainer[i++] = iter;
		}
		break;
	case DetectingModel::SUFFICIENT:
		for (; iter != mvLabelList.end(); iter++){
			if (!(*iter)->mModify(mvIMG))
				DeleteContainer[i++] = iter;
		}
		break;
		
	default:
		break;
	}

	for (int j = 0; j < i; j++)
		mvRemoveLabel(*DeleteContainer[j]);
	
	
}
void DetectingModel::mvAddMark(){
	string Buff;
	
	cin >> Buff;
	if (Buff.compare("end") == 0)
		mvKey = PLAY;
	else
		mvMarkList->mAddMark(cv::imread(Buff, cv::IMREAD_COLOR));
}
void DetectingModel::mvDetecting(){
	//	DetectingModel Ŭ������ ���� ������ �ϴ� �Լ� 
	//	�ݺ��Ǹ鼭 Blob , Detect �� �̷����� ������ �Ѵ�.
		
	mvVc >> mvIMG;
#ifdef DEBUG
	((DebugViewer*)mvConnected[0])->mSetRIMG(&mvIMG);
	((DebugViewer*)mvConnected[0])->mSetChangedIMG(&mvPrevIMG);
#endif
	IMGSetting(mvIMG);
	mvModifyLabel(SUFFICIENT);
	mvFindLabel();

}

void DetectingModel:: mvSendLabel(){
	//// Sender �� ���� Tcp/IP ������� ���� ����
	////	���� ObjectiveC �� ����Ͽ� ��������� ���ġ�ʰ� �̿�
	string Info;
	list<Label*>::iterator iter;
	for (iter = mvLabelList.begin(); iter != mvLabelList.end(); iter++){
		mvClassify->mGetInfo(*iter);
#ifdef DEBUG
		Info = LabeltoString(*(*iter));
		cout << Info << endl;
#endif
	}


}

void DetectingModel::mvEnd(){
	//	�����Ҵ� ��� ����
	mvModifyLabel(DELETEALL);
	delete mvMarkList;
}
void DetectingModel::mvSelectAct(){
	if (((DebugController*)mvConnected[1])->mKeyPushed()){
		int TmpK = ((DebugController*)mvConnected[1])->mPutKey();
		bool StopK = TmpK == 's';
		mvKey = StopK ? (mvKey == PLAY ? WAIT : PLAY) : TmpK;
	}
}

bool DetectingModel::mAction(){
#ifdef DEBUG
	mvSelectAct();
	cout << mvMarkList->mGetSize() << endl;
	if (mvKey == ADD)
		mvAddMark();
	else if (mvKey == PLAY){
		mvDetecting();
		mvSendLabel();

	}
	else if (mvKey == EXIT){
		mvEnd();
		return false;
	}
	return true;
#else if
	mvDetecting();
	if (mvKey = cv::waitKey(10) == 27){
		mvEnd();
		return false;
	}
	return true;
#endif

}

void DetectingModel::mvAddLabel(Label* OB){
	mvLabelList.push_back(OB);
}

void DetectingModel::mvRemoveLabel(Label* OB){
	delete OB;
	mvLabelList.remove(OB);
}
