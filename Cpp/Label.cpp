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
	//-------------------	���� ��ó�� ����---------------------------------
	cv::threshold(Img, RevIMG, 100, 255, cv::THRESH_BINARY_INV);
	//----------------------------------------------------------------------
	//	������ ��Ÿ���� ����Ʈ�� ��ġ �˻�
	FindLabel(RevIMG, PrevIMG, temp, mvID, HIGH);
	
	if (temp.size() == 0)
		return 0;

	LocAngle = (*temp.begin())->mvCenter;
	cv::Point Loc(LocAngle.x - (xGap) / 2, LocAngle.y - (yGap) / 2);
	tempRadius = sqrt((pow(Loc.x, 2) + pow(Loc.y, 2)));

	//	������ ���ϱ� ���� ��ġ �������� Ȯ��
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
	//	���� �̹��� ���� ���� �ȼ� ���� 255�� �ȼ��� ���� ����
	//	���غ��� �ȼ��� ���� ������ ���� ����
	//	���غ��� ���� ��� �� ������ Ȯ��
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
	//	�� Loc�� ���� ������ �ƴ��� Ȯ��
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
	//	���� ���� ������ �ٽ� ��� �Ͽ� ���� �̵�( ������Ͽ� ��ġ �̵� )
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


