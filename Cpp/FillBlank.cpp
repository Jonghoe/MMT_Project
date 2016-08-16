#include"FrameFillBlank.h"
#include"Label.h"
unsigned WINAPI FillBlank::mainPerform(void* fr){
	Frame* info = (Frame*)fr;
	int i = 0;
	while (!info->mGetEnd()){
		if (info->mGetStop())
			;
		else if (i++ % 3 == 0)
			cout << info->mGetLID()<<info->mGetMID()<<info->mGetPriority()<< endl;

		Sleep(100);
	}
	return 0;
}
void FillBlank::mAction(){
	_beginthreadex(NULL, NULL, mainPerform, this, NULL, NULL);
}
void FillBlank::mvUpdate(void* info){
	mvHdAr[mvHdSz++] = ((Label*)info)->mGetCenter();
}
