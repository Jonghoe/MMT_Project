#include"DetectingModel.h"
#include"MyFunction.h"
inline bool inRange(int b, int v, int e){
	return b <= v && v <= e;
}
string IToS(const int info,const int len){
	//	받은 숫자 정보를 스트링으로 바꾼후 len에 맞게 0을 넣음
	string INFO;
	char buff[10];
	itoa(info, buff, 10);
	
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

	for (int y = 10; y < IMG.rows; y+=10){
		const uchar* ptrM = IMG.ptr<uchar>(y);
		const uchar* ptrV = PrevIMG.ptr<uchar>(y);
		for (int x = 10; x < IMG.cols; x+=10){
			Label* tmp;
			if (ptrM[x] != 0 &&ptrV[x]==0 )
				if (Label::MakeLabel(IMG, PrevIMG, &tmp, cv::Point(x, y), LabelNum, scale,MAKE))
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
	
