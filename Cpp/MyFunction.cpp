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
//라벨 생성
Label* MakeLabel(const Mat& img, Mat& prevImg, const Point FL, const uchar val, const Type scale, MorT T){
#define INRANGE(b,v,e) ((b)<(v))&&((v)<(e))

	int pixelNum = 0;
	int ranges[4] = { FL.x, FL.y, FL.y, FL.x };
	deque<Point> pDeq;
	pDeq.push_front(FL);
	vector<Point> deleteList;		//	픽셀의 수가 범위 불충족시 그 부분을 지우기위한 컨테이너 
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

	// 라벨 내용 설정
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
	//	받은 숫자 정보를 스트링으로 바꾼후 len에 맞게 0을 넣음
	string INFO;
	char buff[10];
	_itoa(info, buff, 10);
	
	for (unsigned i = 0; i < len - strlen(buff); i++)
		INFO.append("0");
	INFO.append(buff);
	return INFO;
}
string LabeltoString( Label& obj){
//	라벨의 위치(센터 점), 라벨ID , 마크ID 스틀링으로 변환
//  EX)     (530,450) /  202 /  3  = >  053045020203
//	x좌표 4자리 y좌표 3자리 라벨ID 3자리 마크 ID 2자리
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
	//템플릿 매칭하여 TMI의 마크정보와 일치하는 지 확인
	Point MaxLoc;
	TRMatchingInform* Inform = (TRMatchingInform*)TMI;
	Mat result;
	int rowGap = Inform->CompareIMG.rows - Inform->MarkIMG.rows;
	int colGap = Inform->CompareIMG.cols - Inform->MarkIMG.cols;
	if (rowGap<0|| colGap < 0)
		return 0;
	//템플릿 매칭
	matchTemplate(Inform->CompareIMG, Inform->MarkIMG, result, TM_CCOEFF_NORMED);
	minMaxLoc(result, NULL, &Inform->MaxVal, NULL, &MaxLoc);
	return 0;
}
// 라벨링 하는 함수 
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

