#include"Detecting_Model.h"
#include"MyFunction.h"

string I_To_S(const int info,const int len){
	//	���� ���� ������ ��Ʈ������ �ٲ��� len�� �°� 0�� ����
	string tmp,Ret_V;
	tmp = to_string(info);
	for (unsigned i = 0; i < len - tmp.length(); i++)
		Ret_V.append("0");
	Ret_V.append(tmp);
	return Ret_V;
}
string Label_to_String( Label& obj){
//	���� ��ġ(���� ��), ��ID , ��ũID ��Ʋ������ ��ȯ
//  EX)     (530,450) /  202 /  3  = >  053045020203
//	x��ǥ 4�ڸ� y��ǥ 3�ڸ� ��ID 3�ڸ� ��ũ ID 2�ڸ�
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
	//���ø� ��Ī�Ͽ� TMI�� ��ũ������ ��ġ�ϴ� �� Ȯ��
	cv::Point MaxLoc;
	TR_Matching_Inform* Inform = (TR_Matching_Inform*)TMI;
	cv::Mat result;
	if (Inform->CompareIMG.rows < Inform->MarkIMG.rows
		|| Inform->CompareIMG.cols < Inform->MarkIMG.cols)
		return 0;
	int COL = Inform->CompareIMG.cols - Inform->MarkIMG.cols + 1;
	int ROW = Inform->CompareIMG.rows - Inform->MarkIMG.rows + 1;
	cv::Mat(cv::Size(COL,ROW ), CV_32FC1);
	//���ø� ��Ī
	cv::matchTemplate(Inform->CompareIMG, Inform->MarkIMG, result, cv::TM_CCOEFF_NORMED);
	cv::minMaxLoc(result, NULL, &Inform->MaxVal, NULL, &MaxLoc);
	return 0;
}
// �󺧸� �ϴ� �Լ� 
void Find_Label(const cv::Mat IMG, cv::Mat& Prev_IMG, list<Label*>&V_Label,const size_t& LabelNum,const Type scale){
	// IMG : ���� �̹��� 
	// Prev_IMG : ���� �̹��� 
	// V_Label : �� ����Ʈ
	// LabelNum : ������ų �� ID �� 
	// scale : �ȼ� ���� ����
	// �̹����� �ȼ� ���� 255�� �ȼ��� �� �ǽɱ������� ��� ����(Make_Label)

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
	vector<cv::Point> Delete_List;		//	�ȼ��� ���� ���� �������� �� �κ��� ��������� �����̳� 
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
	// �� ���� ����
	(*ML) = new Label(Ranges,Pixel_Num,Val);
	return true;
}
