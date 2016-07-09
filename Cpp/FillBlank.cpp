#include"Frame_FillBlank.h"
#include"Label.h"
void FillBlank::mvAction(){
	cout << 5 << endl;
}
void FillBlank::mvUpdate(void* info){
	mvHdAr[mvHdSz++] = ((Label*)info)->mGetCenter();
}
