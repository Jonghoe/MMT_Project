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
		Thread[i] = (HANDLE)_beginthreadex(NULL, NULL, FindMark, &Inform[i], NULL, NULL);
	//������ ���
	WaitForMultipleObjects(mvSize, Thread, TRUE, INFINITE);
	//���� �ִ� ���� ã�Ƽ� ����
	MaxVal = Inform[0].MaxVal;
	cout << Inform[0].MaxVal<<endl;
	for (size_t i = 1; i < mvSize; i++){
		cout << Inform[i].MaxVal<<endl;
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