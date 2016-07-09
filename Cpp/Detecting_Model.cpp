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
	//	LabelID 시작값 200 부터 220 ( 라벨의 최대 개수 20)까지 중
	//	빈 ID값 찾아서 다음 LabelID 값으로 설정
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
	//	픽셀의 값이 255이면서 어떤 라벨의 범위 안이 아닌 경우 라벨 의심 구역으로 지정하여 
	//	MakeLabel함수 호출.
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
	//	Target 이 ALL인 경우 모든 라벨을 삭제
	//	Target 이 SUFFICIENT인 경우 조건을 만족하는 라벨을 삭제

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
	//	DetectingModel 클래스의 메인 역할을 하는 함수 
	//	반복되면서 Blob , Detect 등 이런저런 역할을 한다.
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
	//// Sender 를 통해 Tcp/IP 기능으로 정보 전달
	////	추후 ObjectiveC 를 사용하여 소켓통신을 사용치않고 이용
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
	//	동적할당 모두 해제
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
	//	FindLabel에서 의심 받은 점(픽셀)부터 4방향으로 픽셀의 값이 255인 픽셀들의 수를 저장해 간다.
	//	만약 주변의 픽셀(255인)의 수가 기준 보다 작을 시 불필요한 라벨로 판정 
	//	기준에 만족시 라벨로 지정
	size_t PixelNum = 0;
	int Ranges[4] = { FL.x, FL.y, FL.y, FL.x };
	deque<cv::Point> PDeque;
	vector<cv::Point> DeleteList;		//	픽셀의 수가 범위 불충족시 그 부분을 지우기위한 컨테이너 
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
	// 라벨 내용 설정
	(*ML) = new Label(Ranges, PixelNum,
		Model->mvLabelNum, Model->mvMarkList,
		Model->mvIMG, Model->mvPrevIMG);
	return true;
}
