#pragma once
#include"D_Label.h"
using namespace std;
class D_Acter{
private:
	Mat Img;
	int mvLabelNum;
	vector<D_Label*> mvLb;
	void CreateLb(){
		int i;
		Point P;
		for (i = 0; i < mvLabelNum&&!(mvLb[i] == NULL); ++i);
		cout << "시작위치 입력 : ";
		cin >> P.x >> P.y;
		if (i < mvLabelNum)
			mvLb[i] = new D_Label(Img,i,P);
		else{
			mvLb.push_back(new D_Label(Img,mvLabelNum,P));
			++mvLabelNum;
		}
	}
	void SelectLb()const{
		int S;
		bool EXIT=true;
		do{
			cout << "선택 번호 : ";
			cin >> S;
			if (S >= mvLb.size() || mvLb[S] == NULL)
				cerr << "해당 라벨이 존재 하지 않습니다." << endl;
			else
				mvLb[S]->SetAction()==1 ? EXIT = true:EXIT = false;
			
		} while (EXIT);
	}
	void Draw(){
		imshow("Debug", Img);
	}
	void Work(){
		for (int i = 0; i < mvLabelNum; ++i){
			D_Label* tmp = NULL;
			if (mvLb[i] !=NULL)
				mvLb[i]->Action()==-1 ? tmp = mvLb[i] : tmp = NULL;
			if (tmp != NULL){
				mvLb[i] = NULL;
				delete tmp;
			}
		}
	}
public:
	D_Acter(Mat& img) :mvLb(), mvLabelNum(0), Img(img){ mvLb.reserve(20); }
	void Do(){
		int S;
		do{
			Img -= Img;
			S = waitKey(100);
			if (S == 'c')
				CreateLb();

			else if (S == 's')
				SelectLb();
			else
				Work();
			Draw();
		} while (S != 27);
	}
	
};