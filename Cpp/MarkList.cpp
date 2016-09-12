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
	//������ �ڵ鷯���� �Ѱ��� ��������
	for (size_t i = 0; i < mvSize; i++){
		//���� ������ ũ�⸦ ����
		resize(src, Tmp, mvList[i].size());
		Inform[i].CompareIMG = Tmp;
		Inform[i].MarkIMG = mvList[i];
		Inform[i].MaxVal = 0;
	}
	//������ ����
	for (size_t i = 0; i < mvSize; i++)
		Thread[i] = (HANDLE)_beginthreadex(NULL, NULL, FindMark, &Inform[i], NULL, NULL);
	//������ ���
	WaitForMultipleObjects(mvSize, Thread, TRUE, INFINITE);
	//���� �ִ� ���� ã�Ƽ� ����
	MaxVal = Inform[0].MaxVal;
	for (size_t i = 1; i < mvSize; i++){
		if (MaxVal < Inform[i].MaxVal && Inform[i].MaxVal >0.8){
			MaxVal = Inform[i].MaxVal;
			Index = i;
		}
	}
	//���� �ִ� ���� ������ �ε����� ����;
	delete[] Thread;
	delete[] Inform;
	return Index;
}