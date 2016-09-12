#include"FrFillBlank.h"
#include"Label.h"
void FrFillBlank::mvStart(){
	cout << "FillBlank Start" << endl;
}
void FrFillBlank::mvStop(){
	cout << "FrFillBlank Stop" << endl;
}
void FrFillBlank::mvPlay(){
	Sleep(50);
	cout << " 메인 내용 " << mvPriority << endl;
}
