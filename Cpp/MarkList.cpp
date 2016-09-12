#include "MarkList.h"
#include "MyFunction.h"
#include <process.h>
#include <Windows.h>
using namespace cv;
MarkList* MarkList::Alive = nullptr;
MarkList::MarkList():mvSize(0){}
void MarkList::mAddMark( Mat& Mark){
	if (Mark.rows * Mark.cols == 0){
		cout << "Wrong Input" << endl;
		return;
	}
	cvtColor(Mark, Mark, COLOR_BGR2GRAY);
	mvList.push_back(Mark);
	mvSize++;
}
MarkList* MarkList::mMakeMarkList(){
	if (Alive == nullptr)
		Alive = new MarkList();
	return Alive;
}
void MarkList::mDeleteMarkList(){
	if (Alive == nullptr)
		return;
	delete this;
}			

void MarkList::mDeleteMark(const Mat&Mark){
	size_t i = 0;
	auto iter = mvList.begin();
	i = mFindMark(Mark);
	mvList.erase((iter + i));
}
int MarkList::mFindMark(const Mat& src)const{
	HANDLE* Thread = new HANDLE[mvSize];
	TRMatchingInform* Inform = new TRMatchingInform[mvSize];
	Mat Tmp(src);
	double MaxVal;
	int Index = 0;
	
	if (mvList.size() == 0)
		return 0;
	//쓰레드 핸들러에게 넘겨줄 정보저장
	for (size_t i = 0; i < mvSize; i++){
		//비교할 영상의 크기를 변경
		resize(src, Tmp, mvList[i].size());
		Inform[i].CompareIMG = Tmp;
		Inform[i].MarkIMG = mvList[i];
		Inform[i].MaxVal = 0;
	}
	//쓰레드 실행
	for (size_t i = 0; i < mvSize; i++)
		Thread[i] = (HANDLE)_beginthreadex(NULL, NULL, FindMark, &Inform[i], NULL, NULL);
	//쓰레드 대기
	WaitForMultipleObjects(mvSize, Thread, TRUE, INFINITE);
	//가장 최대 값을 찾아서 저장
	MaxVal = Inform[0].MaxVal;
	for (size_t i = 1; i < mvSize; i++){
		if (MaxVal < Inform[i].MaxVal && Inform[i].MaxVal >0.8){
			MaxVal = Inform[i].MaxVal;
			Index = i;
		}
	}
	//가장 최대 값을 가지는 인덱스를 리턴;
	delete[] Thread;
	delete[] Inform;
	return Index;
}