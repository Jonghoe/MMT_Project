#include"Blob.h"
using namespace cv;
Blob::Blob(const Blob& b) : mvCenter(b.mvCenter),
mvRB(b.mvRB), mvLT(b.mvLT),mvID(b.mvID), mvPixelN(b.mvPixelN),
mvPrevImg(b.mvPrevImg.clone()){}
Blob::Blob():mvCenter(), mvID(), mvLT(), mvRB(), mvPixelN(){}
Blob::Blob(int* ranges, int PixelNum, int id,const Mat& previmg)
	: mvCenter(Point((ranges[0] + ranges[3]) / 2, (ranges[1] + ranges[2]) / 2)),
	mvRB(Point(ranges[3], ranges[2])), mvLT(Point(ranges[0], ranges[1])),
	mvID(id), mvPixelN(PixelNum),mvPrevImg(previmg(Rect(mvLT,mvRB)).clone()){}

Blob::Blob(int* ranges, int PixelNum, int id)
	: mvCenter(Point((ranges[0] + ranges[3]) / 2, (ranges[1] + ranges[2]) / 2)),
	mvRB(Point(ranges[3], ranges[2])), mvLT(Point(ranges[0], ranges[1])),
	mvID(id), mvPixelN(PixelNum){}
ID Blob::mGetID()const{ return this->mvID % 200; }
const Point& Blob::mGetCenter()const { return this->mvCenter; }
Point& Blob::mGetCenter() { return this->mvCenter; }
const Point& Blob::mGetRB()const { return this->mvRB; }
Point& Blob::mGetRB() { return this->mvRB; }
const Point& Blob::mGetLT()const { return this->mvLT; }
Point& Blob::mGetLT() { return this->mvLT; }
cv::Mat& Blob::mGetImg(){ return this->mvPrevImg; }
void Blob::mChange(const Blob& b){
	
	mvCenter = b.mvCenter;
	mvLT = b.mvLT;
	mvRB = b.mvRB;
}
