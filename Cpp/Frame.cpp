#include"Frame.h"
unsigned WINAPI Frame::mainPerform(void* fr){
	Frame* info = (Frame*)fr;
	int i = 0;
	while (!info->mGetEnd()){
		if (info->mGetStop())
			;
		else 
			cout << "This Frame is Base Frame" << endl;
		Sleep(100);
	}
	return 0;
}

void Frame::mStop(){
	mvStop = true;
}
