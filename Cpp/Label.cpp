#include"Label.h"
#include"MyFunction.h"
ID Label::mGetID()const{ return this->mvID; }
ID Label::mGetMarkID()const{ return this->mvMarkId; }
Label::Label(){ mvCenter = cv::Point(0, 0); mvPixelN = 0; mvTilt = 0; }
Label::Label(int* Ranges, int PixelNum, int id,
	MarkList* MarkList,cv::Mat& Img, cv::Mat& previmg){
	mvCenter = cv::Point((Ranges[0] + Ranges[3]) / 2, (Ranges[1] + Ranges[2]) / 2);
	mvRB = cv::Point(Ranges[3] + 1, Ranges[2] + 1);
	mvLT = cv::Point(Ranges[0], Ranges[1]);
	mvPixelN = PixelNum;
	mvID = id;
	mvMarkList = MarkList;
	mvPrevImg = previmg;
	mSetMark();
	if (mvMarkId != 0)
		mSetAngle(Img);
	else
		mvTilt = 0;
	cout << mvID << "is Created !!" << endl;
}
Label::Label(int* Ranges, int PixelNum, int Val){
	mvCenter = cv::Point((Ranges[0] + Ranges[3]) / 2, (Ranges[1] + Ranges[2]) / 2);
	mvRB = cv::Point(Ranges[3] + 1, Ranges[2] + 1);
	mvLT = cv::Point(Ranges[0], Ranges[1]);
	mvPixelN = PixelNum;
	mvID = Val;
}
Label::~Label(){}
double Label::mvCalculateAngle(cv::Mat& IMG){
	list<Label*> temp;
	cv::Point LocAngle;
	int yGap = mvRB.y - mvLT.y;
	int xGap = mvRB.x - mvLT.x;
	cv::Mat PrevIMG = cv::Mat(cv::Size(xGap, yGap), CV_8U, cv::Scalar::all(0));
	cv::Mat Img = IMG(cv::Range(mvLT.y, mvRB.y), cv::Range(mvLT.x, mvRB.x));
	cv::Mat RevIMG;
	double tempRadius, angle = 0;
	const double PI = 2 * asin(1);
	//-------------------	영상 전처리 구간---------------------------------
	cv::threshold(Img, RevIMG, 100, 255, cv::THRESH_BINARY_INV);
	//----------------------------------------------------------------------
	//	각도를 나타내줄 포인트의 위치 검색
	FindLabel(RevIMG, PrevIMG, temp, mvID, HIGH);
	
	if (temp.size() == 0)
		return 0;

	LocAngle = (*temp.begin())->mvCenter;
	cv::Point Loc(LocAngle.x - (xGap) / 2, LocAngle.y - (yGap) / 2);
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

void Label::mSetAngle(cv::Mat& IMG){
	mvTilt = mvCalculateAngle(IMG);
}

cv::Point Label::mGetCenter()const{	return this->mvCenter;}
bool Label::mModify(cv::Mat& mImg){
	//	라벨의 이미지 범위 안의 픽셀 값이 255인 픽셀의 수를 세어
	//	기준보다 픽셀의 수가 적으면 라벨을 삭제
	//	기준보다 많을 경우 라벨 움직임 확인
	size_t PixelNum = 0;
	for (int y = mvLT.y; y < mImg.rows&& y < mvRB.y; y++){
		uchar* ptrI = mImg.ptr<uchar>(y);
		for (int x = mvLT.x; x < mImg.cols&& x < mvRB.x; x++){
			if (ptrI[x] != 0)
				PixelNum++;
			if (PixelNum > DLIMIT){
				this->mvMoving(mImg);
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
void Label::mSetMark(){
	cv::Mat zone = mvPrevImg(cv::Range(mvLT.y, mvRB.y),
		cv::Range(mvLT.x,mvRB.x));
	mvMarkId =  mvMarkList->mFindMark(zone);
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
				if (MakeLabel(mIMG, mvPrevImg, &tmp, cv::Point(x, y), mvID, LOW))
					VLabel.push_back(tmp);
			}
	}
	if (VLabel.size() != 0){
		mvCenter = (*VLabel.begin())->mvCenter;
		mvLT = (*VLabel.begin())->mvLT;
		mvRB = (*VLabel.begin())->mvRB;
	}
}


