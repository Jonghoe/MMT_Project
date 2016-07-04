#include "MarkList.h"
#include "MyFunction.h"
#include <process.h>
#include <Windows.h>
MarkList::MarkList():mvSize(0){}
void MarkList::mAddMark( cv::Mat& _Mark){
	cv::cvtColor(_Mark, _Mark, cv::COLOR_BGR2GRAY);
	mvList.push_back(_Mark);
	mvSize++;
}
void MarkList::mDeleteMark(const cv::Mat&_Mark){
	size_t i = 0;
	auto iter = mvList.begin();
	i = mFindMark(_Mark);
	mvList.erase((iter + i));
}
int MarkList::mFindMark(const cv::Mat& src){
	HANDLE* Thread = new HANDLE[mvSize];
	TR_Matching_Inform* Inform = new TR_Matching_Inform[mvSize];
	cv::Mat Tmp;
	double MaxVal;
	int Index = 0;
	//비교할 영상의 크기를 변경
	cv::resize(src, Tmp, mvList[0].size());
	//쓰레드 핸들러에게 넘겨줄 정보저장
	for (size_t i = 0; i < mvSize; i++){
		Inform[i].CompareIMG = Tmp;
		Inform[i].MarkIMG = mvList[i];
		Inform[i].MaxVal = 0;
	}
	//쓰레드 실행
	for (size_t i = 0; i < mvSize; i++)
		Thread[i] = (HANDLE)_beginthreadex(NULL, NULL, Find_Mark, &Inform[i], NULL, NULL);
	//쓰레드 대기
	WaitForMultipleObjects(mvSize, Thread, TRUE, INFINITE);
	//가장 최대 값을 찾아서 저장
	MaxVal = Inform[0].MaxVal;
	for (size_t i = 1; i < mvSize; i++){
		if (MaxVal < Inform[i].MaxVal && Inform[i].MaxVal >0.2){
			MaxVal = Inform[i].MaxVal;
			Index = i;
		}
	}
	//가장 최대 값을 가지는 인덱스를 리턴;
	delete[] Thread;
	delete[] Inform;
	return Index;
}