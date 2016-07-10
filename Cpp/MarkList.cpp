#include "MarkList.h"
#include "MyFunction.h"
#include <process.h>
#include <Windows.h>
MarkList::MarkList():mvSize(0){}
void MarkList::mAddMark( cv::Mat& Mark){
	if (Mark.rows * Mark.cols == 0){
		cout << "Wrong Input" << endl;
		return;
	}
	cv::cvtColor(Mark, Mark, cv::COLOR_BGR2GRAY);
	mvList.push_back(Mark);
	mvSize++;
}
void MarkList::mDeleteMark(const cv::Mat&Mark){
	size_t i = 0;
	auto iter = mvList.begin();
	i = mFindMark(Mark);
	mvList.erase((iter + i));
}
int MarkList::mFindMark(const cv::Mat& src)const{
	HANDLE* Thread = new HANDLE[mvSize];
	TRMatchingInform* Inform = new TRMatchingInform[mvSize];
	cv::Mat Tmp;
	double MaxVal;
	int Index = 0;
	if (mvList.size() == 0)
		return 0;
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
		Thread[i] = (HANDLE)_beginthreadex(NULL, NULL, FindMark, &Inform[i], NULL, NULL);
	//쓰레드 대기
	WaitForMultipleObjects(mvSize, Thread, TRUE, INFINITE);
	//가장 최대 값을 찾아서 저장
	MaxVal = Inform[0].MaxVal;
	cout << Inform[0].MaxVal<<endl;
	for (size_t i = 1; i < mvSize; i++){
		cout << Inform[i].MaxVal<<endl;
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