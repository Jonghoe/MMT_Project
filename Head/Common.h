#pragma once
#include<opencv.hpp>
enum MotionType{ MOVE, EXTEND, REDUCE };
enum State{ STOP, PLAY, UPDATE, END, START };
enum Target{ ALL, SUFFICIENT };
enum Type{ LOW, MIDDLE, HIGH };
enum MorT{ MAKE, TRACKING };
enum MVC{ M, V, C };			//Model,View,Control

typedef unsigned ID;
struct TRMatchingInform{				//ÅÛÇÃ¸´ ¸ÅÄª ±¸Á¶Ã¼
	cv::Mat CompareIMG;
	cv::Mat MarkIMG;
	int Index;
	double MaxVal;
};
