#include"DetectingModel.h"
#include"MyFunction.h"

string IToS(const int info,const int len){
	//	���� ���� ������ ��Ʈ������ �ٲ��� len�� �°� 0�� ����
	string tmp,RetV;
	tmp = to_string(info);
	for (unsigned i = 0; i < len - tmp.length(); i++)
		RetV.append("0");
	RetV.append(tmp);
	return RetV;
}
string LabeltoString( Label& obj){
//	���� ��ġ(���� ��), ��ID , ��ũID ��Ʋ������ ��ȯ
//  EX)     (530,450) /  202 /  3  = >  053045020203
//	x��ǥ 4�ڸ� y��ǥ 3�ڸ� ��ID 3�ڸ� ��ũ ID 2�ڸ�
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
	//���ø� ��Ī�Ͽ� TMI�� ��ũ������ ��ġ�ϴ� �� Ȯ��
	cv::Point MaxLoc;
	TRMatchingInform* Inform = (TRMatchingInform*)TMI;
	cv::Mat result;
	int rowGap = Inform->CompareIMG.rows < Inform->MarkIMG.rows;
	int colGap = Inform->CompareIMG.cols < Inform->MarkIMG.cols;
	if (rowGap<0|| colGap < 0)
		return 0;
	//���ø� ��Ī
	cv::matchTemplate(Inform->CompareIMG, Inform->MarkIMG, result, cv::TM_CCOEFF_NORMED);
	cv::minMaxLoc(result, NULL, &Inform->MaxVal, NULL, &MaxLoc);
	return 0;
}
// �󺧸� �ϴ� �Լ� 
void FindLabel(const cv::Mat IMG, cv::Mat& PrevIMG, list<Label*>&VLabel,const size_t& LabelNum,const Type scale){
	// IMG : ���� �̹��� 
	// PrevIMG : ���� �̹��� 
	// VLabel : �� ����Ʈ
	// LabelNum : ������ų �� ID �� 
	// scale : �ȼ� ���� ����
	// �̹����� �ȼ� ���� 255�� �ȼ��� �� �ǽɱ������� ��� ����(MakeLabel)

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
	vector<cv::Point> DeleteList;		//	�ȼ��� ���� ���� �������� �� �κ��� ��������� �����̳� 
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
	// �� ���� ����
	(*ML) = new Label(Ranges,PixelNum,Val);
	return true;
}
