#include"Frame_FillBlank.h"
#include"Label.h"
void FillBlank::mvAction(){
	cout << 5 << endl;
}
void FillBlank::mvUpdate(void* info){
	if (!mvIsIn(((Label*)info)->mGetCenter()))
		return;
	mvHdAr[mvHdSz++] = ((Label*)info)->mGetCenter();
}
