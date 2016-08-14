#include"Label.h"
#include"MyFunction.h"
ID Label::mGetID()const{ return this->mvID; }
ID Label::mGetMarkID()const{ return this->mvMarkID; }
cv::Point Label::mGetCenter()const{ return this->mvCenter; }
Label::Label(){ mvCenter = cv::Point(0, 0); mvPixelN = 0; mvTilt = 0; }
Label::Label(int* ranges, int pixelNum, int id,cv::Mat& prevImg){
	mvCenter = cv::Point((ranges[0] + ranges[3]) / 2, (ranges[1] + ranges[2]) / 2);
	mvRB = cv::Point(ranges[3] + 1, ranges[2] + 1);
	mvLT = cv::Point(ranges[0], ranges[1]);
	mvPixelN = pixelNum;
	mvID = id;
	mvPrevImg = prevImg;
	mvTilt = 0;
	mvZone=prevImg(cv::Range(mvLT.y, mvRB.y), cv::Range(mvLT.x, mvRB.x));
#ifdef DEBUG
	cout << mvID <<"is Created"<< endl;
#endif
}
Label::Label(int* ranges, int pixelNum, int id){
	mvCenter = cv::Point((ranges[0] + ranges[3]) / 2, (ranges[1] + ranges[2]) / 2);
	mvRB = cv::Point(ranges[3] + 1, ranges[2] + 1);
	mvLT = cv::Point(ranges[0], ranges[1]);
	mvPixelN = pixelNum;
	mvID = id;
}
Label::~Label(){}
void Label::mSetAngle(cv::Mat& IMG){
	mvTilt = mvCalculateAngle(IMG);
}
double Label::mvCalculateAngle(cv::Mat& IMG){
	list<Label*> temp;
	cv::Point locAngle;
	int yGap = mvRB.y - mvLT.y;
	int xGap = mvRB.x - mvLT.x;
	cv::Mat prevIMG = cv::Mat(cv::Size(xGap, yGap), CV_8U, cv::Scalar::all(0));
	cv::Mat img = IMG(cv::Range(mvLT.y, mvRB.y), cv::Range(mvLT.x, mvRB.x));
	cv::Mat revIMG;
	double tempRadius, angle = 0;
	const double PI = 2 * asin(1);
	//-------------------	영상 전처리 구간---------------------------------
	cv::threshold(img, revIMG, 100, 255, cv::THRESH_BINARY_INV);
	//----------------------------------------------------------------------
	//	각도를 나타내줄 포인트의 위치 검색
	FindLabel(revIMG, prevIMG, temp, mvID, HIGH);
	
	if (temp.size() == 0)
		return 0;

	locAngle = (*temp.begin())->mvCenter;
	cv::Point Loc(locAngle.x - (xGap) / 2, locAngle.y - (yGap) / 2);
	tempRadius = sqrt((pow(Loc.x, 2) + pow(Loc.y, 2)));

	//	각도를 구하기 위한 위치 구역별로 확인
	if (Loc.x > 0)
		angle = asin(Loc.y / tempRadius);
	else
		if (Loc.y >= 0)
			angle = PI - asin(Loc.y / tempRadius);
		else
			angle = -PI - asin(Loc.y / tempRadius);
	for (auto iter = temp.begin(); iter != temp.end(); iter++)
		delete *iter;

	return angle;
}
void Label::mvRotateImg(){
	cv::Mat dst;
	cv::Point2f pt((float)(mvPrevImg.cols / 2), (float)(mvPrevImg.rows / 2));
	cv::Mat r = getRotationMatrix2D(pt, -mvTilt, 1.0);
	warpAffine(mvPrevImg, mvZone, r, cv::Size(mvPrevImg.cols, mvPrevImg.rows));
}
void Label::mSetMark(const MarkList& markList){
	mvMarkID = markList.mFindMark(mvZone);
}


bool Label::mModify(cv::Mat& mimg){
	//	라벨의 이미지 범위 안의 픽셀 값이 255인 픽셀의 수를 세어
	//	기준보다 픽셀의 수가 적으면 라벨을 삭제
	//	기준보다 많을 경우 라벨 움직임 확인
	size_t pixelNum = 0;
	for (int y = mvLT.y; y < mimg.rows&& y < mvRB.y; y++){
		uchar* ptrI = mimg.ptr<uchar>(y);
		for (int x = mvLT.x; x < mimg.cols&& x < mvRB.x; x++){
			if (ptrI[x] != 0)
				pixelNum++;
			if (pixelNum > DLIMIT){
				this->mvMoving(mimg);
				return true;
			}
		}
	}
	this->mClearIMG();
	return false;
}

bool Label::mContainPoint(const cv::Point Loc)const{	
	//	점 Loc가 라벨의 안인지 아닌지 확인
	bool inX = this->mvLT.x <= Loc.x && Loc.x <= this->mvRB.x;
	bool inY = this->mvLT.y <= Loc.y && Loc.y <= this->mvRB.y;
	return inX&&inY;
}
void Label::mvMoving(cv::Mat& mIMG){
	//	라벨의 범위 내에서 다시 블랍 하여 라벨을 이동( 재생성하여 위치 이동 )
	list<Label*>VLabel;
	(this)->mClearIMG();
	for (int y = mvLT.y; y < mvRB.y; y++){
		uchar* ptrM = mIMG.ptr<uchar>(y);
		uchar* ptrV = mvPrevImg.ptr<uchar>(y);
		for (int x = mvLT.x; x < mvRB.x; x++)
			if (ptrM[x] != 0 && ptrV[x] == 0){
				Label* tmp = NULL;
				if ((tmp = MakeLabel(mIMG, mvPrevImg, cv::Point(x, y), mvID, LOW,TRACKING)))
					VLabel.push_back(tmp);
			}
	}
	if (VLabel.size() != 0){
		mvCenter = (*VLabel.begin())->mvCenter;
		mvLT = (*VLabel.begin())->mvLT;
		mvRB = (*VLabel.begin())->mvRB;
	}
}
Label* MakeLabel(const cv::Mat& img, cv::Mat& prevImg, const cv::Point FL, const uchar val, const Type scale,Act T){
#define INRANGE(b,v,e) ((b)<(v))&&((v)<(e))

	int pixelNum = 0;
	int ranges[4] = { FL.x, FL.y, FL.y, FL.x };
	deque<cv::Point> pDeq;
	pDeq.push_front(FL);
	vector<cv::Point> deleteList;		//	픽셀의 수가 범위 불충족시 그 부분을 지우기위한 컨테이너 
	deleteList.reserve(ULIMIT);
	deleteList.push_back(FL);
	while (!pDeq.empty()){
		cv::Point* Pt;
		cv::Point Loc = pDeq.back();
		cv::Point Ar[4] = { cv::Point(Loc.x - 1, Loc.y), cv::Point(Loc.x, Loc.y - 1),
			cv::Point(Loc.x, Loc.y + 1), cv::Point(Loc.x + 1, Loc.y) };
		pDeq.pop_back();
		for (Pt = &Ar[0]; Pt < Ar+ 4; ++Pt){
			if (INRANGE(0, Pt->y, img.rows) && INRANGE(0, Pt->x, img.cols) &&
				img.ptr<uchar>(Pt->y)[Pt->x] != 0 &&prevImg.ptr<uchar>(Pt->y)[Pt->x] == 0){
				SetRange(*Pt, ranges);
				pDeq.push_front(*Pt);
				deleteList.push_back(cvPoint(Pt->x, Pt->y));
				prevImg.ptr<uchar>(Pt->y)[Pt->x] = val;
				++pixelNum;
			}
		}
	}
	// DeleteCheck
	if (DeleteCheck(prevImg, deleteList, pixelNum, scale))
		return nullptr;

	// 라벨 내용 설정
	if (T == MAKE)
		return new Label(ranges, pixelNum,val, prevImg);
	else if (T==TRACKING)
		return new Label(ranges, pixelNum, val);
	return NULL;
}


