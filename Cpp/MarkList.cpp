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
	//���� ������ ũ�⸦ ����
	cv::resize(src, Tmp, mvList[0].size());
	//������ �ڵ鷯���� �Ѱ��� ��������
	for (size_t i = 0; i < mvSize; i++){
		Inform[i].CompareIMG = Tmp;
		Inform[i].MarkIMG = mvList[i];
		Inform[i].MaxVal = 0;
	}
	//������ ����
	for (size_t i = 0; i < mvSize; i++)
		Thread[i] = (HANDLE)_beginthreadex(NULL, NULL, Find_Mark, &Inform[i], NULL, NULL);
	//������ ���
	WaitForMultipleObjects(mvSize, Thread, TRUE, INFINITE);
	//���� �ִ� ���� ã�Ƽ� ����
	MaxVal = Inform[0].MaxVal;
	for (size_t i = 1; i < mvSize; i++){
		if (MaxVal < Inform[i].MaxVal && Inform[i].MaxVal >0.2){
			MaxVal = Inform[i].MaxVal;
			Index = i;
		}
	}
	//���� �ִ� ���� ������ �ε����� ����;
	delete[] Thread;
	delete[] Inform;
	return Index;
}