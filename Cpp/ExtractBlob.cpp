#include"ExtractBlob.h"
#include"MyFunction.h"
using namespace cv;
using namespace std;
void ExtractBlob::mvInitRange(){
	mvRange[0] = mvTmp.rows;
	mvRange[1] = mvTmp.cols;
	mvRange[2] = 0;
	mvRange[3] = 0;
}
void ExtractBlob::mvSetRange(const Point& Loc){	//  L U D R  순으로 정리
	Loc.x < mvRange[0] ? mvRange[0] = Loc.x : (mvRange[3] < Loc.x ? mvRange[3] = Loc.x : Loc.x);
	Loc.y < mvRange[1] ? mvRange[1] = Loc.y : (mvRange[2] < Loc.y ? mvRange[2] = Loc.y : Loc.y);
}
void ExtractBlob::mvBlobing(int row, int col, const Mat& img){
	
	if (!((0 < row && row < img.rows)&& 
		(0 < col && col < img.cols) )||
		img.ptr<uchar>(row)[col] == 0 ||
		mvTmp.ptr<uchar>(row)[col] != 0	)
		return ;
	++mvPixelNum;
	if (row == 0)
		cout << "A" << endl;
	mvTmp.ptr<uchar>(row)[col] = mvBlobNum;
	mvSetRange(Point(col,row));
	mvBlobing(row - 1, col, img);
	mvBlobing(row, col - 1, img);
	mvBlobing(row + 1, col, img);
	mvBlobing(row, col + 1, img);
}

vector<Blob> ExtractBlob::operator()(Mat& img){
	IMGSetting(img);
	vector<Blob>  vc;
	const uchar* ptrI = nullptr;
	uchar * ptrT = nullptr;
	if (img.rows*img.cols == 0 )
		return vc;
	
	mvBlobNum = 1;
	mvTmp.release();
	mvTmp.create(img.size(), img.type());
	mvTmp = Scalar::all(0);

	for (int row = 0; row < img.rows; row += 5){
		ptrI = img.ptr<uchar>(row);
		ptrT = mvTmp.ptr<uchar>(row);
		for (int col = 0; col < img.cols; col += 5){
			if (ptrI[col] != 0 && ptrT[col] == 0 ){
				mvInitRange();
				mvBlobing(row, col, img);
				if (mvPixelNum > mvDLimit)
					vc.push_back(Blob(mvRange, mvPixelNum, mvBlobNum++, img));
				mvPixelNum = 0;
			}
		}
	}
	return vc;
}