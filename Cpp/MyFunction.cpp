#include"DetectingModel.h"
#include"MyFunction.h"
using namespace cv;

inline Mat RotationMat2(double d){
	Mat rot(2, 2, CV_32FC1);
	rot.at<double>(0, 0) = cos(d);
	rot.at<double>(0, 1) = sin(d);
	rot.at<double>(1, 0) = -sin(d);
	rot.at<double>(1, 1) = cos(d);
	return rot;
}
Point RotatePoint(double d, Point& p){
	Mat RM = RotationMat2(d);
	return Point((int)(p.x * RM.ptr<double>(0)[0] + p.y*RM.ptr<double>(0)[1]),
			(int)(p.x * RM.ptr<double>(1)[0] + p.y*RM.ptr<double>(1)[1]));
}
void Mat2Point(const Mat&m, Point& p){
	p.x = m.at<int>(0, 0);
	p.y = m.at<int>(0, 1);
}
void Point2Mat(const Point& p, Mat& m){
	m.release();
	m.create(2, 1, CV_32SC1);
	m.at<int>(0, 0) = p.x;
	m.at<int>(0, 1) = p.y;
}
void ModifyLabel(Label* l,const Blob& b){
	l->mChange(b);
	l->mSetChanged(true);
}
//�� ����
Label* MakeLabel(const Mat& img, Mat& prevImg, const Point FL, const uchar val, const Type scale, MorT T){
#define INRANGE(b,v,e) ((b)<(v))&&((v)<(e))

	int pixelNum = 0;
	int ranges[4] = { FL.x, FL.y, FL.y, FL.x };
	deque<Point> pDeq;
	pDeq.push_front(FL);
	vector<Point> deleteList;		//	�ȼ��� ���� ���� �������� �� �κ��� ��������� �����̳� 
	deleteList.reserve(ULIMIT);
	deleteList.push_back(FL);
	while (!pDeq.empty()){
		Point* Pt;
		Point Loc = pDeq.back();
		Point Ar[4] = { Point(Loc.x - 1, Loc.y), Point(Loc.x, Loc.y - 1),
			Point(Loc.x, Loc.y + 1), Point(Loc.x + 1, Loc.y) };
		pDeq.pop_back();
		for (Pt = &Ar[0]; Pt < Ar + 4; ++Pt){
			if (INRANGE(0, Pt->y, img.rows) && INRANGE(0, Pt->x, img.cols) &&
				img.ptr<uchar>(Pt->y)[Pt->x] != 0 && prevImg.ptr<uchar>(Pt->y)[Pt->x] == 0){
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

	// �� ���� ����
	if (T == MAKE)
		return new Label(ranges, pixelNum, val, prevImg);
	else if (T == TRACKING)
		return new Label(ranges, pixelNum, val);
	return NULL;
}


inline bool inRange(int b, int v, int e){
	return b <= v && v <= e;
}
string IToS(const int info,const int len){
	//	���� ���� ������ ��Ʈ������ �ٲ��� len�� �°� 0�� ����
	string INFO;
	char buff[10];
	_itoa(info, buff, 10);
	
	for (unsigned i = 0; i < len - strlen(buff); i++)
		INFO.append("0");
	INFO.append(buff);
	return INFO;
}
string LabeltoString( Label& obj){
//	���� ��ġ(���� ��), ��ID , ��ũID ��Ʋ������ ��ȯ
//  EX)     (530,450) /  202 /  3  = >  053045020203
//	x��ǥ 4�ڸ� y��ǥ 3�ڸ� ��ID 3�ڸ� ��ũ ID 2�ڸ�
	string RetV;
	Point center = obj.mGetCenter();
	ID LabelID = obj.mGetID();
	ID MarkID = obj.mGetMarkID();
	RetV.append(IToS(center.x, 4));
	RetV.append(IToS(center.y, 3));
	RetV.append(IToS(LabelID, 3));
	RetV.append(IToS(MarkID, 2));
	return RetV;
}


unsigned WINAPI FindMark(void* TMI){
	//TMI : Thread Info Struct
	//���ø� ��Ī�Ͽ� TMI�� ��ũ������ ��ġ�ϴ� �� Ȯ��
	Point MaxLoc;
	TRMatchingInform* Inform = (TRMatchingInform*)TMI;
	Mat result;
	int rowGap = Inform->CompareIMG.rows - Inform->MarkIMG.rows;
	int colGap = Inform->CompareIMG.cols - Inform->MarkIMG.cols;
	if (rowGap<0|| colGap < 0)
		return 0;
	//���ø� ��Ī
	matchTemplate(Inform->CompareIMG, Inform->MarkIMG, result, TM_CCOEFF_NORMED);
	minMaxLoc(result, NULL, &Inform->MaxVal, NULL, &MaxLoc);
	return 0;
}
// �󺧸� �ϴ� �Լ� 
void FindLabel(const Mat IMG, Mat& PrevIMG, list<Label*>&VLabel,const size_t& LabelNum,const Type scale){

	for (int y = 10; y < IMG.rows; y+=10){
		const uchar* ptrM = IMG.ptr<uchar>(y);
		const uchar* ptrV = PrevIMG.ptr<uchar>(y);
		for (int x = 10; x < IMG.cols; x+=10){
			Label* tmp;
			if (ptrM[x] != 0 &&ptrV[x]==0 )
				if (tmp = MakeLabel(IMG, PrevIMG,  Point(x, y), LabelNum, scale,MAKE))
					VLabel.push_back(tmp);
		}
	}
}
bool DeleteCheck(Mat&Label, vector<Point>& con,const size_t PixelNum,const Type scale){
	switch (scale){
	case LOW:
		if (PixelNum <= ULIMIT){
			for (unsigned i = 0; i < con.size(); i++)
				Label.ptr<uchar>(con[i].y)[con[i].x] = DELETEV;
			return true;
		}
		break;
	case MIDDLE:
		if (DLIMIT <= PixelNum&&PixelNum <= ULIMIT){
			for (unsigned i = 0; i < con.size(); i++)
				Label.ptr<uchar>(con[i].y)[con[i].x] = DELETEV;
			return true;
		}
		break;
	case HIGH:
		if (DLIMIT <= PixelNum){
			for (unsigned i = 0; i < con.size(); i++)
				Label.ptr<uchar>(con[i].y)[con[i].x] = DELETEV;
			return true;
		}
		break;
	}
	return false;
}

