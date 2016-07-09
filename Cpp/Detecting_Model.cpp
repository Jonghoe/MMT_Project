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
	//	LabelID 시작값 200 부터 220 ( 라벨의 최대 개수 20)까지 중
	//	빈 ID값 찾아서 다음 LabelID 값으로 설정
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
	//	픽셀의 값이 255이면서 어떤 라벨의 범위 안이 아닌 경우 라벨 의심 구역으로 지정하여 
	//	Make_Label함수 호출.
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
	//	Target 이 ALL인 경우 모든 라벨을 삭제
	//	Target 이 SUFFICIENT인 경우 조건을 만족하는 라벨을 삭제

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
	//	Detecting_Model 클래스의 메인 역할을 하는 함수 
	//	반복되면서 Blob , Detect 등 이런저런 역할을 한다.
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
	//// Sender 를 통해 Tcp/IP 기능으로 정보 전달
	////	추후 Objective_C 를 사용하여 소켓통신을 사용치않고 이용
	string Info;
	auto iter = mvLabelList.begin();
	for (; iter != mvLabelList.end(); iter++){
		Info = Label_to_String(*(*iter));
		cout << Info << endl;
		mvSubject->mPush(*iter);
		//mvSender.mSend(Info);
	}
//	auto iter = mvLabelList.begin();
//	for (; iter != mvLabelList.end(); ++iter)
//		mvSubject->mPush(*iter);
}

void Detecting_Model::mvEnd(){
	//	동적할당 모두 해제
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

bool Detecting_Model::Make_Label(Detecting_Model* Model, Label** ML, cv::Point F_L, Type scale){
	//	Tabel : MainFrame
	//	ML : for MakingLabel
	//	F_L : First location
	//	scale : Range of Pixel
	//	Find_Label에서 의심 받은 점(픽셀)부터 4방향으로 픽셀의 값이 255인 픽셀들의 수를 저장해 간다.
	//	만약 주변의 픽셀(255인)의 수가 기준 보다 작을 시 불필요한 라벨로 판정 
	//	기준에 만족시 라벨로 지정
#define IN_RANGE(S,E,V) ((S)<(V)&&(V)<(E))
	size_t Pixel_Num = 0;
	int Ranges[4] = { F_L.x, F_L.y, F_L.y, F_L.x };
	deque<cv::Point> P_Deque;
	vector<cv::Point> Delete_List;		//	픽셀의 수가 범위 불충족시 그 부분을 지우기위한 컨테이너 
	P_Deque.push_front(F_L);
	Delete_List.reserve(U_LIMIT);
	Delete_List.push_back(F_L);
	while (!P_Deque.empty()){
		int i;
		cv::Point Loc = P_Deque.back();
		cv::Point Ar[4] = { cv::Point(Loc.x - 1, Loc.y), cv::Point(Loc.x, Loc.y - 1),
			cv::Point(Loc.x, Loc.y + 1), cv::Point(Loc.x + 1, Loc.y) };
		P_Deque.pop_back();
		for (i = 0; i < 4; i++){
			if ((IN_RANGE(0, Model->mvIMG.rows, Ar[i].y) &&
				IN_RANGE(0, Model->mvIMG.cols, Ar[i].x)) &&
				Model->mvIMG.ptr<uchar>(Ar[i].y)[Ar[i].x] != 0 &&
				Model->mvPrev_IMG.ptr<uchar>(Ar[i].y)[Ar[i].x] == 0){
				SetRange(Ar[i], Ranges);
				P_Deque.push_front(Ar[i]);
				Delete_List.push_back(cvPoint(Ar[i].x, Ar[i].y));
				Model->mvPrev_IMG.ptr<uchar>(Ar[i].y)[Ar[i].x] = Model->mvLabel_Num;
				++Pixel_Num;
			}
		}

	}
	// Delete_Check
	if (Delete_Check(Model->mvPrev_IMG, Delete_List, Pixel_Num, scale))
		return false;
	// 라벨 내용 설정
	(*ML) = new Label(Ranges, Pixel_Num, Model->mvLabel_Num, Model->mvMarkList, Model->mvIMG, Model->mvPrev_IMG);
	return true;
}
