#include"FrameFillBlank.h"
#include"Label.h"
void FillBlank::mAction(){
	cout << 5 << endl;
}
void FillBlank::mvUpdate(void* info){
	mvHdAr[mvHdSz++] = ((Label*)info)->mGetCenter();
}
