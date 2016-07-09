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
	mvSubject = FrameSubject::mMakeSubject();
	mvKey = PLAY;
	mvMarkList->mAddMark(cv::imread("Mark_0.jpg", cv::IMREAD_COLOR));
	mvMarkList->mAddMark(cv::imread("Mark_1.jpg", cv::IMREAD_COLOR));
	mvMarkList->mAddMark(cv::imread("Mark_2.jpg", cv::IMREAD_COLOR));
}

bool DetectingModel::mExit(){
	if (mvKey == EXIT){
		mvEnd();
		return true;
	}
	return false;

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

void DetectingModel::mvFindLabel(){
	//	�ȼ��� ���� 255�̸鼭 � ���� ���� ���� �ƴ� ��� �� �ǽ� �������� �����Ͽ� 
	//	MakeLabel�Լ� ȣ��.
	Label* Labelshell = NULL;
	uchar* ptrM = NULL, *ptrV = NULL;
	for (int y = 10; y < mvIMG.rows; y+=10){
		ptrM = mvIMG.ptr<uchar>(y);
		ptrV = mvPrevIMG.ptr<uchar>(y);
		for (int x = 10; x < mvIMG.cols; x+=10)
			if(ptrM[x] != 0 && ptrV[x] <StartLabel){ 
				mvSetNextLabelID();
				Labelshell = NULL;
				if (MakeLabel(this, &Labelshell, cv::Point(x, y), LOW))
					mvLabelList.push_back(Labelshell);
			}
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

void DetectingModel::mDetecting(){
	//	DetectingModel Ŭ������ ���� ������ �ϴ� �Լ� 
	//	�ݺ��Ǹ鼭 Blob , Detect �� �̷����� ������ �Ѵ�.
	if (((DebugController*)mvConnected[1])->mKeyPushed()){
		int TmpK = ((DebugController*)mvConnected[1])->mPutKey();
		bool StopK = TmpK == 's';
		mvKey = StopK ? (mvKey==PLAY ? WAIT:PLAY) : TmpK;
	}
	
	if (mvKey == PLAY){
		mvVc >> mvIMG;
		((DebugViewer*)mvConnected[0])->mSetRIMG(&mvIMG);
		((DebugViewer*)mvConnected[0])->mSetChangedIMG(&mvPrevIMG);
		IMGSetting(mvIMG);
		mvModifyLabel(SUFFICIENT);
		mvFindLabel();
		mvSendLabel();
	}

}

void DetectingModel:: mvSendLabel(){
	//// Sender �� ���� Tcp/IP ������� ���� ����
	////	���� ObjectiveC �� ����Ͽ� ��������� ���ġ�ʰ� �̿�
	string Info;
	auto iter = mvLabelList.begin();
	for (; iter != mvLabelList.end(); iter++){
		Info = LabeltoString(*(*iter));
		cout << Info << endl;
		mvSubject->mPush(*iter);
		//mvSender.mSend(Info);
	}
//	auto iter = mvLabelList.begin();
//	for (; iter != mvLabelList.end(); ++iter)
//		mvSubject->mPush(*iter);
}

void DetectingModel::mvEnd(){
	//	�����Ҵ� ��� ����
	mvModifyLabel(DELETEALL);
	delete mvMarkList;
}

void DetectingModel::mvAddLabel(Label* OB){
	mvLabelList.push_back(OB);
}

void DetectingModel::mvRemoveLabel(Label* OB){
	delete OB;
	mvLabelList.remove(OB);
}

bool DetectingModel::MakeLabel(DetectingModel* Model, Label** ML, cv::Point FL, Type scale){
	//	Tabel : MainFrame
	//	ML : for MakingLabel
	//	FL : First location
	//	scale : Range of Pixel
	//	FindLabel���� �ǽ� ���� ��(�ȼ�)���� 4�������� �ȼ��� ���� 255�� �ȼ����� ���� ������ ����.
	//	���� �ֺ��� �ȼ�(255��)�� ���� ���� ���� ���� �� ���ʿ��� �󺧷� ���� 
	//	���ؿ� ������ �󺧷� ����
	size_t PixelNum = 0;
	int Ranges[4] = { FL.x, FL.y, FL.y, FL.x };
	deque<cv::Point> PDeque;
	vector<cv::Point> DeleteList;		//	�ȼ��� ���� ���� �������� �� �κ��� ��������� �����̳� 
	PDeque.push_front(FL);
	DeleteList.reserve(ULIMIT);
	DeleteList.push_back(FL);
	while (!PDeque.empty()){
		int i;
		cv::Point Loc = PDeque.back();
		cv::Point Ar[4] = { cv::Point(Loc.x - 1, Loc.y), cv::Point(Loc.x, Loc.y - 1),
			cv::Point(Loc.x, Loc.y + 1), cv::Point(Loc.x + 1, Loc.y) };
		PDeque.pop_back();
#define INRANGE(S,V,E) ((S)<(V)&&(V)<(E))
		for (i = 0; i < 4; i++){
			if ((INRANGE(0, Ar[i].y, Model->mvIMG.rows) &&
				INRANGE(0, Ar[i].x, Model->mvIMG.cols)) &&
				Model->mvIMG.ptr<uchar>(Ar[i].y)[Ar[i].x] != 0 &&
				Model->mvPrevIMG.ptr<uchar>(Ar[i].y)[Ar[i].x] == 0){
				SetRange(Ar[i], Ranges);
				PDeque.push_front(Ar[i]);
				DeleteList.push_back(cvPoint(Ar[i].x, Ar[i].y));
				Model->mvPrevIMG.ptr<uchar>(Ar[i].y)[Ar[i].x] = Model->mvLabelNum;
				++PixelNum;
			}
		}

	}
	// DeleteCheck
	if (DeleteCheck(Model->mvPrevIMG, DeleteList, PixelNum, scale))
		return false;
	// �� ���� ����
	(*ML) = new Label(Ranges, PixelNum,
		Model->mvLabelNum, Model->mvMarkList,
		Model->mvIMG, Model->mvPrevIMG);
	return true;
}
