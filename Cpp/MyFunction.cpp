#include"Detecting_Model.h"
#include"MyFunction.h"

string I_To_S(const int info,const int len){
	//	받은 숫자 정보를 스트링으로 바꾼후 len에 맞게 0을 넣음
	string tmp,Ret_V;
	tmp = to_string(info);
	for (unsigned i = 0; i < len - tmp.length(); i++)
		Ret_V.append("0");
	Ret_V.append(tmp);
	return Ret_V;
}
string Label_to_String( Label& obj){
//	라벨의 위치(센터 점), 라벨ID , 마크ID 스틀링으로 변환
//  EX)     (530,450) /  202 /  3  = >  053045020203
//	x좌표 4자리 y좌표 3자리 라벨ID 3자리 마크 ID 2자리
	string Ret_V;
	cv::Point center = obj.mGetCenter();
	ID Label_ID = obj.mGetID();
	ID Mark_ID = obj.mGetMarkID();
	Ret_V.append(I_To_S(center.x, 4));
	Ret_V.append(I_To_S(center.y, 3));
	Ret_V.append(I_To_S(Label_ID, 3));
	Ret_V.append(I_To_S(Mark_ID, 2));
	return Ret_V;
}


unsigned WINAPI Find_Mark(void* TMI){
	//TMI : Thread Info Struct
	//템플릿 매칭하여 TMI의 마크정보와 일치하는 지 확인
	cv::Point MaxLoc;
	TR_Matching_Inform* Inform = (TR_Matching_Inform*)TMI;
	cv::Mat result;
	if (Inform->CompareIMG.rows < Inform->MarkIMG.rows
		|| Inform->CompareIMG.cols < Inform->MarkIMG.cols)
		return 0;
	int COL = Inform->CompareIMG.cols - Inform->MarkIMG.cols + 1;
	int ROW = Inform->CompareIMG.rows - Inform->MarkIMG.rows + 1;
	cv::Mat(cv::Size(COL,ROW ), CV_32FC1);
	//템플릿 매칭
	cv::matchTemplate(Inform->CompareIMG, Inform->MarkIMG, result, cv::TM_CCOEFF_NORMED);
	cv::minMaxLoc(result, NULL, &Inform->MaxVal, NULL, &MaxLoc);
	return 0;
}
// 라벨링 하는 함수 
void Find_Label(const cv::Mat IMG, cv::Mat& Prev_IMG, list<Label*>&V_Label,const size_t& LabelNum,const Type scale){
	// IMG : 현재 이미지 
	// Prev_IMG : 이전 이미지 
	// V_Label : 라벨 리스트
	// LabelNum : 생성시킬 라벨 ID 값 
	// scale : 픽셀 수의 범위
	// 이미지의 픽셀 값이 255인 픽셀은 라벨 의심구역으로 블랍 수행(Make_Label)

	for (int y = 10; y < IMG.rows; y+=10){
		const uchar* ptrM = IMG.ptr<uchar>(y);
		const uchar* ptrV = Prev_IMG.ptr<uchar>(y);
		for (int x = 10; x < IMG.cols; x+=10){
			Label* tmp;
			if (ptrM[x] != 0 &&ptrV[x]==0 )
				if (Make_Label(IMG, Prev_IMG, &tmp, cv::Point(x, y), LabelNum, scale))
					V_Label.push_back(tmp);
		}
	}
}
bool Delete_Check(cv::Mat&Label, vector<cv::Point>& con,const size_t Pixel_Num,const Type scale){
	switch (scale){
	case LOW:
		if (Pixel_Num <= U_LIMIT){
			for (unsigned i = 0; i < con.size(); i++)
				Label.ptr<uchar>(con[i].y)[con[i].x] = DELETE_V;
			return true;
		}
		break;
	case MIDDLE:
		if (D_LIMIT <= Pixel_Num&&Pixel_Num <= U_LIMIT){
			for (unsigned i = 0; i < con.size(); i++)
				Label.ptr<uchar>(con[i].y)[con[i].x] = DELETE_V;
			return true;
		}
		break;
	case HIGH:
		if (D_LIMIT <= Pixel_Num){
			for (unsigned i = 0; i < con.size(); i++)
				Label.ptr<uchar>(con[i].y)[con[i].x] = DELETE_V;
			return true;
		}
		break;
	}
	return false;
}
	
bool Make_Label(const cv::Mat& IMG,cv::Mat& Prev_IMG, Label** ML, const cv::Point F_L, const uchar Val,const Type scale){
#define IN_RANGE(S,E,V) ((S)<(V)&&(V)<(E))

	int Pixel_Num = 0;
	int Ranges[4] = { F_L.x, F_L.y, F_L.y, F_L.x };
	deque<cv::Point> P_Deque;
	P_Deque.push_front(F_L);
	vector<cv::Point> Delete_List;		//	픽셀의 수가 범위 불충족시 그 부분을 지우기위한 컨테이너 
	Delete_List.reserve(U_LIMIT);
	Delete_List.push_back(F_L);
	while (!P_Deque.empty()){
		int i;
		cv::Point Loc = P_Deque.back();
		cv::Point Ar[4] = { cv::Point(Loc.x - 1, Loc.y), cv::Point(Loc.x, Loc.y - 1),
							cv::Point(Loc.x, Loc.y + 1), cv::Point(Loc.x + 1, Loc.y) };
		P_Deque.pop_back();
		if (Loc.x*Loc.y == 0)
			cout << "A";
		for (i = 0; i < 4; i++){
			if (IN_RANGE(0,IMG.rows,Ar[i].y)&& IN_RANGE(0,IMG.cols,Ar[i].x)	&&
				IMG.ptr<uchar>(Ar[i].y)[Ar[i].x] != 0 && 
				Prev_IMG.ptr<uchar>(Ar[i].y)[Ar[i].x] == 0){
				SetRange(Ar[i], Ranges);
				P_Deque.push_front(Ar[i]);
				Delete_List.push_back(cvPoint(Ar[i].x, Ar[i].y));
				Prev_IMG.ptr<uchar>(Ar[i].y)[Ar[i].x] = Val;
				++Pixel_Num;
			}
		}
	}
	// Delete_Check
	if (Delete_Check(Prev_IMG, Delete_List, Pixel_Num, scale))
		return false;
	// 라벨 내용 설정
	(*ML) = new Label(Ranges,Pixel_Num,Val);
	return true;
}
