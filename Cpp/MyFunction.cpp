#include"DetectingModel.h"
#include"MyFunction.h"
inline bool inRange(int b, int v, int e){
	return b <= v && v <= e;
}
string IToS(const int info,const int len){
	//	���� ���� ������ ��Ʈ������ �ٲ��� len�� �°� 0�� ����
	string INFO;
	char buff[10];
	itoa(info, buff, 10);
	
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
	
