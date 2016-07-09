#include"Label.h"
#include"MyFunction.h"
ID Label::mGetID()const{ return this->mvID; }
ID Label::mGetMarkID()const{ return this->mvMarkId; }
Label::Label(){ mvCenter = cv::Point(0, 0); mvPixel_N = 0; mvTilt = 0; }
Label::Label(int* Ranges, int Pixel_Num, int id,
	MarkList* _MarkList,cv::Mat& Img, cv::Mat& prev_img){
	mvCenter = cv::Point((Ranges[0] + Ranges[3]) / 2, (Ranges[1] + Ranges[2]) / 2);
	mvRB = cv::Point(Ranges[3] + 1, Ranges[2] + 1);
	mvLT = cv::Point(Ranges[0], Ranges[1]);
	mvPixel_N = Pixel_Num;
	mvID = id;
	mvMarkList = _MarkList;
	mvPrev_Img = prev_img;
	mSetMark();
	if (mvMarkId != 0)
		mSet_Angle(Img);
	else
		mvTilt = 0;
}
Label::Label(int* Ranges, int Pixel_Num, int Val){
	mvCenter = cv::Point((Ranges[0] + Ranges[3]) / 2, (Ranges[1] + Ranges[2]) / 2);
	mvRB = cv::Point(Ranges[3] + 1, Ranges[2] + 1);
	mvLT = cv::Point(Ranges[0], Ranges[1]);
	mvPixel_N = Pixel_Num;
	mvID = Val;
}
Label::~Label(){}
double Label::mvCalculate_Angle(cv::Mat& IMG){
	list<Label*> temp;
	cv::Point Loc_Angle;
	cv::Mat Prev_IMG = cv::Mat(cv::Size(mvRB.x - mvLT.x, mvRB.y - mvLT.y), CV_8U, cv::Scalar::all(0));
	cv::Mat Img = IMG(cv::Range(mvLT.y, mvRB.y), cv::Range(mvLT.x, mvRB.x));
	cv::Mat Rev_IMG;
	double temp_Radius, angle = 0;
	const double PI = 2 * asin(1);
	//-------------------	영상 전처리 구간---------------------------------
	cv::threshold(Img, Rev_IMG, 100, 255, cv::THRESH_BINARY_INV);
	//----------------------------------------------------------------------
	//	각도를 나타내줄 포인트의 위치 검색
	Find_Label(Rev_IMG, Prev_IMG, temp, mvID, HIGH);
	
	if (temp.size() == 0)
		return 0;

	Loc_Angle = (*temp.begin())->mvCenter;
	cv::Point Loc(Loc_Angle.x - (mvRB.x - mvLT.x) / 2, Loc_Angle.y - (mvRB.y - mvLT.y) / 2);
	temp_Radius = sqrt((pow(Loc.x, 2) + pow(Loc.y, 2)));

	//	각도를 구하기 위한 위치 구역별로 확인
	if (Loc.x > 0)
		angle = asin(Loc.y / temp_Radius);
	else
		if (Loc.y >= 0)
			angle = PI - asin(Loc.y / temp_Radius);
		else
			angle = -PI - asin(Loc.y / temp_Radius);
	for (auto iter = temp.begin(); iter != temp.end(); iter++)
		delete *iter;
	return angle;
}
void Label::mSet_Angle(cv::Mat& IMG){
	mvTilt = mvCalculate_Angle(IMG);
}

cv::Point Label::mGetCenter()const{	return this->mvCenter;}
bool Label::mModify(cv::Mat& mImg){
	//	라벨의 이미지 범위 안의 픽셀 값이 255인 픽셀의 수를 세어
	//	기준보다 픽셀의 수가 적으면 라벨을 삭제
	//	기준보다 많을 경우 라벨 움직임 확인
	size_t Pixel_Num = 0;
	for (size_t y = mvLT.y; y < (size_t)mImg.rows&& y < (size_t)mvRB.y; y++){
		uchar* ptrI = mImg.ptr<uchar>(y);
		for (size_t x = mvLT.x; x < (size_t)mImg.cols&& x < (size_t)mvRB.x; x++){
			if (ptrI[x] != 0)
				Pixel_Num++;
			if (Pixel_Num > D_LIMIT){
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
	bool Is_In = this->mvLT.x <= Loc.x && this->mvRB.x <= Loc.x ?
		(this->mvLT.y <= Loc.y&& this->mvRB.y <= Loc.y ? true : false ): false;
	return Is_In;
}
void Label::mSetMark(){
	cv::Mat zone = mvPrev_Img(cv::Range(mvLT.y, mvRB.y),
		cv::Range(mvLT.x,mvRB.x));
	mvMarkId =  mvMarkList->mFindMark(zone);
}
void Label::mvMoving(cv::Mat& mIMG){
	//	라벨의 범위 내에서 다시 블랍 하여 라벨을 이동( 재생성하여 위치 이동 )
	list<Label*>V_Label;
	(this)->mClearIMG();
	for (size_t y = (size_t)mvLT.y; y < (size_t)mvRB.y; y++){
		uchar* ptrM = mIMG.ptr<uchar>(y);
		uchar* ptrV = mvPrev_Img.ptr<uchar>(y);
		for (size_t x = (size_t)mvLT.x; x < (size_t)mvRB.x; x++)
			if (ptrM[x] != 0 && ptrV[x] == 0){
				Label* tmp = NULL;
				if (Make_Label(mIMG, mvPrev_Img, &tmp, cv::Point(x, y), mvID, LOW))
					V_Label.push_back(tmp);
			}
	}
	if (V_Label.size() != 0){
		mvCenter = (*V_Label.begin())->mvCenter;
		mvLT = (*V_Label.begin())->mvLT;
		mvRB = (*V_Label.begin())->mvRB;
	}
}


