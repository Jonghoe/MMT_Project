#include"DetectingModel.h"
#include"MyFunction.h"

string IToS(const int info,const int len){
	//	받은 숫자 정보를 스트링으로 바꾼후 len에 맞게 0을 넣음
	string tmp,RetV;
	tmp = to_string(info);
	for (unsigned i = 0; i < len - tmp.length(); i++)
		RetV.append("0");
	RetV.append(tmp);
	return RetV;
}
string LabeltoString( Label& obj){
//	라벨의 위치(센터 점), 라벨ID , 마크ID 스틀링으로 변환
//  EX)     (530,450) /  202 /  3  = >  053045020203
//	x좌표 4자리 y좌표 3자리 라벨ID 3자리 마크 ID 2자리
	string RetV;
	cv::Point center = obj.mGetCenter();
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
	cv::Point MaxLoc;
	TRMatchingInform* Inform = (TRMatchingInform*)TMI;
	cv::Mat result;
	int rowGap = Inform->CompareIMG.rows < Inform->MarkIMG.rows;
	int colGap = Inform->CompareIMG.cols < Inform->MarkIMG.cols;
	if (rowGap<0|| colGap < 0)
		return 0;
	//템플릿 매칭
	cv::matchTemplate(Inform->CompareIMG, Inform->MarkIMG, result, cv::TM_CCOEFF_NORMED);
	cv::minMaxLoc(result, NULL, &Inform->MaxVal, NULL, &MaxLoc);
	return 0;
}
// 라벨링 하는 함수 
void FindLabel(const cv::Mat IMG, cv::Mat& PrevIMG, list<Label*>&VLabel,const size_t& LabelNum,const Type scale){
	// IMG : 현재 이미지 
	// PrevIMG : 이전 이미지 
	// VLabel : 라벨 리스트
	// LabelNum : 생성시킬 라벨 ID 값 
	// scale : 픽셀 수의 범위
	// 이미지의 픽셀 값이 255인 픽셀은 라벨 의심구역으로 블랍 수행(MakeLabel)

	for (int y = 10; y < IMG.rows; y+=10){
		const uchar* ptrM = IMG.ptr<uchar>(y);
		const uchar* ptrV = PrevIMG.ptr<uchar>(y);
		for (int x = 10; x < IMG.cols; x+=10){
			Label* tmp;
			if (ptrM[x] != 0 &&ptrV[x]==0 )
				if (MakeLabel(IMG, PrevIMG, &tmp, cv::Point(x, y), LabelNum, scale))
					VLabel.push_back(tmp);
		}
	}
}
bool DeleteCheck(cv::Mat&Label, vector<cv::Point>& con,const size_t PixelNum,const Type scale){
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
	
bool MakeLabel(const cv::Mat& Img,cv::Mat& PrevIMG, Label** ML, const cv::Point FL, const uchar Val,const Type scale){
#define INRANGE(S,E,V) ((S)<(V)&&(V)<(E))
#define PTRXY(IMG,X,Y) ((IMG).ptr<uchar>(Ar[i].(Y))[Ar[i].(X)] != 0)
	int PixelNum = 0;
	int Ranges[4] = { FL.x, FL.y, FL.y, FL.x };
	deque<cv::Point> PDeque;
	PDeque.push_front(FL);
	vector<cv::Point> DeleteList;		//	픽셀의 수가 범위 불충족시 그 부분을 지우기위한 컨테이너 
	DeleteList.reserve(ULIMIT);
	DeleteList.push_back(FL);
	while (!PDeque.empty()){
		int i;
		cv::Point Loc = PDeque.back();
		cv::Point Ar[4] = { cv::Point(Loc.x - 1, Loc.y), cv::Point(Loc.x, Loc.y - 1),
							cv::Point(Loc.x, Loc.y + 1), cv::Point(Loc.x + 1, Loc.y) };
		PDeque.pop_back();
		for (i = 0; i < 4; i++){
			if (INRANGE(0,Img.rows,Ar[i].y)&& INRANGE(0,Img.cols,Ar[i].x)	&&
				Img.ptr<uchar>(Ar[i].y)[Ar[i].x] != 0 && 
				PrevIMG.ptr<uchar>(Ar[i].y)[Ar[i].x] == 0){
				SetRange(Ar[i], Ranges);
				PDeque.push_front(Ar[i]);
				DeleteList.push_back(cvPoint(Ar[i].x, Ar[i].y));
				PrevIMG.ptr<uchar>(Ar[i].y)[Ar[i].x] = Val;
				++PixelNum;
			}
		}
	}
	// DeleteCheck
	if (DeleteCheck(PrevIMG, DeleteList, PixelNum, scale))
		return false;
	// 라벨 내용 설정
	(*ML) = new Label(Ranges,PixelNum,Val);
	return true;
}
