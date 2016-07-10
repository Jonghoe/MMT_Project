#include"Label.h"
#include"MyFunction.h"
ID Label::mGetID()const{ return this->mvID; }
ID Label::mGetMarkID()const{ return this->mvMarkID; }
Label::Label(){ mvCenter = cv::Point(0, 0); mvPixelN = 0; mvTilt = 0; }
Label::Label(int* ranges, int pixelNum, int id,cv::Mat& prevImg){
	mvCenter = cv::Point((ranges[0] + ranges[3]) / 2, (ranges[1] + ranges[2]) / 2);
	mvRB = cv::Point(ranges[3] + 1, ranges[2] + 1);
	mvLT = cv::Point(ranges[0], ranges[1]);
	mvPixelN = pixelNum;
	mvID = id;
	mvPrevImg = prevImg;
	mvTilt = 0;
	cout << mvID << "is Created !!" << endl;
}
Label::Label(int* ranges, int pixelNum, int id){
	mvCenter = cv::Point((ranges[0] + ranges[3]) / 2, (ranges[1] + ranges[2]) / 2);
	mvRB = cv::Point(ranges[3] + 1, ranges[2] + 1);
	mvLT = cv::Point(ranges[0], ranges[1]);
	mvPixelN = pixelNum;
	mvID = id;
}
Label::~Label(){}
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
	//-------------------	���� ��ó�� ����---------------------------------
	cv::threshold(img, revIMG, 100, 255, cv::THRESH_BINARY_INV);
	//----------------------------------------------------------------------
	//	������ ��Ÿ���� ����Ʈ�� ��ġ �˻�
	FindLabel(revIMG, prevIMG, temp, mvID, HIGH);
	
	if (temp.size() == 0)
		return 0;

	locAngle = (*temp.begin())->mvCenter;
	cv::Point Loc(locAngle.x - (xGap) / 2, locAngle.y - (yGap) / 2);
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
bool Label::mModify(cv::Mat& mimg){
	//	���� �̹��� ���� ���� �ȼ� ���� 255�� �ȼ��� ���� ����
	//	���غ��� �ȼ��� ���� ������ ���� ����
	//	���غ��� ���� ��� �� ������ Ȯ��
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
	//	�� Loc�� ���� ������ �ƴ��� Ȯ��
	bool inX = this->mvLT.x <= Loc.x && Loc.x <= this->mvRB.x;
	bool inY = this->mvLT.y <= Loc.y && Loc.y <= this->mvRB.y;
	return inX&&inY;
}
int Label::mSetMark(const MarkList& markList){
	cv::Mat zone = mvPrevImg(cv::Range(mvLT.y, mvRB.y),
		cv::Range(mvLT.x,mvRB.x));
	return mvMarkID =  markList.mFindMark(zone);
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
				if (MakeLabel(mIMG, mvPrevImg, &tmp, cv::Point(x, y), mvID, LOW,TRACKING))
					VLabel.push_back(tmp);
			}
	}
	if (VLabel.size() != 0){
		mvCenter = (*VLabel.begin())->mvCenter;
		mvLT = (*VLabel.begin())->mvLT;
		mvRB = (*VLabel.begin())->mvRB;
	}
}
bool Label::MakeLabel(const cv::Mat& img, cv::Mat& prevImg, Label** ML, const cv::Point FL, const uchar val, const Type scale,Act T){
#define INRANGE(b,v,e) ((b)<(v))&&((v)<(e))

	int pixelNum = 0;
	int ranges[4] = { FL.x, FL.y, FL.y, FL.x };
	deque<cv::Point> pDeq;
	pDeq.push_front(FL);
	vector<cv::Point> deleteList;		//	�ȼ��� ���� ���� �������� �� �κ��� ��������� �����̳� 
	deleteList.reserve(ULIMIT);
	deleteList.push_back(FL);
	while (!pDeq.empty()){
		int i;
		cv::Point Loc = pDeq.back();
		cv::Point Ar[4] = { cv::Point(Loc.x - 1, Loc.y), cv::Point(Loc.x, Loc.y - 1),
			cv::Point(Loc.x, Loc.y + 1), cv::Point(Loc.x + 1, Loc.y) };
		pDeq.pop_back();
		for (i = 0; i < 4; i++){
			if (INRANGE(0, Ar[i].y, img.rows) && INRANGE(0, Ar[i].x, img.cols) &&
				img.ptr<uchar>(Ar[i].y)[Ar[i].x] != 0 &&
				prevImg.ptr<uchar>(Ar[i].y)[Ar[i].x] == 0){
				SetRange(Ar[i], ranges);
				pDeq.push_front(Ar[i]);
				deleteList.push_back(cvPoint(Ar[i].x, Ar[i].y));
				prevImg.ptr<uchar>(Ar[i].y)[Ar[i].x] = val;
				++pixelNum;
			}
		}
	}
	// DeleteCheck
	if (DeleteCheck(prevImg, deleteList, pixelNum, scale))
		return false;
	// �� ���� ����
	if (T == MAKE)
		(*ML) = new Label(ranges, pixelNum,val, prevImg);
	else if (T==TRACKING)
		(*ML) = new Label(ranges, pixelNum, val);
	return true;
}


