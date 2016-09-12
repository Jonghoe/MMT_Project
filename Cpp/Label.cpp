#include"Label.h"
#include"MyFunction.h"
ID Label::mGetMarkID()const{ return this->mvMarkID; }

Label::Label():Blob(){ mvTilt = 0; }
Label::Label(const Blob& b) : Blob(b){ mvTilt = 0; mvIsMoved = true; }
Label::Label(int* ranges, int pixelNum, int id, Mat& prevImg)
	:Blob(ranges,pixelNum,id),mvZone(prevImg(Range(mvLT.y, mvRB.y), Range(mvLT.x, mvRB.x)))
	{
	mvPrevImg = prevImg;
	mvTilt = 0;
#ifdef DEBUG_LV2
	cout << mvID <<"is Created"<< endl;
#endif
}
Label::Label(int* ranges, int pixelNum, int id) :Blob(ranges, pixelNum,id) {}
Label::~Label(){ ; }
void Label::mSetAngle(Mat& IMG){
	mvTilt = mvCalculateAngle(IMG);
}
double Label::mvCalculateAngle(Mat& IMG){
	list<Label*> temp;
	Point locAngle;
	Mat prevIMG = Mat(IMG.size(),IMG.type(),Scalar::all(0));
	Mat revIMG;
	double tempRadius, angle = 0;
	const double PI = 2 * asin(1);
	//-------------------	영상 전처리 구간---------------------------------
	threshold(IMG, revIMG, 100, 255, THRESH_BINARY_INV);
	//----------------------------------------------------------------------
	//	각도를 나타내줄 포인트의 위치 검색
	FindLabel(revIMG, prevIMG, temp, mvID, HIGH);
	
	if (temp.size() == 0)
		return 0;

	locAngle = (*temp.begin())->mvCenter;
	Point Loc(locAngle.x / 2, locAngle.y / 2);
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
	Mat dst;
	Point2f pt((float)(mvPrevImg.cols / 2), (float)(mvPrevImg.rows / 2));
	Mat r = getRotationMatrix2D(pt, -mvTilt, 1.0);
	warpAffine(mvPrevImg, mvZone, r, Size(mvPrevImg.cols, mvPrevImg.rows));
}
void Label::mSetMark(const MarkList& markList){
	mvMarkID = markList.mFindMark(mvPrevImg);
}
void Label::mSetChanged(bool ch){ mvIsMoved = ch; }
bool Label::mGetChanged(){ return mvIsMoved; }
bool Label::mModify(Mat& mimg){
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
	return false;
}

bool Label::mContainPoint(const Point& Loc)const{	
	//	점 Loc가 라벨의 안인지 아닌지 확인
	bool inX = this->mvLT.x <= Loc.x && Loc.x <= this->mvRB.x;
	bool inY = this->mvLT.y <= Loc.y && Loc.y <= this->mvRB.y;
	return inX&&inY;
}
void Label::mvMoving(Mat& mIMG){
	//	라벨의 범위 내에서 다시 블랍 하여 라벨을 이동( 재생성하여 위치 이동 )
	list<Label*>VLabel;
	mvClearIMG();
	for (int y = mvLT.y; y < mvRB.y; y++){
		uchar* ptrM = mIMG.ptr<uchar>(y);
		uchar* ptrV = mvPrevImg.ptr<uchar>(y);
		for (int x = mvLT.x; x < mvRB.x; x++)
			if (ptrM[x] != 0 && ptrV[x] == 0){
				Label* tmp = NULL;
				if ((tmp = MakeLabel(mIMG, mvPrevImg, Point(x, y), mvID, LOW,TRACKING)))
					VLabel.push_back(tmp);
			}
	}
	if (VLabel.size() != 0){
		mvCenter = (*VLabel.begin())->mvCenter;
		mvLT = (*VLabel.begin())->mvLT;
		mvRB = (*VLabel.begin())->mvRB;
	}
}

