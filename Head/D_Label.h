#pragma once
#include"opencv.hpp"
#include<string>
#include<list>
using namespace cv;
using namespace std;

class D_Label{
	typedef size_t ID;
	enum KEY{ ENTER = 13, ESC = 27, SPACE = 32,E='e', UP = 2490368, LEFT = 2424832, RIGHT = 2555904, DOWN = 2621440 };
	enum ACT{M_LEFT,M_UP,M_DOWN,M_RIGHT,ERASE,NOTHING};
	const int LUMOVE = -5;
	const int RDMOVE = 5;

protected:
	ID mvID;
	Mat& Map, mvImg;
	list<ACT> mvAct;
	Point mvLT, mvCenter, mvAcc;
	size_t mvWidth, mvHeight;
	ACT mvPrev;
	
	void Move(Point diff){
		mvCenter.x += diff.x;
		mvCenter.y += diff.y;
		mvLT.x += diff.x;
		mvLT.y += diff.y;
	}
	void Accelerate(){
		if (mvPrev == *mvAct.begin())
			;
	}
	void Erase(Mat& Dst)const{
		Range row(mvLT.y, mvLT.y + mvHeight);
		Range col(mvLT.x, mvLT.x + mvWidth);
		Map(row, col) -= mvImg;
	}
	void Draw(Mat& Dst)const{
		Range row(mvLT.y, mvLT.y + mvHeight);
		Range col(mvLT.x, mvLT.x + mvWidth);
		Map(row, col) += mvImg;
	}
	bool Is_Ok(KEY k){
		int x, y;
		if (k == LEFT){ x = LUMOVE; y = 0; }
		else if (k == UP){ x = 0; y = LUMOVE; }
		else if (k == DOWN){ x = 0; y =RDMOVE; }
		else if (k == RIGHT){ x = RDMOVE; y = 0; }
			
		Point diff(x, y);
		Point move(mvLT + diff);

		return 0 <= move.x && move.x + mvWidth < Map.cols &&
			0 <= move.y&&move.y + mvHeight< Map.rows;
	}
public:
	D_Label(Mat& img, ID _id,Point& _p) :Map(img),mvAct(),mvID(_id),mvLT(_p),mvAcc(){
		mvImg = imread("Mark_0.jpg");
		mvWidth = mvImg.cols;
		mvHeight = mvImg.rows;
		mvCenter.x = mvLT.x + mvWidth / 2;
		mvCenter.y = mvLT.y + mvHeight / 2;
		std::string text;
		text = to_string(mvID);
		putText(mvImg, text, Point(mvWidth*0.25,mvHeight* 0.75), FONT_HERSHEY_SIMPLEX,1.0, Scalar(0, 0, 0));
	}
	
	int Action(){
		list<ACT>::iterator it=mvAct.begin();
		
		if (mvAct.size() == 0)
			;
		else if ((*it) == M_LEFT && Is_Ok(LEFT)){
				Move(Point(LUMOVE, 0));
				mvAct.pop_front();
		}
		else if ((*it) == M_UP && Is_Ok(UP)){
				Move(Point(0, LUMOVE));
				mvAct.pop_front();
		}
		else if ((*it) == M_DOWN && Is_Ok(DOWN)){
				Move(Point(0, RDMOVE));
				mvAct.pop_front();
		} 
		else if ((*it) == M_RIGHT &&Is_Ok(RIGHT)){
				Move(Point(RDMOVE, 0));
				mvAct.pop_front();
		}
		else if ((*it) == ERASE){
				Erase(Map);
				mvAct.pop_front();
				return -1;
		}
		
		Draw(Map);

		cout << mvCenter << endl;
		mvPrev = *it;
		return 0;
	}
	
	int SetAction(){
		int k;
		list<ACT>::iterator it;
		if (mvAct.size() != 0)
			it= mvAct.begin();
		do{
			k = waitKey();
		
			if (k == LEFT){
				mvAct.push_back(M_LEFT);
				cout << "Left" << endl;
			}
			else if (k == UP){
				mvAct.push_back(M_UP);
				cout << "UP" << endl;
			}
			else if (k == DOWN){
				mvAct.push_back(M_DOWN);
				cout << "DOWN" << endl;
			}
			else if (k == RIGHT){
				mvAct.push_back(M_RIGHT);
				cout << "RIGHT" << endl;
			}
			else if (k == E){
				mvAct.push_back(ERASE);
				cout << "ERASE" << endl;
			}
			if (k == 13)
				return 1;
		} while (k != ESC);
		return 0;
	}
	
	void SetCenter(Point center){	mvCenter = center;	}
	void SetWidth(int width){ mvWidth = width; }
	void SetHeight(int height){ mvHeight = height; }
};